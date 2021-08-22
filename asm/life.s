global _start
%include "lib.s"

section .data
;* play area
%define WIDTH 80
%define HEIGHT 32
;* full board
%define BOARD_WIDTH 83
%define BOARD_HEIGHT 34
%define BOARD_SIZE 2822

section .bss
buf             resb 0x9
board_buffer    resb 2832   ; size of board for read
board_cpy       resb 250    ; a copy of the current row, above and below 
section .text

_start:
        pop   eax               ; argc
        pop   eax               ; argv[0]
        pop   eax               ; filename

        ;* read file
        mov ebx, board_buffer
        mov ecx, BOARD_SIZE
        call _read_file

        ;* print board
        mov eax, board_buffer
        call _print_string
        call _newline

        ;* print memory
        mov eax, board_buffer
        mov ebx, BOARD_SIZE
        call _dump_mem
        call _newline

        ;* test copy
        mov eax, board_buffer
        mov ebx, board_cpy
        call copy_row 

        ; add eax, BOARD_WIDTH
        ; add ebx, BOARD_WIDTH
        ; call copy_row 

        ; add eax, BOARD_WIDTH
        ; add ebx, BOARD_WIDTH
        ; call copy_row 

        ; mov eax, board_cpy
        ; mov ebx, BOARD_WIDTH
        ; add ebx, BOARD_WIDTH
        ; add ebx, BOARD_WIDTH
        ; call _dump_mem
        ;* test copy

        mov eax, 0x1    ; end 
        xor ebx, ebx
        int 0x80

;* (copies a row on the board)
;* input:
;*      eax: source
;*      ebx: destination
copy_row:
        push eax
        push ebx
        push ecx
        push edx
        mov ecx, BOARD_WIDTH

.looper:
        mov edx, [eax]
        
.looper2:
        mov [ebx], DL
        shr edx, 8      ; shift 8 bits
        inc ebx         ; next address
        dec ecx
        cmp ecx, 0
        je .return

        cmp edx, 0
        jg .looper2

        add eax, 4
        cmp ecx, 0
        jg .looper

.return:
        pop edx
        pop ecx
        pop ebx
        pop eax
        ret
