; ==============================================================================
; SNES System Vectors - CRITICAL FOR BOOT (.critical.asm extension)
; ==============================================================================
; 
; NEVER CONVERT TO C - Required for SNES hardware boot functionality
; The .critical.asm extension marks this as permanently ASM-only
; 
; These vectors MUST stay at fixed addresses:
; - RESET_VECTOR: Referenced in HEADER segment at $C0FFB0
; - NMI_VECTOR: Hardware NMI handler entry point  
; - IRQ_VECTOR: Hardware IRQ handler entry point

.SEGMENT "BANK00"

; Include system vector implementations (.critical.asm = never convert to C)
.INCLUDE "../system/reset_vector.critical.asm"
.INCLUDE "../system/nmi_vector.critical.asm"  
.INCLUDE "../system/irq_vector.critical.asm"

; Export symbols for linker
.EXPORT RESET_VECTOR
.EXPORT NMI_VECTOR  
.EXPORT IRQ_VECTOR

; These get referenced in HEADER segment (bank00.asm)
; HEADER contains .ADDR RESET_VECTOR, etc.