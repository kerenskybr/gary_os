[BITS 32]

section .asm

global print:function
global getkey:function
global garyos_malloc:function
global garyos_free:function
global garyos_putchar:function

; void print(const char* message)
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1 ; command print
    int 0x80
    add esp, 4

    pop ebp
    ret

; int getkey()
getkey:
    push ebp
    mov ebp, esp
    mov eax, 2 ; command getkey
    int 0x80
    pop ebp
    ret 

; void garyos_putchar(char c)
garyos_putchar:
    push ebp
    mov ebp, esp
    mov eax, 3 ; command putchar
    push dword [ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret

; void* garyos_malloc(size_t size)
garyos_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4 ; malloc
    push dword[ebp+8] ; size
    int 0x80
    add esp, 4
    pop ebp
    ret

; void garyos_free(void* ptr)
garyos_free:
    push ebp
    mov ebp, esp
    mov eax, 5 ; command 5 free
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret 