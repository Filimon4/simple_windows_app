.data
    lastResult dw 256 dup(0)  ; ����� ��� �������� ���������� ���������� (Unicode)
    tempBuffer dw 256 dup(0)  ; ��������� �����
    
; ��������� �������
ProcessArrayInt PROTO :DWORD    ; ����: ��������� �� wchar_t ������
ProcessArrayByte PROTO :DWORD   ; ����: ��������� �� wchar_t ������
ProcessMatrixInt PROTO :DWORD   ; ����: ��������� �� wchar_t ������
ProcessMatrixByte PROTO :DWORD  ; ����: ��������� �� wchar_t ������

.code

; �������������� ������ � ����� (������ std::stoi)
StringToInt proc strPtr:DWORD
    push rsi
    push rdx
       
    xor eax, eax
    xor edx, edx

    mov rsi, rcx

convert_loop:
    mov dl, byte ptr [rsi]
    test dl, dl ; zero flat ���� ���� 0, ���������� ��������� �, ��������� ����� ������ "\0"
    jz conversion_done

    cmp dl, '0'
    jb invalid_char
    cmp dl, '9'
    ja invalid_char

    sub dl, '0' ; ACII �������. ���� �������� ������ 0, �� �������� �����
    imul eax, 10 ; ����������� �����������, ��� ���������� �����
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

; ��������� ������ �� ����������� (������ splitString)
SplitString proc strPtr:DWORD, delimiter:DWORD
    ; ...
    ret
SplitString endp

; ����� ������������� �������� � �������
FindMaxElement proc arrayPtr:DWORD, count:DWORD
    ; ...
    ret
FindMaxElement endp

; ������������ ��������� ������ ������������� (������ swapAroundMaxIdx)
SwapAroundMax proc arrayPtr:DWORD, count:DWORD, maxIndex:DWORD
    ; ...
    ret
SwapAroundMax endp

; �������������� ����� � ������ (������ std::to_wstring)
IntToString proc value:DWORD, strPtr:DWORD
    ; ...
    ret
IntToString endp

ProcessArrayInt proc inputPtr:DWORD
    ; ��������� ������� ���������
    mov esi, inputPtr
    
    ; 1. ��������� ������ �� ����� (SplitAndConvertToNumbers<int>)
    ; ���������� ���������� C++ ����, �� �� ASM
    
    ; 2. ������� ������������ ������� (SwapAroundMaxIdx)
    ; ... ASM ���������� ...
    
    ; 3. ��������� �������������� ������ (ConvertToWstring)
    ; ... ASM ���������� ...
    
    ; ��������� ��������� � lastResult
    lea edi, lastResult
    ; �������� �������������� ������ � lastResult
    
    ; ���������� ��������� �� lastResult
    mov eax, edi
    ret
ProcessArrayInt endp

ProcessArrayByte proc inputPtr:DWORD
    ; ���������� ProcessArrayInt, �� � ���������� std::byte
    ; ...
    ret
ProcessArrayByte endp

ProcessMatrixInt proc inputPtr:DWORD
    ; 1. ��������� �� ������ �� \r\n (SplitString)
    mov esi, inputPtr
    ; ... ASM ���������� ...
    
    ; 2. ��� ������ ������:
    ;   - ��������� �� �����
    ;   - ������� ��������
    ;   - ������������ ��������
    ;   - ��������� ������ ����������
    
    ; 3. ���������� ������ � \r\n
    
    ; ���������� ��������� �� lastResult
    lea eax, lastResult
    ret
ProcessMatrixInt endp

ProcessMatrixByte proc inputPtr:DWORD
    ; ���������� ProcessMatrixInt, �� � ���������� std::byte
    ; ...
    ret
ProcessMatrixByte endp

END
