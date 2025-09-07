#!/usr/bin/env python3
"""
ROM Variant Detection for EarthBound Build System
Detects ROM variant by checking header/checksum to use correct YAML config

Supported variants:
- US: earthbound.yml (US retail ROM)
- US19950327: earthbound-1995-03-27.yml (US prototype ROM)
- JP: mother2.yml (Japanese ROM)
"""
import sys
import hashlib

def detect_rom_variant(rom_path):
    """Detect EarthBound ROM variant from MD5 checksum"""
    
    # Known ROM checksums for accurate detection
    ROM_CHECKSUMS = {
        # US retail ROM (most common)
        "a864b2e5c141d2dec1c4cbed75a42a85": "US",  # EarthBound (USA)
        
        # Japanese ROM  
        "a086f6648288c2858329b1c9c6e49d82": "JP",  # Mother 2 - Gyiyg no Gyakushuu (Japan)
        
        # US prototype ROM (if available)
        # Add prototype checksum here when available:
        # "prototype_md5_here": "US19950327",
    }
    
    try:
        # Calculate MD5 checksum
        with open(rom_path, 'rb') as f:
            rom_data = f.read()
            rom_md5 = hashlib.md5(rom_data).hexdigest()
            
        print(f"ROM MD5: {rom_md5}", file=sys.stderr)
        
        # Look up variant by checksum
        if rom_md5 in ROM_CHECKSUMS:
            variant = ROM_CHECKSUMS[rom_md5]
            print(f"Detected variant: {variant}", file=sys.stderr)
            return variant
        else:
            # Unknown ROM - try header fallback
            print(f"Unknown ROM checksum: {rom_md5}", file=sys.stderr)
            return detect_rom_by_header(rom_path)
                    
    except Exception as e:
        print(f"Error detecting ROM: {e}", file=sys.stderr)
        print("Fallback: Defaulting to US", file=sys.stderr)
        return "US"  # Default fallback

def detect_rom_by_header(rom_path):
    """Fallback detection using ROM header (less reliable)"""
    try:
        with open(rom_path, 'rb') as f:
            # Read ROM header area (0x7FC0-0x7FFF contains SNES header)
            f.seek(0x7FC0)  
            header = f.read(0x40)
            
            # Check ROM title at 0x7FC0 (21 bytes)
            title = header[0:21].decode('ascii', errors='ignore').strip()
            
            print(f"ROM Title (header): '{title}'", file=sys.stderr)
            
            # Detect by ROM title patterns
            if "EARTH BOUND" in title:
                if "1995" in title or "03-27" in title:
                    return "US19950327"  # US prototype
                else:
                    return "US"          # US retail
            elif "MOTHER 2" in title:
                return "JP"              # Japanese
            else:
                # Final fallback - assume US
                print("Header detection failed, assuming US", file=sys.stderr)
                return "US"
                
    except Exception as e:
        print(f"Header detection failed: {e}", file=sys.stderr)
        return "US"

def main():
    """Main entry point"""
    if len(sys.argv) != 2:
        print("Usage: detect_rom_variant.py <rom_file>", file=sys.stderr)
        print("", file=sys.stderr)
        print("Detects EarthBound ROM variant and outputs:", file=sys.stderr)
        print("  US          - US retail ROM (earthbound.yml)", file=sys.stderr)
        print("  US19950327  - US prototype ROM (earthbound-1995-03-27.yml)", file=sys.stderr) 
        print("  JP          - Japanese ROM (mother2.yml)", file=sys.stderr)
        sys.exit(1)
    
    rom_file = sys.argv[1]
    
    # Check if ROM file exists
    try:
        with open(rom_file, 'rb') as f:
            pass
    except FileNotFoundError:
        print(f"Error: ROM file '{rom_file}' not found", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: Cannot read ROM file '{rom_file}': {e}", file=sys.stderr)
        sys.exit(1)
    
    # Detect variant
    variant = detect_rom_variant(rom_file)
    print(variant)

if __name__ == "__main__":
    main()