section .bss
h1      resb 32
s1      resb 32
b1      resb 5
c1      resb 2

section .text


;* (print a string)
;* input:
;*      eax: string ptr
_print_string:
        pusha
        mov ecx, eax    ; store
        call _string_length

        mov edx, eax       ; length
        mov eax, 0x4       ; write syscall
        mov ebx, 1         ; fd

        int 0x80           ; interrupt -> syscall
        popa
        ret

;* (prints integer)
;* input: 
;*      eax: int
_print_int:
        pusha
        mov ebx, s1
        call _int_to_string
        mov eax, s1
        call _print_string 
        popa
        ret

;* (prints hexadecimal)
;* input: 
;*      eax: int
_print_hex:
        pusha
        mov ebx, h1
        call _int_to_string_hex
        call _print_string
        popa
        ret

;* (prints a single char)
;* input:
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

;* (prints a hex byte)
;* input:
;*      eax: byte
_print_byte:
        pusha
        push ebx
        push eax

        mov ebx, b1
        add ebx, 2
        mov byte [ebx], 0x0
        dec ebx

        call _digit_to_char
        mov byte [ebx], AL
        dec ebx
        pop eax
        shr eax, 4
        call _digit_to_char
        mov byte [ebx], AL

        mov eax, ebx
        pop ebx

        call _print_string
        popa
        ret

;* (prints a newline)
_newline:
        push eax
        mov eax, 0xA
        call _print_char
        pop eax
        ret

;* (prints a space)
_space:
        push eax
        mov eax, 0x20
        call _print_char
        pop eax
        ret

;* (prints a tab)
_tab:
        push eax
        mov eax, 0x9
        call _print_char
        pop eax
        ret

;*(print a chunk of memory)
;* input: 
;*      eax: offset location
;*      ebx: size (bytes)
_dump_mem:
        pusha
        mov ecx, 0

        and eax, 0xFFFFFFF0

        jmp .skip
.looper:
        call _newline
.skip:
        ;*print address
        push eax
        call _print_hex
        call _tab
        pop eax
        ;*print address

        push ecx        ; store
        mov ecx, 0      ; init j
.looper2:

        push eax
        mov eax, [eax]
        call _print_byte
        call _space
        pop eax
        inc eax         ; increment address
        inc ecx         ; increment j
        cmp ecx, 0x10   ; while (j < 16)
        jl .looper2

        pop ecx         ; restore

        add ecx, 0x10   ; 16 bytes

        cmp ecx, ebx    ; while (i < x)
        jl .looper

.exit:
        call _newline
        popa
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
_num_hex:
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

;* (converts string to an int)
;* input:
;*      eax: null terminated string
_string_to_int:
        push ebx
        mov ebx, [eax]
        cmp ebx, 0x2D
        je .negative

.negative:


.leave:
        pop ebx
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

        call _num_hex

        add ebx, eax          ; increment by _num_hex

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

;* (read from a file)
;* input:
;*      eax: file name
;*      ebx: buffer
;*      ecx: size
_read_file:

        push ecx       
        push ebx
        push eax
        
        mov eax, 5     ; open
        pop ebx        ; file name
        mov ecx, 0     ; read-only mode
        int 0x80     

        mov ebx, eax   ;fd
        mov eax, 3     ; read
        pop ecx        ; buffer
        pop edx        ; buffer size
        int  0x80        


        ret
        