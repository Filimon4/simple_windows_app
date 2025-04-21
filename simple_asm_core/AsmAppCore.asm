includelib kernel32.lib
includelib msvcrt.lib

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
ProcessArrayInt PROTO :DWORD    ; вход: указатель на wchar_t строку
ProcessArrayByte PROTO :DWORD   ; вход: указатель на wchar_t строку
ProcessMatrixInt PROTO :DWORD   ; вход: указатель на wchar_t строку
ProcessMatrixByte PROTO :DWORD  ; вход: указатель на wchar_t строку

.code

; Преобразование строки в число (аналог std::stoi)
StringToInt proc strPtr:DWORD
    push rsi
    push rdx
       
    xor eax, eax
    xor edx, edx

    mov rsi, rcx

convert_loop:
    mov dl, byte ptr [rsi]
    test dl, dl ; zero flat если итог 0, производит побитовый И, проверяет конце строки "\0"
    jz conversion_done

    cmp dl, '0'
    jb invalid_char
    cmp dl, '9'
    ja invalid_char

    sub dl, '0' ; ACII символы. Если вычивает строку 0, то получаем число
    imul eax, 10 ; увеличиваем разрядность, для следующего числа
    add eax, edx
    
    inc rsi

    jmp convert_loop

invalid_char:
    inc rsi
    jmp convert_loop

conversion_done:
    pop rdx
    pop rsi
    ret
StringToInt endp

; Разбиение строки по разделителю (аналог splitString)
SplitString proc
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14

    ; Сохраняем параметры
    mov rsi, rcx        ; RSI = исходная строка
    mov r12, rdx        ; R12 = символ-разделитель
    xor r13, r13        ; R13 = счетчик подстрок

    ; Первый проход: подсчитываем количество подстрок
count_loop:
    mov al, byte ptr [rsi]
    test al, al
    jz count_done       ; Конец строки
    
    cmp al, r12b        ; Сравниваем с разделителем
    je found_delimiter
    
    inc rsi
    jmp count_loop

found_delimiter:
    inc r13             ; Увеличиваем счетчик подстрок
    inc rsi             ; Пропускаем разделитель
    jmp count_loop

count_done:
    inc r13             ; Учитываем последнюю подстроку

    ; Выделяем память для массива указателей (8 байт на элемент)
    mov rcx, r13
    inc rcx             ; +1 для NULL в конце
    shl rcx, 3          ; Умножаем на 8 (размер указателя)
    call __imp_VirtualAlloc
    mov r14, rax        ; R14 = массив указателей
    xor r15, r15        ; R15 = индекс в массиве

    ; Второй проход: копируем подстроки
    mov rsi, [rsp+40]   ; Восстанавливаем исходный указатель
    mov rdi, rsi        ; RDI = начало текущей подстроки

copy_loop:
    mov al, byte ptr [rsi]
    test al, al
    jz copy_last        ; Конец строки - обрабатываем последнюю подстроку
    
    cmp al, r12b        ; Сравниваем с разделителем
    je split_substring
    
    inc rsi
    jmp copy_loop

split_substring:
    ; Вычисляем длину подстроки
    mov rcx, rsi
    sub rcx, rdi        ; RCX = длина подстроки
    
    ; Выделяем память для подстроки (+1 для нуль-терминатора)
    push rsi
    push rdi
    mov rdx, rcx
    inc rdx
    call __imp_VirtualAlloc
    pop rdi
    pop rsi
    
    ; Копируем подстроку
    mov rbx, rax        ; RBX = буфер для подстроки
    mov rcx, rsi
    sub rcx, rdi        ; Длина
    mov rdx, rdi        ; Источник
    mov r8, rbx         ; Приемник
    call memcpy
    
    ; Добавляем нуль-терминатор
    mov byte ptr [rbx+rcx], 0
    
    ; Сохраняем указатель в массив
    mov [r14+r15*8], rbx
    inc r15
    
    ; Пропускаем разделитель и обновляем RDI
    inc rsi
    mov rdi, rsi
    jmp copy_loop

copy_last:
    ; Обрабатываем последнюю подстроку
    mov rcx, rsi
    sub rcx, rdi        ; Длина подстроки
    jz empty_last       ; Если длина 0 (строка заканчивается разделителем)
    
    ; Выделяем память
    push rsi
    mov rdx, rcx
    inc rdx
    call __imp_VirtualAlloc
    pop rsi
    
    ; Копируем
    mov rbx, rax
    mov rcx, rsi
    sub rcx, rdi
    mov rdx, rdi
    mov r8, rbx
    call memcpy
    
    ; Добавляем нуль-терминатор
    mov byte ptr [rbx+rcx], 0
    mov [r14+r15*8], rbx
    jmp finish_split

empty_last:
    mov qword ptr [r14+r15*8], 0

finish_split:
    ; Добавляем NULL в конец массива
    mov qword ptr [r14+r15*8+8], 0
    
    ; Возвращаем массив
    mov rax, r14
    
    pop r14
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    ret
SplitString endp

FindMaxElement proc arrayPtr:QWORD, count:QWORD
    push rsi          ; Сохраняем регистры
    push rbx
    
    mov rsi, rcx      ; RSI = указатель на массив
    mov rcx, rdx      ; RCX = количество элементов
    test rcx, rcx     ; Проверка на пустой массив
    jz .error
    
    mov eax, [rsi]    ; EAX = первый элемент (текущий максимум)
    dec rcx           ; Уменьшаем счётчик
    jz .done          ; Если только один элемент
    
    add rsi, 4        ; Переходим ко второму элементу
    
.search_loop:
    mov ebx, [rsi]    ; EBX = текущий элемент
    cmp ebx, eax      ; Сравниваем с текущим максимумом
    jle .next         ; Если меньше или равен - пропускаем
    mov eax, ebx      ; Иначе обновляем максимум
    
.next:
    add rsi, 4        ; Следующий элемент
    loop .search_loop ; Повторяем пока RCX > 0
    
.done:
    pop rbx           ; Восстанавливаем регистры
    pop rsi
    ret
    
.error:
    xor eax, eax      ; Возвращаем 0 если массив пуст
    pop rbx
    pop rsi
    ret
FindMaxElement endp

SwapAroundMax proc
    push rsi
    push rdi
    push r12
    
    ; Проверка на валидность индекса
    cmp r8, rdx
    jae invalid_params  ; Если maxIndex >= count
    
    ; Вычисляем границы для переворота
    mov rsi, rcx        ; RSI = начало массива
    lea rdi, [rcx + r8*4 - 4] ; RDI = элемент перед максимальным
    
    ; Переворачиваем левую часть (до максимального)
left_reverse:
    cmp rsi, rdi
    jae right_part      ; Если указатели встретились
    
    ; Обмен значений
    mov eax, [rsi]
    mov r9d, [rdi]
    mov [rsi], r9d
    mov [rdi], eax
    
    add rsi, 4
    sub rdi, 4
    jmp left_reverse
    
right_part:
    ; Теперь переворачиваем правую часть (после максимального)
    lea rsi, [rcx + r8*4 + 4] ; RSI = элемент после максимального
    lea rdi, [rcx + rdx*4 - 4] ; RDI = конец массива
    
right_reverse:
    cmp rsi, rdi
    jae finish
    
    ; Обмен значений
    mov eax, [rsi]
    mov r9d, [rdi]
    mov [rsi], r9d
    mov [rdi], eax
    
    add rsi, 4
    sub rdi, 4
    jmp right_reverse
    
finish:
    pop r12
    pop rdi
    pop rsi
    ret
    
invalid_params:
    xor eax, eax        ; Возвращаем 0 в случае ошибки
    pop r12
    pop rdi
    pop rsi
    ret
SwapAroundMax endp

; Преобразование 64-битного числа в строку (десятичное представление)
; Вход: RCX = число (value), RDX = указатель на буфер (strPtr)
; Выход: строка в буфере (нуль-терминированная)
; Разрушает: RAX, RCX, RDX, R8, R9, R10

IntToString proc
    push rbx
    push rdi
    
    mov rdi, rdx      ; RDI = указатель на буфер
    mov rax, rcx      ; RAX = число для преобразования
    mov r10, 10       ; Делитель (10 для десятичной системы)
    
    ; Обработка отрицательных чисел
    test rax, rax
    jns positive
    neg rax           ; Делаем число положительным
    mov byte ptr [rdi], '-' ; Записываем минус
    inc rdi
    
positive:
    ; Находим конец буфера (будем заполнять в обратном порядке)
    lea r8, [rdi + 32] ; Максимально возможная длина 64-битного числа
    mov r9, r8         ; R9 = конец буфера
    
convert_loop:
    xor rdx, rdx       ; Обнуляем RDX перед делением
    div r10            ; RDX:RAX / 10 → RAX=частное, RDX=остаток
    add dl, '0'        ; Преобразуем цифру в символ
    dec r8             ; Двигаемся назад по буферу
    mov [r8], dl       ; Сохраняем цифру
    test rax, rax      ; Частное = 0?
    jnz convert_loop   ; Нет - продолжаем
    
    ; Копируем цифры в правильном порядке
    mov rcx, r9        ; Конец буфера
    sub rcx, r8        ; Длина числа
    
copy_loop:
    mov al, [r8]
    mov [rdi], al
    inc r8
    inc rdi
    loop copy_loop
    
    ; Добавляем нуль-терминатор
    mov byte ptr [rdi], 0
    
    pop rdi
    pop rbx
    ret
IntToString endp

ProcessArrayInt proc inputPtr:QWORD
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    sub rsp, 32

    mov rsi, inputPtr
    
    ; Шаг 1: Разделить строки
    mov rcx, rsi         ; RCX = входная строка
    mov rdx, ','         ; RDX = разделитель
    call SplitString
    mov r12, rax         ; R12 = массив с указателями на строки

    xor r13, r13
count_loop:
    mov rax, [r12 + r13*8]
    test rax, rax
    jz count_done
    inc r13
    jmp count_loop
count_done:

    ; Выделить память под массив чисел
    mov rcx, r13
    shl rcx, 2
    call __imp_VirtualAlloc
    mov rbx, rax

    ; Строки в числа
    xor rdi, rdi
convert_loop:
    cmp rdi, r13
    jge convert_done
    
    mov rcx, [r12 + rdi*8]
    call StringToInt
    mov [rbx + rdi*4], eax
    inc rdi
    jmp convert_loop
convert_done:

    ; Шаг 2: Максимальный эелемент и его индекс
    mov rcx, rbx         ; RCX = указатель
    mov rdx, r13         ; RDX = счётчиук
    call FindMaxElement
    mov r9, rax          ; Максимальный элемент

    ; индекс максимумва
    xor r8, r8           ; R8 = индекс
find_max_idx:
    cmp [rbx + r8*4], r9d
    je max_found
    inc r8
    cmp r8, r13
    jl find_max_idx
max_found:

    ; Переварачиваем
    mov rcx, rbx         ; RCX = указатель на массив
    mov rdx, r13         ; RDX = счётчик
    ; в R8 есть индекс макс. элемента
    call SwapAroundMax

    ; Шаг 3: Конвертируем из чисел в строку
    lea rdi, lastResult
    xor r10, r10         ; R10 = индекс массива
build_string:
    cmp r10, r13
    jge string_done
    
    ; Конвертируем число в строку
    mov rcx, [rbx + r10*4]
    mov rdx, rdi         ; RDX = бафер для указателя
    call IntToString

    ; Находим конец у текущей строчки с числом
    mov rax, rdi
find_end:
    cmp byte ptr [rax], 0
    je end_found
    inc rax
    jmp find_end
end_found:
    mov rdi, rax         ; Обновляет бафер с указателем

    ; Доб. запятую
    cmp r10, r13
    je no_comma
    mov byte ptr [rdi], ','
    inc rdi
no_comma:
    inc r10
    jmp build_string
string_done:

    ; Освобождаем память
    mov rcx, rbx
    call __imp_VirtualFree
    mov rcx, r12
    call __imp_VirtualFree

    ; Return pointer to lastResult
    lea rax, lastResult

    ; Возврат
    add rsp, 32
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    ret
ProcessArrayInt endp

ProcessArrayByte proc inputPtr:DWORD
    ; Аналогично ProcessArrayInt, но с обработкой std::byte
    ; ...
    ret
ProcessArrayByte endp

ProcessMatrixInt proc inputPtr:DWORD
    ; 1. Разбиваем на строки по \r\n (SplitString)
    mov esi, inputPtr
    ; ... ASM реализация ...
    
    ; 2. Для каждой строки:
    ;   - разбиваем на числа
    ;   - находим максимум
    ;   - переставляем элементы
    ;   - формируем строку результата
    
    ; 3. Объединяем строки с \r\n
    
    ; Возвращаем указатель на lastResult
    lea eax, lastResult
    ret
ProcessMatrixInt endp

ProcessMatrixByte proc inputPtr:DWORD
    ; Аналогично ProcessMatrixInt, но с обработкой std::byte
    ; ...
    ret
ProcessMatrixByte endp

END
