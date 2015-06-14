[GLOBAL default_exception_handler]
[GLOBAL default_interrupt_handler]
[GLOBAL exception_handlers]
[GLOBAL pit_interrupt]
[GLOBAL spurious_interrupt]

[EXTERN g_pitTicks]
[EXTERN g_localApicAddr]
[EXTERN except_handler]

; -------------------------------------------------------------------------------------------------
; Default handlers

default_exception_handler:
        jmp $

default_interrupt_handler:
        iretd

; -------------------------------------------------------------------------------------------------
; Specific exception handlers

%macro make_exception_handler 1
exception%1_handler:
        cli
        push byte 0
        push byte %1
        jmp exception_body
%endmacro

%macro make_error_exception_handler 1
exception%1_handler:
        cli
        push byte %1
        jmp exception_body
%endmacro

make_exception_handler 0
make_exception_handler 1
make_exception_handler 2
make_exception_handler 3
make_exception_handler 4
make_exception_handler 5
make_exception_handler 6
make_exception_handler 7
make_error_exception_handler 8
make_exception_handler 9
make_error_exception_handler 10
make_error_exception_handler 11
make_error_exception_handler 12
make_error_exception_handler 13
make_error_exception_handler 14
make_exception_handler 15
make_exception_handler 16
make_error_exception_handler 17
make_exception_handler 18
make_exception_handler 19

exception_handlers:
        dd exception0_handler
        dd exception1_handler
        dd exception2_handler
        dd exception3_handler
        dd exception4_handler
        dd exception5_handler
        dd exception6_handler
        dd exception7_handler
        dd exception8_handler
        dd exception9_handler
        dd exception10_handler
        dd exception11_handler
        dd exception12_handler
        dd exception13_handler
        dd exception14_handler
        dd exception15_handler
        dd exception16_handler
        dd exception17_handler
        dd exception18_handler
        dd exception19_handler

exception_body:
        push eax
        push ecx
        push edx
        push ebx
        push ebp
        push esi
        push edi

        call except_handler

        jmp $

; -------------------------------------------------------------------------------------------------
; Prints a string to the screen
; in: rdi = screen address
;     rsi = string address
vga_print:
        lodsb
        cmp al, 0
        je .done
        stosb
        mov al, 0x1f
        stosb
        jmp vga_print
.done:
        ret

; -------------------------------------------------------------------------------------------------
; PIT interrupt
pit_interrupt:
        push eax
        push edi

        mov eax, dword [g_pitTicks]
        inc eax
        mov dword [g_pitTicks], eax

        ; Acknowledge interrupt
        mov edi, [g_localApicAddr]
        add edi, 0xb0
        xor eax, eax
        stosd

        pop edi
        pop eax
        iretd

; -------------------------------------------------------------------------------------------------
; Spurious interrupt
spurious_interrupt:
        iretd