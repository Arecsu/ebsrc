#!/usr/bin/env python3
"""
C Bank File Generator for EarthBound Phase 1 Build System

Converts ASM bank configuration files to Pure C implementations while
maintaining copyright compliance through ROM data extraction.

Usage: python3 tools/generate_c_banks.py
"""

import os
import re
from pathlib import Path

def analyze_asm_bank_file(bank_path):
    """Analyze ASM bank file to extract function references and ROM data"""
    functions = []
    rom_data = []
    
    try:
        with open(bank_path, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                line = line.strip()
                
                # Extract .INCLUDE references to functions
                if line.startswith('.INCLUDE') and not any(x in line for x in ['common.asm', 'config.asm', 'structs.asm', 'symbols/']):
                    # Extract filename from .INCLUDE "path/file.asm"
                    match = re.search(r'\.INCLUDE\s+"([^"]+)"', line)
                    if match:
                        include_path = match.group(1)
                        functions.append({
                            'asm_file': include_path,
                            'line_num': line_num,
                            'converted': include_path.endswith('.converted.asm')
                        })
                
                # Extract .INCBIN references to ROM data
                elif '.INCBIN' in line.upper():
                    match = re.search(r'\.INCBIN\s+"([^"]+)"', line, re.IGNORECASE)
                    if match:
                        rom_data.append({
                            'data_file': match.group(1),
                            'line_num': line_num
                        })
                        
    except Exception as e:
        print(f"Warning: Could not analyze {bank_path}: {e}")
        
    return functions, rom_data

def check_c_function_exists(asm_path):
    """Check if ASM function has been converted to C"""
    # Extract function name from ASM path
    filename = os.path.basename(asm_path)
    func_name = filename.replace('.asm', '').replace('.converted', '')
    
    # Check in main C source files
    c_files = ['src/battle.c', 'src/audio.c', 'src/system.c', 'src/psi.c', 'src/inventory.c']
    
    for c_file in c_files:
        if os.path.exists(c_file):
            try:
                with open(c_file, 'r') as f:
                    content = f.read()
                    if f'btlact_{func_name}(' in content or f'{func_name}(' in content:
                        return True, c_file
            except:
                pass
                
    return False, None

def generate_c_bank_file(bank_num, functions, rom_data, output_path):
    """Generate C bank file with function calls and ROM data inclusion"""
    
    bank_hex = f"{bank_num:02X}"
    
    c_content = f'''/* 
 * Bank {bank_hex} - Generated C Implementation
 * Auto-generated from ASM bank configuration
 * 
 * This file replaces src/bankconfig/US/bank{bank_hex.lower()}.asm
 * Functions have been converted to C, ROM data included via .incbin
 */

#pragma code-name ("BANK{bank_hex}")

// Standard includes
#include "battle.h"
#include "audio.h" 
#include "system.h"
#include "psi.h"
#include "inventory.h"
#include "rom_data.h"

// =============================================================================
// CONVERTED FUNCTIONS (now implemented in C)
// =============================================================================

'''
    
    # Document converted functions
    converted_count = 0
    pending_count = 0
    
    for func_info in functions:
        converted, c_file = check_c_function_exists(func_info['asm_file'])
        if converted:
            c_content += f"// ✓ {func_info['asm_file']} → C function in {c_file}\n"
            converted_count += 1
        else:
            c_content += f"// ⏳ {func_info['asm_file']} → Pending conversion\n"
            pending_count += 1
    
    c_content += f'''
// Conversion Status: {converted_count} converted, {pending_count} pending

// =============================================================================
// ROM DATA INCLUSION (copyright-safe via extraction)
// =============================================================================

'''
    
    # ROM data inclusion
    if rom_data:
        c_content += 'extern "C" {\n'
        
        for i, data_info in enumerate(rom_data):
            data_file = data_info['data_file']
            symbol_name = f"ROM_DATA_{bank_hex}_{i}"
            
            c_content += f'''
// ROM data from {data_file}
extern const unsigned char {symbol_name}[];
asm(".segment \\"ROMDATA_GRAPHICS\\"");
asm("{symbol_name}: .incbin \\"{data_file}\\"");
'''
        
        c_content += '}\n'
    else:
        c_content += '// No ROM data in this bank\n'
    
    c_content += f'''
// =============================================================================  
// BANK INITIALIZATION (if needed)
// =============================================================================

void bank_{bank_hex.lower()}_init(void) {{
    // Bank-specific initialization code if needed
    // This runs when the bank is first loaded
}}

/*
 * Build Instructions:
 * cc65 {output_path} --code-name BANK{bank_hex} -o build/bank{bank_hex.lower()}.o
 */
'''
    
    return c_content

def generate_rom_data_c_file(all_rom_data):
    """Generate centralized ROM data C file"""
    
    content = '''/*
 * ROM Data Inclusion - Phase 1 Implementation
 * Auto-generated from bank analysis
 * 
 * This file contains all .incbin directives for ROM data inclusion
 * Data is extracted from donor.sfc and included at build time
 */

// =============================================================================
// ROM DATA SEGMENTS  
// =============================================================================

// Graphics data
asm(".segment \\"ROMDATA_GRAPHICS\\"");
'''
    
    # Collect unique ROM data files
    unique_files = set()
    for bank_data in all_rom_data.values():
        for data_info in bank_data:
            unique_files.add(data_info['data_file'])
    
    # Generate .incbin directives
    for i, data_file in enumerate(sorted(unique_files)):
        symbol_name = f"ROM_DATA_{i:03d}"
        content += f'''
extern const unsigned char {symbol_name}[];
asm("{symbol_name}: .incbin \\"{data_file}\\"");
'''
    
    content += '''
// =============================================================================
// DATA ACCESS FUNCTIONS (Phase 1)
// =============================================================================

#include "rom_data.h"

const unsigned char* get_rom_data(unsigned short data_id) {
    // Simple lookup table for ROM data access
    // Phase 2 will implement more sophisticated access
    
    switch (data_id) {
'''
    
    for i, data_file in enumerate(sorted(unique_files)):
        content += f'        case {i}: return ROM_DATA_{i:03d};\n'
        
    content += '''        default: return NULL;
    }
}

// Phase 1: Basic ROM data access
// Phase 2: Will add structured data access with metadata
'''
    
    return content

def main():
    """Generate C bank files from ASM bank configuration"""
    
    print("EarthBound C Bank Generator - Phase 1")
    print("====================================")
    
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    
    bank_config_dir = project_root / "src" / "bankconfig" / "US"
    output_dir = project_root / "src" / "generated"
    
    # Create output directory
    output_dir.mkdir(exist_ok=True)
    
    # Process each bank file
    all_functions = {}
    all_rom_data = {}
    
    bank_files = sorted(bank_config_dir.glob("bank*.asm"))
    
    for bank_file in bank_files:
        # Extract bank number
        bank_match = re.search(r'bank([0-9a-fA-F]+)\.asm', bank_file.name)
        if not bank_match:
            continue
            
        bank_num = int(bank_match.group(1), 16)
        
        print(f"Processing Bank {bank_num:02X}: {bank_file.name}")
        
        # Analyze ASM bank file
        functions, rom_data = analyze_asm_bank_file(bank_file)
        
        all_functions[bank_num] = functions
        all_rom_data[bank_num] = rom_data
        
        print(f"  Functions: {len(functions)}")
        print(f"  ROM data: {len(rom_data)}")
        
        # Generate C bank file
        c_content = generate_c_bank_file(bank_num, functions, rom_data, f"src/generated/bank{bank_num:02x}.c")
        
        output_file = output_dir / f"bank{bank_num:02x}.c"
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(c_content)
            
        print(f"  Generated: {output_file}")
    
    # Generate centralized ROM data file  
    rom_data_content = generate_rom_data_c_file(all_rom_data)
    rom_data_file = project_root / "src" / "rom_data_c.c"
    
    with open(rom_data_file, 'w', encoding='utf-8') as f:
        f.write(rom_data_content)
    
    print(f"\nGenerated centralized ROM data file: {rom_data_file}")
    
    # Summary
    total_functions = sum(len(funcs) for funcs in all_functions.values())
    total_rom_data = sum(len(data) for data in all_rom_data.values())
    
    print(f"\nGeneration Summary:")
    print(f"  Banks processed: {len(bank_files)}")
    print(f"  Total functions: {total_functions}")
    print(f"  Total ROM data files: {total_rom_data}")
    print(f"  Output directory: {output_dir}")
    
    print(f"\nNext steps:")
    print(f"  1. Review generated C files in {output_dir}")
    print(f"  2. Run 'make extract' to ensure ROM data is available")
    print(f"  3. Run 'make earthbound' to build with Pure C system")

if __name__ == "__main__":
    main()