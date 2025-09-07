# ebsrc

This is currently a disassembly of the game known as Earthbound in the west and Mother 2 in the east.

Currently, I'm converting ASM to C code using LLMs whenever I can.

Reference the [original repo](https://github.com/Herringway/ebsrc) in order to get a working build out of ASM files 

In this repo, ASM build is broken due to renaming files to ".converted.asm" which have been ported to C.

## TODO

**The main task is converting ASM to C. This is being tracked in the file TODO.md**

There are more markdown files scattered across the project directory. They all hold important information in some way. I'm still messing with an appropiate workflow to handle and referencing ROM data. Not striving for a bit-to-bit reprodudction. Only 100% functional in hopes we have clean code that can be ported to other platforms in the long run.

For now, striving to have a working SNES build.