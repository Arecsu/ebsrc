#!/bin/bash
# ASM to C Conversion Progress Tracker (Corrected)
# Excludes ROM data files, symbols, and bank configs that are auto-generated

echo "=== EARTHBOUND DECOMPILATION PROGRESS (EXECUTABLE CODE) ==="
echo "Excluding ROM data, symbols, and bank config files (handled automatically)"
echo

# Function to check if a directory should be excluded
should_exclude_dir() {
    local dir="$1"
    case "$dir" in
        *bankconfig*|*bin*|*symbols*) return 0 ;;
        *) return 1 ;;
    esac
}

# Function to check if a file should be excluded  
should_exclude_file() {
    local file="$1"
    case "$file" in
        *.symbols.asm|*bankconfig*|*/bin/*) return 0 ;;
        *) return 1 ;;
    esac
}

for dir in $(find src -type d | sort); do
    if [ "$dir" = "src" ]; then continue; fi
    if should_exclude_dir "$dir"; then continue; fi
    
    # Count only executable ASM files (excluding data/symbols)
    total=0
    converted=0
    
    while IFS= read -r -d '' file; do
        if ! should_exclude_file "$file"; then
            total=$((total + 1))
            if [[ "$file" == *.converted.asm ]]; then
                converted=$((converted + 1))
            fi
        fi
    done < <(find "$dir" -maxdepth 1 \( -name "*.asm" -o -name "*.converted.asm" \) -print0 2>/dev/null)
    
    if [ $total -eq 0 ]; then continue; fi
    
    remaining=$((total - converted))
    percentage=$(( (converted * 100) / total ))
    
    dirname=$(basename "$dir")
    printf "%-20s %2d/%2d files (%3d%%)\n" "$dirname:" "$converted" "$total" "$percentage"
done

echo
echo "=== SUMMARY ==="

# Count all executable files (excluding ROM data, symbols, bank configs)
total_executable=0
converted_executable=0

while IFS= read -r -d '' file; do
    if ! should_exclude_file "$file"; then
        total_executable=$((total_executable + 1))
        if [[ "$file" == *.converted.asm ]]; then
            converted_executable=$((converted_executable + 1))
        fi
    fi
done < <(find src \( -name "*.asm" -o -name "*.converted.asm" \) -print0 2>/dev/null)

remaining_executable=$((total_executable - converted_executable))
if [ $total_executable -gt 0 ]; then
    overall_percentage=$(( (converted_executable * 100) / total_executable ))
else
    overall_percentage=0
fi

echo "=== EXECUTABLE CODE CONVERSION ==="
echo "Total converted: $converted_executable/$total_executable ($overall_percentage%)"
echo "Remaining: $remaining_executable files"
echo
echo "=== EXCLUDED (AUTO-HANDLED) ==="
excluded_count=$(find src \( -name "*.asm" -o -name "*.converted.asm" \) | while read file; do 
    if should_exclude_file "$file"; then echo "$file"; fi 
done | wc -l)
echo "ROM data/symbols/configs: $excluded_count files (handled by tools/generate_rom_data_header.py)"
echo
echo "=== TOTAL PROJECT ==="
total_all=$(find src \( -name "*.asm" -o -name "*.converted.asm" \) | wc -l)
echo "All ASM files: $total_all"
echo "Architecture: $excluded_count auto-generated + $total_executable executable = $total_all total"