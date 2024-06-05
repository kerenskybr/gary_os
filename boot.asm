ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:step2

; DISK - READ SECTOR(S) INTO MEMORY
; AH = 02h
; AL = number of sectors to read (must be nonzero)
; CH = low eight bits of cylinder number
; CL = sector number 1-63 (bits 0-5)
; high two bits of cylinder (bits 6-7, hard disk only)
; DH = head number
; DL = drive number (bit 7 set for hard disk)
; ES:BX -> data buffer

; Return:
; CF set on error
; if AH = 11h (corrected ECC error), AL = burst length
; CF clear if successful
; AH = status (see #00234)
; AL = number of sectors transferred (only valid if CF set for some
; BIOSes)


step2:
    cli                     ; Clear interruptions
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti                     ; enable interruptions

    ; Writing operations
    mov ah, 2               ; REad sector command
    mov al, 1               ; One sector to read
    mov ch, 0               ; Cylinder low eight bits
    mov cl, 2               ; Read sector 2
    mov dh, 0               ; Head number
    mov bx, buffer
    int 0x13
    jc error

    mov si, buffer
    call print

    jmp $

error:
    mov si, error_message
    call print
    jmp $

print:
    mov bx, 0
    mov bh, 02h             ; bg color. Not working tho

.loop:
    lodsb
    cmp al, 0               ; if = zero, done
    je .done
    call print_char
    jmp .loop

.done:
    ret

print_char:
    mov ah, 0eh             ; VIDEO - TELETYPE OUTPUT http://www.ctyme.com/intr/int-10.htm
    ; mov al, 'A'           ; Prints AAAAAAAA instead hello world
    int 0x10                ;bios routine
    ret

error_message: db 'Failed to load sector'

times 510-($ - $$) db 0     ; fill at least 510 bytes of data. Otherwise, will output zeros after the hello world thing
dw 0xAA55                   ; bootloades sector

; 7E00
buffer: 


; https://wiki.osdev.org/


; Compile
; nasm -f bin ./boot.asm -o ./boot.bin

; Inspect
; ndisasm ./boot.bin

; Run in Qemu
; qemu-system-x86_64 -hda ./boot.bin

; To write to a usb disk
; dd if=./boot.bin of=/dev/<usb disk>