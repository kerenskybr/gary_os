ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0:step2

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
    db 0                    ; Base 16-23 bits
    db 0x9a                 ; Access byte
    db 11001111b            ; High 4 bit flas and low 4 bit flags
    db 0                    ; Bse 24-31 bits

; offset 0x10
gdt_data:                   ; ds, ss, es, fs, gs
    dw 0xffff               ; Segment limit first 0-15 bits
    dw 0                    ; Base first 0-15 bits
    db 0                    ; Base 16-23 bits
    db 0x92                 ; Access byte
    db 11001111b            ; High 4 bit flas and low 4 bit flags
    db 0                    ; Bse 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1              ; Sector 1, cause bios is 0
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

; https://wiki.osdev.org/ATA_read/write_sectors
; Start of ata driver 
ata_lba_read:
    mov ebx, eax            ; Backup the LBA
    shr eax, 24             ; Send the highest 8 bits of the lba to hard disk controller
    or eax, 0xE0            ; Select master drive
    mov dx, 0x1F6
    out dx, al              ; Finished sending the 8 highest bits to the lba

    ; Send the total sector to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending

    mov eax, ebx             ; Restore backup from later
    mov dx, 0x1F3
    out dx, al

    ; Send more bits of the lba
    mov dx, 0x1F4
    mov eax, ebx
    shr eax, 8
    out dx, al
    ; finished

    ; Send upper 16 bits of the lba
    mov dx, 0x1F5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

.next_sector:
    push ecx

.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    ; Need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector

    ret

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
; target remote | qemu-system-x86_64 -hda ./bin/boot.bin -S -gdb stdio
; c
; layout asm
; info registers