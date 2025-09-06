# Build Tools

This directory contains the build tools required for the ebsrc project.

## Tools Included

- **ebbinex** - ROM asset extractor for Earthbound
  - Built from: https://github.com/Herringway/ebbinex
  - Language: D (compiled with LDC)
  - Usage: `./ebbinex <yaml_config> <rom_file>`

- **spcasm** - SPC700 assembler for SNES audio
  - Built from: https://github.com/kleinesfilmroellchen/spcasm
  - Language: Rust (compiled with Cargo)
  - Usage: `./spcasm -f plain <input.s> <output.bin>`

## Notes

- These tools are built locally and not included in version control
- The `tools/` directory is ignored in `.gitignore`
- The Makefile references these tools with `./tools/toolname`

## Rebuilding Tools

If you need to rebuild these tools:

```bash
# For ebbinex (requires LDC D compiler)
git clone https://github.com/Herringway/ebbinex.git
cd ebbinex
dub build --build=release --compiler=ldc2
cp ebbinex ../tools/

# For spcasm (requires Rust/Cargo)
git clone https://github.com/kleinesfilmroellchen/spcasm.git
cd spcasm  
cargo build --release
cp target/release/spcasm ../tools/
```