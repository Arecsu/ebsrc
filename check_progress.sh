#!/bin/bash
# ASM to C Conversion Progress Tracker

echo "=== EARTHBOUND DECOMPILATION PROGRESS ==="
echo

for dir in $(find src -type d | sort); do
    if [ "$dir" = "src" ]; then continue; fi
    
    total=$(find "$dir" -maxdepth 1 -name "*.asm" -o -name "*.converted.asm" | wc -l)
    if [ $total -eq 0 ]; then continue; fi
    
    converted=$(find "$dir" -maxdepth 1 -name "*.converted.asm" | wc -l)
    remaining=$(find "$dir" -maxdepth 1 -name "*.asm" | grep -v ".converted" | wc -l)
    percentage=$(( (converted * 100) / total ))
    
    dirname=$(basename "$dir")
    printf "%-20s %2d/%2d files (%3d%%)\n" "$dirname:" "$converted" "$total" "$percentage"
done

echo
echo "=== SUMMARY ==="
total_files=$(find src -name "*.asm" -o -name "*.converted.asm" | wc -l)
converted_files=$(find src -name "*.converted.asm" | wc -l)
remaining_files=$(find src -name "*.asm" | grep -v ".converted" | wc -l)
overall_percentage=$(( (converted_files * 100) / total_files ))

echo "Total converted: $converted_files/$total_files ($overall_percentage%)"
echo "Remaining: $remaining_files files"