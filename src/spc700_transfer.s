; spc700_transfer.s - 65816-specific bank and direct page management for SPC700 data transfer

; Export symbols for C linkage (note the leading underscore for cc65 compatibility)
.export _spc700_transfer_setup
.export _spc700_transfer_restore

; Segment to match audio.c's BANK02
.segment "BANK02"

; Setup: Save current bank and direct page, set to 0
.proc _spc700_transfer_setup: near
    phb         ; Push bank register
    phd         ; Push direct page register
    pea $0000   ; Push 0x0000
    plb         ; Pull to bank register (set bank to 0)
    plb         ; Pull again (discard second byte, as pea pushes 16 bits)
    pea $0000   ; Push 0x0000
    pld         ; Pull to direct page register (set direct page to 0)
    rts
.endproc

; Restore: Restore saved bank and direct page
.proc _spc700_transfer_restore: near
    pld         ; Pull direct page register
    plb         ; Pull bank register
    rts
.endproc