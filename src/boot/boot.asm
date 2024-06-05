ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:step2

step2:
    cli                     ; Clear interruptions
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    ; mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti                     ; enable interruptions

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

; GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:                   ; CS should point to dis
    dw 0xffff               ; Segment limit first 0-15 bits
    dw 0                    ; Base first 0-15 bits
    dw 0                    ; Base 16-23 bits
    db 0x9a                 ; Access byte
    db 11001111b            ; High 4 bit flas and low 4 bit flags
    db 0                    ; Bse 24-31 bits

; offset 0x10
gdt_data:                   ; ds, ss, es, fs, gs
    dw 0xffff               ; Segment limit first 0-15 bits
    dw 0                    ; Base first 0-15 bits
    dw 0                    ; Base 16-23 bits
    db 0x92                 ; Access byte
    db 11001111b            ; High 4 bit flas and low 4 bit flags
    db 0                    ; Bse 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

[BITS 32]
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp $

times 510-($ - $$) db 0     ; fill at least 510 bytes of data. Otherwise, will output zeros after the hello world thing
dw 0xAA55                   ; bootloades sector


; https://wiki.osdev.org/


; Compile
; nasm -f bin ./boot.asm -o ./boot.bin

; Inspect
; ndisasm ./boot.bin

; Run in Qemu
; qemu-system-x86_64 -hda ./boot.bin

; To write to a usb disk
; dd if=./boot.bin of=/dev/<usb disk>

; gdb stuff
; target remote | qemu-system-x86_64 -hda ./boot.bin -S -gdb stdio