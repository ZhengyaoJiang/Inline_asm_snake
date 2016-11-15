// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"
#include "conio.h"

int main()
{
    char message[] = "hello world";
    char board[16][16] = {0};
    // cordinates of body of the snake
    char x_cordinates[1000] = {5, 6, 7, 8, 9, 10 };
    char y_cordinates[1000] = {5, 5, 5, 5, 5, 5 };
    int snake_length = 5;

    char fruit_x = 12;
    char fruit_y = 2;
    int has_eat = 0; // 0 means has not eaten

    char fruit[] = "¹û";
    char star[] = "Éß";
    char empty[] = "£¬";
    char entry[] = "\n";

    char cls[] = "cls";

    int direction = 0; //0 up, 1 down, 2 left, 4 right
    char row = 0;
    char colum = 0;
    

    __asm {
    start:
        call control
        call move
        call judge_eat
        call link_cordinates
        call judge_die
        call paint

        mov eax, 200
        push eax
        call Sleep

        lea eax, cls
        push eax
        call system
        add esp, 4
        jmp start

    judge_eat:
        mov al, x_cordinates
        cmp al, fruit_x
        jne end_judge_eat
        mov al, y_cordinates
        cmp al, fruit_y
        jne end_judge_eat

        ;eat fruit
        lea eax, x_cordinates
        add eax, snake_length
        inc eax
        mov ebx, eax
        dec ebx
        mov ebx, [ebx-1]
        mov [eax], ebx
        lea eax, y_cordinates
        add eax, snake_length
        inc eax
        mov ebx, eax
        dec ebx
        mov ebx, [ebx-1]
        mov [eax], ebx
        mov has_eat, 1

        call rand
        mov ecx, 15
        div ecx
        mov eax, edx
        mov fruit_x, al
        call rand
        mov ecx, 15
        div ecx
        mov eax, edx
        mov fruit_y, al

    end_judge_eat:
        ret

    judge_die:
        cmp x_cordinates, 0
        jl end
        cmp x_cordinates, 15
        jg end
        cmp y_cordinates, 0
        jl end
        cmp y_cordinates, 15
        jg end
        mov ecx, 0
    start_judge_body:
        inc ecx
        cmp ecx, snake_length
        jg end_judge_body

        lea eax, x_cordinates
        add eax, ecx
        mov al, [eax]
        cmp al, x_cordinates
        jne start_judge_body
        lea eax, y_cordinates
        add eax, ecx
        mov al, [eax]
        cmp al, y_cordinates
        jne start_judge_body
        jmp end
    end_judge_body:
        ret

    control:
        call _kbhit
        cmp eax, 0
        je end_control
        call _getch
        cmp eax, 'w'
        mov direction, 0
        je end_control
        cmp eax, 's'
        mov direction, 1
        je end_control
        cmp eax, 'a'
        mov direction, 2
        je end_control
        cmp eax, 'd'
        mov direction, 3
        je end_control

    end_control:
        ret
    
    link_cordinates:
        mov ecx, 0; ecx is the time of loop
        mov row, 0
        mov colum, 0
        jmp cordinates_loop
    end_link:
        ret

    cordinates_loop:
        lea eax, x_cordinates
        add eax, ecx
        mov al, [eax]
        mov row, al
        lea eax, y_cordinates
        add eax, ecx
        mov al, [eax]
        mov colum, al

        cmp ecx, snake_length
        jg end_link

        movzx eax, colum
        mov ebx, 16
        mul ebx
        movzx ebx, row 
        add eax, ebx
        lea ebx, board
        add eax, ebx
        mov [eax], 1
        inc ecx
        jmp cordinates_loop
     
    move:
        call move_snake
        ; call move_point
        ret

    move_snake:
        mov ecx, snake_length
        jmp start_move_body
    start_move_body:
        cmp ecx, 0
        je end_move_body
        lea eax, x_cordinates
        add eax, ecx
        mov ebx, eax
        dec eax
        mov al, [eax]
        mov [ebx], al
        lea eax, y_cordinates
        add eax, ecx
        mov ebx, eax
        dec eax
        mov al, [eax]
        mov [ebx], al
        dec ecx
        jmp start_move_body 
    end_move_body:

        cmp direction, 0
        je up
        cmp direction, 1
        je down
        cmp direction, 2
        je left
        cmp direction, 3
        je right
    up:
        lea eax, y_cordinates
        dec [eax]
        jmp end_move_head
    down:
        lea eax, y_cordinates
        inc [eax]
        jmp end_move_head
    left:
        lea eax, x_cordinates
        dec [eax]
        jmp end_move_head
    right:
        lea eax, x_cordinates
        inc [eax]
        jmp end_move_head
    end_move_head:
        cmp has_eat, 0
        je end_move
        inc snake_length
        mov has_eat, 0
    end_move:
        ret
    
    move_up:
        ret

    paint:
        mov row, 0
        mov colum, 0
        call main_paint_loop
        ret

    main_paint_loop:
        cmp row, 16
        jl print_main
        call print_entry
        mov row, 0
        inc colum
        cmp colum, 16
        jl main_paint_loop
        ret

    print_main:
        movzx eax, colum
        mov ebx, 16
        mul ebx
        movzx ebx, row
        add eax, ebx
        lea ebx, board
        add eax, ebx
        inc row
        cmp [eax], 1
        je print_star

        mov al, fruit_y
        cmp al, colum
        jne print_empty
        mov al, fruit_x
        inc al
        cmp al, row
        jne print_empty
        jmp print_fruit

    print_fruit:
        lea eax, fruit
        push eax
        call printf
        add esp, 4
        jmp main_paint_loop
        
    print_star:
        mov [eax], 0
        lea eax, star
        push eax
        call printf
        add esp, 4
        jmp main_paint_loop
    
    print_empty:
        lea eax, empty
        push eax
        call printf
        add esp, 4
        jmp main_paint_loop


    print_entry:
        lea eax, entry
        push eax
        call printf
        add esp, 4
        ret
    end:
    }

    char a;
    scanf_s("%c", &a);
    return 0;
}

