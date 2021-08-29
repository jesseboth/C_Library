;*RULES*;
;* Any live cell with fewer than two live neighbours dies, as if by underpopulation.
;* Any live cell with two or three live neighbours lives on to the next generation.
;* Any live cell with more than three live neighbours dies, as if by overpopulation.
;* Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

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
%define NEXT_LINE 2
%define DEAD 0x20
%define LIVE 0x58   

section .bss
buf             resb 0x9        ; align
board_buffer    resb 2832       ; size of board for read

board_cpy       resb 0          ; start of space place holder
board_cpy_above resb 83         ; above current
board_cpy_cur   resb 83         ; current
board_cpy_below resb 83         ; below current

current         resb 8          ; address
cur_sub         resb 8
neighbors        resb 8          ; number of neighbors
cur_x           resb 8
cur_y           resb 8

section .text

;* 2 inputs - filename, # of iterations
_start:
        pop eax               ; argc
        pop eax               ; argv[0]
        pop eax               ; filename

        ;* read file
        mov ebx, board_buffer
        mov ecx, BOARD_SIZE
        call _read_file

        pop eax                 ; iterations
        call _string_to_int

        xor ebx, ebx            ; zero out
.again:
        push eax
        push ebx

        ;* print board
        mov eax, board_buffer
        call _print_string
        call _newline

        call life

        xor eax, eax            ; 0 seconds
        mov ebx, 500            ;.5 seconds
        call _sleep

        pop ebx
        pop eax

        inc ebx
        cmp ebx, eax
        je .done
        
        jmp .again

.done:
        mov eax, 0x1    ; end 
        xor ebx, ebx
        int 0x80
        ret

;* (determine new generation)
;* input:
;*      none
life:
        mov eax, board_buffer+BOARD_WIDTH

        mov ebx, board_cpy_cur          ; copy top row
        call copy_row
        
        add eax, BOARD_WIDTH            ; copy 2nd line
        add ebx, BOARD_WIDTH
        call copy_row

        mov eax, board_buffer+BOARD_WIDTH
        inc eax                         ; current cell

        mov [current], eax

        mov eax, board_cpy_cur+1        ; first playable char
        mov [cur_sub], eax              ; sub board current address
        
        mov word [cur_x], 0x0
        mov word [cur_y], 0x0

.loop:
        mov eax, [cur_sub]              ; store address
        mov word [neighbors], 0x0        ; zero out

.up_left:
        sub eax, BOARD_WIDTH    ; x o o       
        dec eax                 ; o @ o
        call check_pos          ; o o o
.up:
        inc eax                 ; o x o
        call check_pos          ; o @ o
                                ; o o o
.up_right:                      
        inc eax                 ; o o x 
        call check_pos          ; o @ o
                                ; o o o 
.right:                         
        add eax, BOARD_WIDTH    ; o o o 
        call check_pos          ; o @ x
                                ; o o o 
.left:
        sub eax, 2              ; o o o 
        call check_pos          ; x @ o
                                ; o o o 
.down_left:
        add eax, BOARD_WIDTH    ; o o o
        call check_pos          ; o @ o
                                ; x o o 
.down:                          
        inc eax                 ; o o o 
        call check_pos          ; o @ o 
                                ; o x o 
.down_right:    
        inc eax                 ; o o o 
        call check_pos          ; o @ o
                                ; o o x
        xor eax, eax
        mov ecx, [cur_sub]      ; determine if dead or alive
        mov AL, [ecx]

        mov ecx, [neighbors]

        cmp eax, DEAD
        je .birth_check

.kill_check:

        cmp ecx, 0x2      ; check minimum
        jl .kill

        cmp ecx, 0x3
        jg .kill

        jmp .live

.birth_check:

        cmp ecx, 3
        je .birth

        jmp .finish_iter
.kill:

        xor ebx, ebx
        mov ecx, [current]      ; current char
        mov ebx, DEAD
        mov [ecx], BL

        jmp .finish_iter
.birth:
        xor ebx, ebx
        mov ecx, [current]      ; current char
        mov ebx, LIVE
        mov [ecx], BL

.live:
.finish_iter:
        mov ebx, [cur_x]
        inc ebx
        mov [cur_x], ebx

        mov ebx, [current]
        inc ebx
        mov [current], ebx

        mov ebx, [cur_sub]
        inc ebx
        mov [cur_sub], ebx

        mov ecx, [cur_x]
        cmp ecx, WIDTH
        jl .loop

.next_line:
        mov byte [cur_x], 0x0           ; row start

        mov ebx, [cur_y]                ; increment y
        inc ebx
        mov [cur_y], ebx

        mov ebx, [current]              ; mov current to next line
        add ebx, 3
        mov [current], ebx

        mov eax, board_cpy_cur          ; shift copies up
        mov ebx, board_cpy_above
        call copy_row

        mov eax, board_cpy_below        ; shift
        mov ebx, board_cpy_cur
        call copy_row

        mov eax, [current]              ; copy next line on board
        add eax, BOARD_WIDTH
        dec eax
        mov ebx, board_cpy_below
        call copy_row

        mov ecx, board_cpy_cur+1          ; get pos
        mov [cur_sub], ecx                ; store address

        mov ecx, [cur_y]
        cmp ecx, HEIGHT
        jl .loop

        ret

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
        je .leave

        cmp edx, 0
        jg .looper2

        add eax, 4
        cmp ecx, 0
        jg .looper

.leave:
        pop edx
        pop ecx
        pop ebx
        pop eax
        ret

;* (checks if cell is dead or alive)
;* input:
;*      eax: cell address
check_pos:
        push eax
        push ebx
        xor ebx, ebx
        mov BL, byte [eax]

        cmp ebx, LIVE
        jne .leave

        mov eax, [neighbors]
        inc eax
        mov [neighbors], eax

.leave:
        pop ebx
        pop eax
        ret

;* (prints a line from the board)
;* input:
;*      eax: line address
print_line:
        pusha
        mov ecx, eax           ; store

        mov edx, BOARD_WIDTH   ; length
        mov eax, 0x4           ; write syscall
        mov ebx, 1             ; fd

        int 0x80               ; interrupt -> syscall
        popa
        ret
