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
SplitString proc strPtr:DWORD, delimiter:DWORD
    ; ...
    ret
SplitString endp

; Поиск максимального элемента в массиве
FindMaxElement proc arrayPtr:DWORD, count:DWORD
    ; ...
    ret
FindMaxElement endp

; Перестановка элементов вокруг максимального (аналог swapAroundMaxIdx)
SwapAroundMax proc arrayPtr:DWORD, count:DWORD, maxIndex:DWORD
    ; ...
    ret
SwapAroundMax endp

; Преобразование числа в строку (аналог std::to_wstring)
IntToString proc value:DWORD, strPtr:DWORD
    ; ...
    ret
IntToString endp

ProcessArrayInt proc inputPtr:DWORD
    ; Сохраняем входной указатель
    mov esi, inputPtr
    
    ; 1. Разбиваем строку на числа (SplitAndConvertToNumbers<int>)
    ; Реализация аналогична C++ коду, но на ASM
    
    ; 2. Находим максимальный элемент (SwapAroundMaxIdx)
    ; ... ASM реализация ...
    
    ; 3. Формируем результирующую строку (ConvertToWstring)
    ; ... ASM реализация ...
    
    ; Сохраняем результат в lastResult
    lea edi, lastResult
    ; Копируем сформированную строку в lastResult
    
    ; Возвращаем указатель на lastResult
    mov eax, edi
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
