global _start

        .data:

message: db "Hello World!", 0xA, 0
message_length equ $-message

        .text:

_start:
        mov eax, message
        jmp _print_string
return_print:
        mov eax, 0x1
        xor ebx, ebx
        int 0x80

;* input:
;*      eax: string ptr
_print_string:

        mov ecx, eax    ; store
        jmp _string_length
string_length_return:
        mov edx, eax       ; length
        mov eax, 0x4       ; write syscall
        mov ebx, 1         ; fd

        int 0x80           ; interrupt -> syscall

        jmp return_print

;*(String must end with NULL)
;* input: 
;*      eax: string ptr
;*output:
;*      eax: length
_string_length:
        mov ebx, 0                  ; store length
looper:
        add eax, 1
        add ebx, 1
        cmp byte [eax], 0x0         ; while (*ecx != 0)
        jne looper     ; {}
        mov eax, ebx
        jmp string_length_return