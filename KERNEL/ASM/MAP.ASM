        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        EXTRN   SYS_HASH_TABLE:DWORD
        EXTRN   FIL_OS_MAP:BYTE
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ENDS

;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

;;**************************PROCEDURE*************************
; IN:   [ESI]
; OUT:  EDI
        ALIGN   4
        PUBLIC  NEW_SYSFILES_STRING
NEW_SYSFILES_STRING PROC NEAR
        PUSH    EAX     ;-****
        PUSH    ECX     ;-****
        PUSHFD
        CLI

        PUSH    ESI     ;-**** 
        PUSH    DS      ;-****
        MOV     AX,SYS_FILES_SEL
        MOV     DS,AX
        XOR     ECX,ECX
@@1:    INC     ECX
        LODSB
        OR      AL,AL
        JNZ     @@1
        POP     DS      ;+****

        PUSH    ECX     ;-****
        MOV     EAX,ECX
        CALL    MALLOC
        MOV     EDI,ESI
        POP     ECX     ;+****
        POP     ESI     ;+****

        PUSH    DS      ;-****
        PUSH    ESI     ;-****
        PUSH    EDI     ;-****
        MOV     AX,SYS_FILES_SEL
        MOV     DS,AX
        REP MOVSB
        POP     EDI     ;+****
        POP     ESI     ;+****
        POP     DS      ;+****
        
        POPFD
        POP     ECX     ;+****
        POP     EAX     ;+****
        RET
NEW_SYSFILES_STRING ENDP

        PUBLIC  ADD_MAP_SYMBOLS
ADD_MAP_SYMBOLS PROC NEAR
        MOV     ESI,OFFSET(FIL_OS_MAP)
        CALL    NEW_SYSFILES_STRING
        MOV     ESI,EDI
        PUSH    ESI
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
@@1:    CALL    LEX_GET_CHAR
        JZ      @@100
        CMP     AL,':'
        JNE     @@1
        OR      DWORD PTR LEX_FLAGS[EDI],LF_EXPECTING_HEX
        CALL    LEX
        JZ      @@100
        MOV     EBX,DWORD PTR LEX_CUR_I[EDI]
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_EXPECTING_HEX
        CALL    LEX
        JZ      @@100
        CMP     EAX,TK_IDENT
        JNE     @@1
        MOV     EAX,HTE_STRUCT_SIZE
        CALL    MALLOC
        MOV     ECX,ESI
        MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        MOV     DWORD PTR HTE_STRING[ECX],ESI
        MOV     EAX,DWORD PTR [ESI]
        CMP     EAX,'_LIF'
        JNE     @@2
        CALL    PUT_STRING
        MOV     AL,':'
        INT     I_PUT_CHAR
        MOV     EAX,EBX
        CALL    PUT_HEX
        CALL    CRLF
        MOV     DWORD PTR HTE_TYPE[ECX],HTT_FILE
        JMP     @@10
@@2:    MOV     DWORD PTR HTE_TYPE[ECX],HTT_SYS_SYMBOL
@@10:   MOV     DWORD PTR HTE_USER0[ECX],EBX
        PUSH    EDI
        MOV     EDI,DWORD PTR SYS_HASH_TABLE
        PUSH    ESI
        MOV     EBX,HTT_SYS_SYMBOL
        CALL    FIND_HASH_ENTRY       ;ONLY INSTALL ONCE
        JNZ     @@20
        POP     ESI
        CALL    FIND_HASH_BUCKET
        MOV     EAX,DWORD PTR [ESI]
        MOV     DWORD PTR HTE_NEXT[ECX],EAX
        MOV     DWORD PTR [ESI],ECX
        POP     EDI
        JMP     @@1
@@20:   POP     ESI
        CALL    FREE
        MOV     ESI,ECX
        CALL    FREE
        POP     EDI
        JMP     @@1
@@100:  CALL    DELETE_LEX
        POP     ESI
        CALL    FREE
        RET
ADD_MAP_SYMBOLS ENDP
        ENDS
        END

