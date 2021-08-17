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

        mov eax, 0xabc
        mov ebx, s1
        call _int_to_string_hex
        call _print_string
        call _newline

        mov eax, 0xA
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
        pusha
        ; mov ecx, eax   ; mov char
        mov ecx, c1
        mov [ecx], eax
        mov eax, 0x4   ; write
        mov ebx, 1
        mov edx, 1      

        int 0x80       ; interrupt
        popa
        ret

_newline:
        push eax
        mov eax, 0xA
        call _print_char
        pop eax
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

;* (converts least sig nibble to char)
;* input:
;*      eax: int
;* output:
;*      eax: char 
_digit_to_char:

        and eax, 0xF    ; mask

        cmp eax, 0xA
        jl .under_ten
        add eax, 0x7
.under_ten:
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

;* (gets # of hex digits in an int)
;* input:
;*      eax: int
;* output:
;*      eax: # digits
num_hex:
        push ebx
        mov ebx, 0
.looper:
        add ebx, 2
        shr eax, 8
        cmp eax, 0x0
        jne .looper

        mov eax, ebx
        pop ebx
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

        cmp eax, 0
        jge .continue

        neg eax
        mov byte [ebx], '-'

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

        mov eax, ebx

        ret

;* (converts an hex int to a string)
;* input:
;*      eax: int
;*      ebx: ptr
;* output
;*      eax: ptr
_int_to_string_hex:

        mov byte [ebx], '0'   ; lead with 0x
        inc ebx
        mov byte [ebx], 'x'
        inc ebx

        push eax              ; store

        call num_hex

        add ebx, eax          ; increment by num_hex

        mov byte [ebx], 0     ; null terminate
        dec ebx               ; move back

        mov ecx, eax          ; i
        pop eax               ; restore input

.looper:
        push eax
        call _digit_to_char   ; get char
        
        mov byte [ebx], AL    ; store
        dec ebx 
        pop eax
        shr eax, 4            ; right shift -> 4 bits

        dec ecx               ; i--
        cmp ecx, 0            ; while (i > 0)
        jne .looper

        sub ebx, 2            ; back to start
        mov eax, ebx          ; ret val
        ret