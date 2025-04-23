includelib msvcrt.lib
includelib kernel32.lib

extrn __imp_VirtualAlloc:QWORD
extrn __imp_VirtualFree:QWORD
extrn __imp_GetProcessHeap:QWORD
extrn __imp_HeapAlloc:QWORD
extrn __imp_HeapFree:QWORD

extern memcpy: proc
extern ExitProcess: proc

.data
    lastResult dw 256 dup(0)  ; Буфер для хранения последнего результата (Unicode)
    tempBuffer dw 256 dup(0)  ; Временный буфер
    
; Прототипы функций
ProcessArrayInt PROTO :QWORD    ; вход: указатель на wchar_t строку
ProcessArrayByte PROTO :QWORD   ; вход: указатель на wchar_t строку
ProcessMatrixInt PROTO :QWORD   ; вход: указатель на wchar_t строку
ProcessMatrixByte PROTO :QWORD  ; вход: указатель на wchar_t строку


.code 

StringToInt proc uses rsi rdx, strPtr:QWORD
    xor eax, eax
    xor edx, edx
    mov rsi, rcx

convert_loop:
    mov dl, byte ptr [rsi]
    test dl, dl
    jz conversion_done

    cmp dl, '0'
    jb invalid_char
    cmp dl, '9'
    ja invalid_char

    sub dl, '0'
    imul eax, 10
    add eax, edx
    
    inc rsi
    jmp convert_loop

invalid_char:
    inc rsi
    jmp convert_loop

conversion_done:
    ret
StringToInt endp

; Разбиение строки по разделителю (аналог splitString)
SplitString proc uses rbx rsi rdi r12 r13 r14, strPtr:QWORD, delimiter:BYTE
    mov rsi, rcx        ; RSI = исходная строка
    mov r12b, dl        ; R12b = символ-разделитель
    xor r13, r13        ; R13 = счетчик подстрок

count_loop:
    mov al, byte ptr [rsi]
    test al, al
    jz count_done
    
    cmp al, r12b
    je found_delimiter
    
    inc rsi
    jmp count_loop

found_delimiter:
    inc r13
    inc rsi
    jmp count_loop

count_done:
    inc r13

    ; Выделяем память для массива указателей
    mov rcx, r13
    inc rcx
    shl rcx, 3
    call __imp_VirtualAlloc
    mov r14, rax        ; R14 = массив указателей
    xor r15, r15        ; R15 = индекс в массиве

    ; Второй проход: копируем подстроки
    mov rsi, [rsp+40]   ; Восстанавливаем исходный указатель
    mov rdi, rsi

copy_loop:
    mov al, byte ptr [rsi]
    test al, al
    jz copy_last
    
    cmp al, r12b
    je split_substring
    
    inc rsi
    jmp copy_loop

split_substring:
    mov rcx, rsi
    sub rcx, rdi
    
    push rsi
    push rdi
    mov rdx, rcx
    inc rdx
    call __imp_VirtualAlloc
    pop rdi
    pop rsi
    
    mov rbx, rax
    mov rcx, rsi
    sub rcx, rdi
    mov rdx, rdi
    mov r8, rbx
    call memcpy
    
    mov byte ptr [rbx+rcx], 0
    mov [r14+r15*8], rbx
    inc r15
    
    inc rsi
    mov rdi, rsi
    jmp copy_loop

copy_last:
    mov rcx, rsi
    sub rcx, rdi
    jz empty_last
    
    push rsi
    mov rdx, rcx
    inc rdx
    call __imp_VirtualAlloc
    pop rsi
    
    mov rbx, rax
    mov rcx, rsi
    sub rcx, rdi
    mov rdx, rdi
    mov r8, rbx
    call memcpy
    
    mov byte ptr [rbx+rcx], 0
    mov [r14+r15*8], rbx
    jmp finish_split

empty_last:
    mov qword ptr [r14+r15*8], 0

finish_split:
    mov qword ptr [r14+r15*8+8], 0
    mov rax, r14
    ret
SplitString endp

FindMaxElement proc uses rsi rbx, arrayPtr:QWORD, count:QWORD
    mov rsi, rcx
    mov rcx, rdx
    test rcx, rcx
    jz findmax_error
    
    mov eax, [rsi]
    dec rcx
    jz findmax_done
    
    add rsi, 4
    
findmax_search_loop:
    mov ebx, [rsi]
    cmp ebx, eax
    jle findmax_next
    mov eax, ebx
    
findmax_next:
    add rsi, 4
    loop findmax_search_loop
    
findmax_done:
    ret
    
findmax_error:
    xor eax, eax
    ret
FindMaxElement endp

SwapAroundMax proc uses rsi rdi r12, arrayPtr:QWORD, count:QWORD, maxIndex:QWORD
    cmp r8, rdx
    jae swap_invalid_params
    
    mov rsi, rcx
    lea rdi, [rcx + r8*4 - 4]

swap_left_reverse:
    cmp rsi, rdi
    jae swap_right_part
    
    mov eax, [rsi]
    mov r9d, [rdi]
    mov [rsi], r9d
    mov [rdi], eax
    
    add rsi, 4
    sub rdi, 4
    jmp swap_left_reverse
    
swap_right_part:
    lea rsi, [rcx + r8*4 + 4]
    lea rdi, [rcx + rdx*4 - 4]
    
swap_right_reverse:
    cmp rsi, rdi
    jae swap_finish
    
    mov eax, [rsi]
    mov r9d, [rdi]
    mov [rsi], r9d
    mov [rdi], eax
    
    add rsi, 4
    sub rdi, 4
    jmp swap_right_reverse
    
swap_finish:
    ret
    
swap_invalid_params:
    xor eax, eax
    ret
SwapAroundMax endp

IntToString proc uses rbx rdi, value:QWORD, strPtr:QWORD
    mov rdi, rdx
    mov rax, rcx
    mov r10, 10
    
    test rax, rax
    jns int_positive
    neg rax
    mov byte ptr [rdi], '-'
    inc rdi
    
int_positive:
    lea r8, [rdi + 32]
    mov r9, r8
    
int_convert_loop:
    xor rdx, rdx
    div r10
    add dl, '0'
    dec r8
    mov [r8], dl
    test rax, rax
    jnz int_convert_loop
    
    mov rcx, r9
    sub rcx, r8
    
int_copy_loop:
    mov al, [r8]
    mov [rdi], al
    inc r8
    inc rdi
    loop int_copy_loop
    
    mov byte ptr [rdi], 0
    ret
IntToString endp

ProcessArrayInt proc uses rbx rsi rdi r12 r13, inputPtr:QWORD
    sub rsp, 32
    mov rsi, rcx
    
    ; Шаг 1: Разделить строки
    mov rcx, rsi
    mov dl, ','
    call SplitString
    mov r12, rax

    xor r13, r13
array_count_loop:
    mov rax, [r12 + r13*8]
    test rax, rax
    jz array_count_done
    inc r13
    jmp array_count_loop
array_count_done:

    ; Выделить память под массив чисел
    mov rcx, r13
    shl rcx, 2
    call __imp_VirtualAlloc
    mov rbx, rax

    ; Строки в числа
    xor rdi, rdi
array_convert_loop:
    cmp rdi, r13
    jge array_convert_done
    
    mov rcx, [r12 + rdi*8]
    call StringToInt
    mov [rbx + rdi*4], eax
    inc rdi
    jmp array_convert_loop
array_convert_done:

    ; Находим максимальный элемент
    mov rcx, rbx
    mov rdx, r13
    call FindMaxElement
    mov r9, rax

    ; Находим индекс максимального элемента
    xor r8, r8
array_find_max_idx:
    cmp [rbx + r8*4], r9d
    je array_max_found
    inc r8
    cmp r8, r13
    jl array_find_max_idx
array_max_found:

    ; Переворачиваем части массива
    mov rcx, rbx
    mov rdx, r13
    call SwapAroundMax

    ; Конвертируем обратно в строку
    lea rdi, lastResult
    xor r10, r10
array_build_string:
    cmp r10, r13
    jge array_string_done
    
    mov ecx, [rbx + r10*4]
    mov rdx, rdi
    call IntToString

    mov rax, rdi
array_find_end:
    cmp byte ptr [rax], 0
    je array_end_found
    inc rax
    jmp array_find_end
array_end_found:
    mov rdi, rax

    cmp r10, r13
    je array_no_comma
    mov byte ptr [rdi], ','
    inc rdi
array_no_comma:
    inc r10
    jmp array_build_string
array_string_done:

    ; Освобождаем память
    mov rcx, rbx
    call __imp_VirtualFree
    mov rcx, r12
    call __imp_VirtualFree

    lea rax, lastResult
    add rsp, 32
    ret
ProcessArrayInt endp

; Helper function to find max byte in array
FindMaxByte proc uses rsi, arrayPtr:QWORD, count:QWORD
    mov rsi, rcx        ; RSI = array pointer
    mov rcx, rdx        ; RCX = count
    test rcx, rcx
    jz empty_array
    
    xor eax, eax
    mov al, [rsi]       ; AL = first byte (max)
    dec rcx
    jz done
    
    add rsi, 1          ; Move to next byte
    
search_loop:
    mov dl, [rsi]
    cmp dl, al
    jbe next_byte
    mov al, dl          ; New max found
    
next_byte:
    add rsi, 1
    loop search_loop
    
done:
    ret
    
empty_array:
    xor eax, eax
    ret
FindMaxByte endp

ProcessArrayByte proc uses rbx rsi rdi r12 r13, inputPtr:QWORD
    sub rsp, 32
    mov rsi, rcx        ; RSI = input string pointer
    
    ; Step 1: Split string by commas
    mov rcx, rsi        ; RCX = input string
    mov dl, ','         ; DL = delimiter
    call SplitString
    mov r12, rax        ; R12 = array of string pointers
    test r12, r12
    jz pa_error         ; If splitting failed
    
    ; Count number of elements
    xor r13, r13        ; R13 = element count
count_loop:
    mov rax, [r12 + r13*8]
    test rax, rax
    jz count_done
    inc r13
    jmp count_loop
count_done:
    
    ; Allocate memory for byte array
    mov rcx, r13        ; Number of bytes needed
    call __imp_VirtualAlloc
    mov rbx, rax        ; RBX = byte array
    test rbx, rbx
    jz pa_error
    
    ; Convert strings to bytes
    xor rdi, rdi        ; RDI = index
convert_loop:
    cmp rdi, r13
    jge convert_done
    
    mov rcx, [r12 + rdi*8]  ; Current string
    call StringToInt         ; Convert to number (returns in EAX)
    
    ; Store as byte (with range check)
    cmp eax, 0FFh
    ja value_too_large
    cmp eax, 0
    jl value_too_small
    
    mov [rbx + rdi], al ; Store byte
    inc rdi
    jmp convert_loop
    
value_too_large:
    mov eax, 0FFh       ; Clamp to 255
    mov [rbx + rdi], al
    inc rdi
    jmp convert_loop
    
value_too_small:
    xor eax, eax        ; Clamp to 0
    mov [rbx + rdi], al
    inc rdi
    jmp convert_loop
    
convert_done:
    
    ; Find max byte value
    mov rcx, rbx        ; Array pointer
    mov rdx, r13        ; Count
    call FindMaxByte
    mov r9d, eax        ; R9D = max value
    
    ; Find index of max value
    xor r8, r8          ; R8 = index
find_max_idx:
    cmp r8, r13
    jge max_found
    mov al, [rbx + r8]
    cmp al, r9b
    je max_found
    inc r8
    jmp find_max_idx
max_found:
    
    ; Swap elements around max (same as for integers)
    mov rcx, rbx        ; Array pointer
    mov rdx, r13        ; Count
    ; R8 already contains max index
    call SwapAroundMax
    
    ; Convert back to string
    lea rdi, lastResult
    xor r10, r10        ; Index
string_loop:
    cmp r10, r13
    jge string_done
    
    ; Convert byte to string
    xor eax, eax
    mov al, [rbx + r10]
    mov rcx, rax
    mov rdx, rdi
    call IntToString
    
    ; Find string end
    mov rax, rdi
find_end:
    cmp byte ptr [rax], 0
    je end_found
    inc rax
    jmp find_end
end_found:
    mov rdi, rax
    
    ; Add comma if not last element
    cmp r10, r13
    je no_comma
    mov byte ptr [rdi], ','
    inc rdi
no_comma:
    inc r10
    jmp string_loop
    
string_done:
    ; Clean up
    mov rcx, rbx
    call __imp_VirtualFree
    
    mov rcx, r12
    call __imp_VirtualFree
    
    lea rax, lastResult
    add rsp, 32
    ret
    
pa_error:
    xor eax, eax
    add rsp, 32
    ret
ProcessArrayByte endp

ProcessMatrixInt proc uses rbx rsi rdi r12 r13 r14 r15, inputPtr:QWORD
    sub rsp, 32

    ; Step 1: Split into rows (by ';')
    mov rsi, rcx                ; RSI = input string
    mov rcx, rsi
    mov dl, ';'                 ; DL = row delimiter
    call SplitString
    mov r12, rax                ; R12 = array of row strings
    test r12, r12
    jz pm_error                 ; If splitting failed

    ; Count number of rows
    xor r13, r13                ; R13 = row count

count_rows:
    mov rax, [r12 + r13*8]
    test rax, rax
    jz rows_counted
    inc r13
    jmp count_rows

rows_counted:
    test r13, r13
    jz pm_error                 ; No rows found

    ; Step 2: Process first row to determine column count
    mov rcx, [r12]              ; First row string
    mov dl, ','                 ; DL = column delimiter
    call SplitString
    mov r14, rax                ; R14 = array of first row elements
    test r14, r14
    jz pm_error

    ; Count columns in first row
    xor r15, r15                ; R15 = column count

count_first_row_cols:
    mov rax, [r14 + r15*8]
    test rax, rax
    jz cols_counted
    inc r15
    jmp count_first_row_cols

cols_counted:
    test r15, r15
    jz pm_error                 ; No columns found

    ; Allocate matrix (r13 rows × r15 columns)
    mov rax, r13
    mul r15                     ; RAX = total elements
    shl rax, 2                  ; Multiply by 4 (sizeof DWORD)
    mov rcx, rax
    call __imp_VirtualAlloc
    mov rbx, rax                ; RBX = matrix data
    test rbx, rbx
    jz pm_error

    ; Step 3: Parse all rows into matrix
    xor rdi, rdi                ; RDI = row index

parse_rows:
    cmp rdi, r13
    jge rows_parsed

    ; Split current row into elements
    mov rcx, [r12 + rdi*8]      ; Current row string
    mov dl, ','
    call SplitString
    test rax, rax
    jz parse_error

    ; Verify column count matches
    xor r8, r8                  ; Column counter

verify_cols:
    mov r9, [rax + r8*8]
    test r9, r9
    jz verify_done
    inc r8
    jmp verify_cols

verify_done:
    cmp r8, r15
    jne parse_error             ; Column count mismatch

    ; Parse elements in current row
    xor r9, r9                  ; Column index

parse_cols:
    cmp r9, r15
    jge cols_parsed

    ; Calculate linear index: (row * columns) + col
    mov rax, rdi
    mul r15
    add rax, r9                 ; RAX = linear index

    ; Convert element to integer
    mov rcx, [rax + r9*8]       ; Current element string
    call StringToInt

    ; Store in matrix
    mov [rbx + rax*4], eax

    inc r9
    jmp parse_cols

cols_parsed:
    mov rcx, rax
    call __imp_VirtualFree

    inc rdi
    jmp parse_rows

parse_error:
    mov rcx, rbx
    call __imp_VirtualFree
    jmp pm_error

rows_parsed:
    ; Step 4: Find max element and its position
    mov rcx, rbx                ; Matrix data
    mov rdx, r13                ; Rows
    mov r8, r15                 ; Columns
    call FindMatrixMax
    mov r9, rax                 ; R9 = max value
    mov r10, rdx                ; R10 = max row index
    mov r11, r8                 ; R11 = max col index

    ; Step 5: Process matrix (example: swap quadrants around max)
    mov rcx, rbx                ; Matrix data
    mov rdx, r13                ; Rows
    mov r8, r15                 ; Columns
    mov r9, r10                 ; Max row
    mov r10, r11                ; Max column
    call ProcessMatrixAroundMax

    ; Step 6: Convert matrix back to string
    lea rdi, lastResult
    xor rsi, rsi                ; Row index

matrix_to_string_rows:
    cmp rsi, r13
    jge matrix_string_done

    xor r14, r14                ; Column index

matrix_to_string_cols:
    cmp r14, r15
    jge row_done

    ; Calculate linear index
    mov rax, rsi
    mul r15
    add rax, r14

    ; Convert element to string
    mov ecx, [rbx + rax*4]
    mov rdx, rdi
    call IntToString

    ; Find string end
    mov rax, rdi

find_string_end:
    cmp byte ptr [rax], 0
    je string_end_found
    inc rax
    jmp find_string_end

string_end_found:
    mov rdi, rax

    ; Add column separator (except last column)
    cmp r14, r15
    je no_col_separator
    mov byte ptr [rdi], ','
    inc rdi
no_col_separator:
    inc r14
    jmp matrix_to_string_cols

row_done:
    ; Add row separator (except last row)
    cmp rsi, r13
    je no_row_separator
    mov byte ptr [rdi], ','
    inc rdi
    no_row_separator:
    inc rsi
    jmp matrix_to_string_rows

matrix_string_done:
    ; Clean up
    mov rcx, rbx
    call __imp_VirtualFree

    mov rcx, r12
    call __imp_VirtualFree

    lea rax, lastResult
    add rsp, 32
    ret

pm_error:
    xor eax, eax
    add rsp, 32
    ret

ProcessMatrixInt endp

end