ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:step2

step2:
    cli                     ; Clear interruptions
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti                     ; enable interruptions

    mov si, message         ; move message to si address
    call print
    jmp $


print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0               ; if no zero, done
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh             ; VIDEO - TELETYPE OUTPUT http://www.ctyme.com/intr/int-10.htm
    ; mov al, 'A'
    int 0x10                ;bios routine
    ret

message : db 'Hello World!', 0

times 510-($ - $$) db 0
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