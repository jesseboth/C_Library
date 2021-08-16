global _start

section .data

hello: db "Hello World!", 0xA, 0

error_digit: db "Error: -9 < n > 9", 0xA, 0

section .bss
c1      resb 2
s1      resb 12

section .text

_start:
        mov eax, hello
        call _print_string

        mov eax, 0x6
        call _digit_to_char
        call _print_char
        call _newline

        mov eax, -1234456
        call _num_digits
        call _digit_to_char
        call _print_char
        call _newline

        mov eax, -2147483647
        mov ebx, s1
        call _int_to_string
        mov eax, s1
        call _print_string
        call _newline

_exit_program:

        mov eax, 0x1
        xor ebx, ebx
        int 0x80

;* (print a string)
;* input:
;*      eax: string ptr
_print_string:

        mov ecx, eax    ; store
        call _string_length

        mov edx, eax       ; length
        mov eax, 0x4       ; write syscall
        mov ebx, 1         ; fd

        int 0x80           ; interrupt -> syscall

        ret
;*input:
;*      eax: char
_print_char:
        ; mov ecx, eax   ; mov char
        mov ecx, c1
        mov [ecx], eax
        mov eax, 0x4   ; write
        mov ebx, 1
        mov edx, 1      

        int 0x80       ; interrupt
        ret

_newline:
        mov eax, 0xA
        call _print_char
        ret

;*(String must end with NULL)
;* input: 
;*      eax: string ptr
;*output:
;*      eax: length
_string_length:
        mov ebx, 0            ; store length
.looper:
        add eax, 1
        add ebx, 1
;*          size  *val  null                         
        cmp byte [eax], 0x0   ; while (*ecx != 0)
        jne .looper            ; {}
        mov eax, ebx
        ret

;* (converts single digit to char)
;* input:
;*      eax: int
;* output:
;*      eax: char 
_digit_to_char:
        cmp eax, 0xA
        jl .no_error

        mov eax, error_digit   ; trigger error
        call _print_string
        jmp _exit_program      ; prevent errors
.no_error:
        add eax, 0x30
        ret

;* (gets # of digits in an int)
;* input:
;*      eax: int
;* output:
;*      eax: # digits
_num_digits:
        mov ebx, 0
        mov ecx, 10 ; divide
        cmp eax, 0 
        je .zero    ; n == 0
        jg .looper  ; n > 0     
        
        neg eax
.looper:
        inc ebx
        mov edx, 0
        div ecx ; eax / 10

        cmp eax, 0
        jne .looper
        mov eax, ebx
        jmp .exit
.zero:
        mov eax, 1
.exit:
        ret

        


;* (converts an int to a string)
;* input:
;*      eax: int
;*      ebx: ptr
;* output
;*      eax: ptr
_int_to_string:
        push eax ; preserve
        push ebx 

        call _num_digits
        mov ecx, eax           ; store num_digits

        pop ebx 
        pop eax
        push eax

        cmp eax, 0
        jge .continue

        neg eax
        mov byte [ebx], '-'
        ; inc ebx
.continue:
        add ebx, ecx
        inc ebx
        mov byte [ebx], 0 ; null char
        
        mov ecx, 10
.looper:
        dec ebx         ;next char ptr
        mov edx, 0
        div ecx ; eax / 10
        add edx, 0x30
        mov byte [ebx], DL
        cmp eax, 0
        jne .looper


        pop eax
        mov eax, ebx

        ret
