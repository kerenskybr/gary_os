[BITS 32]

global print:function
global getkey:function
global garyos_malloc:function

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