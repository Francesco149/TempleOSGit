        INCLUDE OSINC.INC
        INCLUDE COMPILE.INC
        EXTRN   SPACE:NEAR
        EXTRN   DEBUG_SPIN_WAIT:NEAR
        EXTRN   PUT_HEX:NEAR
        EXTRN   PUT_STRING:NEAR
        EXTRN   PUT_HEX_BYTE:NEAR
        EXTRN   STRLEN:NEAR
        EXTRN   NEW_STRING:NEAR
        EXTRN   MALLOC:NEAR
        EXTRN   FREE:NEAR
        EXTRN   FIND_HASH_ENTRY:NEAR
        EXTRN   FIND_HASH_BUCKET:NEAR
        EXTRN   FIND_MEMBER:NEAR
        EXTRN   LOAD_TEXT_FILE:NEAR
        EXTRN   GET_STRING:NEAR
        EXTRN   GET_KEY:NEAR
        EXTRN   SYS_CURRENT_SCAN_CODE:DWORD
        EXTRN   SYSTEM_EXTERN_TABLE:DWORD
        EXTRN   CRLF:NEAR
        EXTRN   INIT_CB_IN_PTRS:NEAR
        EXTRN   GET_INTERMEDIATE_CODE:NEAR
        EXTRN   ADD_INTERMEDIATE_CODE:NEAR
        EXTRN   DELETE_LINKED_LIST:NEAR
        EXTRN   SWAP_IN_NEXT:NEAR
        EXTRN   TRANSLATE_SCAN_CODE:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

ALPHA_BITMAP            DD      00000000H,00000000H,87FFFFFEH,07FFFFFEH
                        DD      0,0,0,0
ALPHA_NUMERIC_BITMAP    DD      00000000H,03FF0000H,87FFFFFEH,07FFFFFEH
                        DD      0,0,0,0
DEC_NUMERIC_BITMAP      DD      00000000H,03FF0000H,0,0
                        DD      0,0,0,0
HEX_NUMERIC_BITMAP      DD      00000000H,03FF0000H,7EH,7EH
                        DD      0,0,0,0

DUEL_CHAR_TOKENS1:
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0F3Dh,0000h,0000h,0000h,0000h,0826h,0000h
        DW      0000h,0000h,0000h,102Bh,0000h,0E3Eh,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,053Dh,063Dh,073Dh,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,097Ch,0000h,0000h,0000h
DUEL_CHAR_TOKENS2:
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,112Dh,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0B3Ch,0000h,0C3Eh,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h
        DW      0000h,0000h,0000h,0000h,0000h,0000h,0000h,0000h


;;**************************PROCEDURE*************************
; IN:   ESI=BUFFER
;       EDX=FLAGS
; OUT:  EDI=LEX_NODE
        ALIGN   4
        PUBLIC  NEW_LEX
NEW_LEX PROC NEAR
        PUSH    EAX     ;-****
        PUSH    ECX     ;-****

        PUSH    ESI     ;-****
        MOV     EAX,LEX_STRUCT_SIZE
        CALL    MALLOC
        MOV     EDI,ESI
        MOV     ECX,LEX_STRUCT_SIZE/4
        XOR     EAX,EAX
        REP STOSD
        MOV     EDI,ESI

        MOV     ESI,DWORD PTR FS:TSS_HASH_TABLE
        MOV     DWORD PTR LEX_HASH_TABLE_LIST[EDI],ESI
        LEA     ESI,DWORD PTR LEX_LFNS[EDI]
        MOV     DWORD PTR LEX_CUR_LFN[EDI],ESI
        LEA     ESI,DWORD PTR LEX_STACK[EDI]
        MOV     DWORD PTR LEX_STACK_PTR[EDI],ESI
        POP     ESI     ;+****
        MOV     DWORD PTR LEX_FLAGS[EDI],EDX
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_PROMPT
        JZ      @@10
        MOV     EAX,256
        CALL    MALLOC
        MOV     DWORD PTR [ESI],0
@@10:   MOV     DWORD PTR LEX_LFNS+LFN_BUFFER[EDI],ESI
        MOV     DWORD PTR LEX_LFNS+LFN_BUF_PTR[EDI],ESI
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ESI
        MOV     DWORD PTR LEX_LFNS+LFN_LINE_NUM[EDI],1

        POP     ECX     ;+****
        POP     EAX     ;+****
        RET
NEW_LEX  ENDP

;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
        ALIGN   4
        PUBLIC  DELETE_LEX
DELETE_LEX PROC NEAR
        PUSH    EAX     ;-****
        PUSH    ECX     ;-****
        PUSH    ESI     ;-****

        TEST    DWORD PTR LEX_FLAGS[EDI],LF_DONT_FREE_BUFFER
        JZ      @@10
        MOV     DWORD PTR LEX_LFNS+LFN_BUFFER[EDI],0

@@10:   MOV     ECX,LFN_COUNT
        LEA     EAX,DWORD PTR LEX_LFNS[EDI]
@@1:    MOV     ESI,DWORD PTR LFN_BUFFER[EAX] 
        CALL    FREE
        MOV     ESI,DWORD PTR LFN_NAME[EAX] 
        CALL    FREE
        LOOP    @@1

        MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        CALL    FREE
        MOV     ESI,EDI
        CALL    FREE            
        POP     ESI     ;+****
        POP     ECX     ;+****
        POP     EAX     ;+****
        RET
DELETE_LEX  ENDP

;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
        ALIGN   4
        PUBLIC  PUT_TOKEN
PUT_TOKEN PROC NEAR
        PUSH    EAX
        PUSH    ESI
        MOV     AL,'"'
        INT     I_PUT_CHAR
        MOV     EAX,DWORD PTR LEX_TOKEN[EDI]
        CMP     EAX,TK_IDENT
        JE      @@10
        CMP     EAX,TK_STRING
        JNE     @@20
@@10:   MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        CALL    PUT_STRING
        JMP     @@100
@@20:   CMP     EAX,TK_INTEGER
        JNE     @@40
        MOV     EAX,DWORD PTR LEX_CUR_I[EDI]
        CALL    PUT_HEX
        JMP     @@100
@@40:   INT     I_PUT_CHAR
@@100:  MOV     AL,'"'
        INT     I_PUT_CHAR
        CALL    SPACE
        POP     ESI
        POP     EAX
        RET
PUT_TOKEN  ENDP

;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
; OUT:  EAX=CHAR
;       ZERO FLAG SET ON EOF
        ALIGN   4
        PUBLIC  LEX_GET_CHAR
LEX_GET_CHAR PROC NEAR
        TEST    LEX_FLAGS[EDI],LF_USE_LAST_CHAR
        JZ      @@1
        MOV     EAX,DWORD PTR LEX_LAST_CHAR[EDI]
        AND     LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        OR      EAX,EAX
        RET

@@1:    PUSH    ESI
        MOV     ESI,DWORD PTR LEX_CUR_BUF_PTR[EDI]
        MOVZX   EAX,BYTE PTR[ESI]
        INC     ESI
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ESI   
        POP     ESI
        MOV     DWORD PTR LEX_LAST_CHAR[EDI],EAX
        BT      DWORD PTR FS:TSS_TASK_FLAGS,TSSF_ECHO
        JNC     @@2
        INT     I_PUT_CHAR
@@2:    CMP     EAX,10
        JNE     @@2A
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR LEX_CUR_LFN[EDI]
        INC     DWORD PTR LFN_LINE_NUM[ESI]
        MOV     EDI,DWORD PTR LEX_CUR_BUF_PTR[EDI]
        MOV     DWORD PTR LFN_CUR_LINE_START[ESI],EDI
        POP     EDI
        POP     ESI
@@2A:   OR      EAX,EAX
        JZ      @@3
        RET

@@3:    PUSH    ESI
        PUSH    EDI
        LEA     ESI,DWORD PTR LEX_LFNS[EDI]
        MOV     EDI,DWORD PTR LEX_CUR_LFN[EDI]
        CMP     ESI,EDI
        JE      @@10
        MOV     ESI,LFN_BUFFER[EDI]
        CALL    FREE
        MOV     DWORD PTR LFN_BUFFER[EDI],0
        MOV     ESI,LFN_NAME[EDI]
        CALL    FREE
        MOV     DWORD PTR LFN_NAME[EDI],0
        POP     ESI
        SUB     EDI,LFN_STRUCT_SIZE
        MOV     EAX,DWORD PTR LFN_BUF_PTR[EDI]
        MOV     LEX_CUR_BUF_PTR[ESI],EAX
        MOV     LEX_CUR_LFN[ESI],EDI
        MOV     EDI,ESI
        POP     ESI
        JMP     @@1

@@10:   POP     EDI
        POP     ESI
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_PROMPT
        JNZ     @@11
        MOV     DWORD PTR LEX_LAST_CHAR[EDI],EAX
        OR      EAX,EAX
        RET

        
@@11:   MOV     EAX,ST_COMMAND_PROMPT
        INT     I_PUT_SYSTEXT
        PUSH    ECX
        MOV     ECX,DWORD PTR LEX_LFNS+LFN_BUFFER[EDI]
        MOV     DWORD PTR LEX_LFNS+LFN_BUF_PTR[EDI],ECX
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ECX
        PUSH    EDI             ;-****
        PUSH    LEX_FLAGS[EDI]  ;-****
        MOV     EDI,ECX

        PUSH    ESI             ;-****
        MOV     ESI,DWORD PTR FS:TSS_CONSOLE_LTF
        OR      ESI,ESI
        JZ      @@20
        PUSH    EAX                     ;-****
        PUSH    EBX                     ;-****
        PUSH    EDX                     ;-****
@@21:   MOV     DWORD PTR FS:TSS_MENU_SELECTION,0
@@21F:  BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY
        JNC     @@21D
        CALL    TRANSLATE_SCAN_CODE
        JMP     @@21G
@@21D:  MOV     EAX,DWORD PTR FS:TSS_MENU_SELECTION
        OR      EAX,EAX
        JNZ     @@21E
        CALL    SWAP_IN_NEXT
        JMP     @@21F
@@21E:  XOR     EAX,EAX
@@21G:  PUSH    EAX                     ;-****

        PUSH    DWORD PTR FS:TSS_CONSOLE_LTF
        AND     EAX,0FFh
        PUSH    EAX
        PUSH    DWORD PTR SYS_CURRENT_SCAN_CODE
        PUSH    DWORD PTR FS:TSS_MENU_SELECTION
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_LTF_PUTCHAR*4[EAX]
        ADD     ESP,16

        CMP     DWORD PTR FS:TSS_MENU_SELECTION,9999    ;COPY "FUNCTION TEXT"
        JNE     @@21A
        PUSH    ECX                     ;-****
        MOV     ESI,DWORD PTR FS:TSS_MENU_LTF
        MOV     ESI,DWORD PTR LTF_CUR_LINE[ESI]
        MOV     ESI,DWORD PTR LTFL_LINE_DATA[ESI]
        MOV     ECX,5                                   ;SKIP "9999 "
@@21B:  LODSB   
        OR      AL,AL
        JZ      @@21A
        LOOP    @@21B
        POP     ECX                     ;+****
@@21C:  LODSB
        AND     EAX,0FFh
        JZ      @@21A
        PUSH    DWORD PTR FS:TSS_CONSOLE_LTF
        PUSH    EAX
        PUSH    0               ;SCAN CODE
        PUSH    0               ;MENU SELECT
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_LTF_PUTCHAR*4[EAX]
        ADD     ESP,16
        JMP     @@21C

@@21A:  POP     EAX                     ;+****
        CMP     AL,13
        JNE     @@21
        MOV     ESI,DWORD PTR FS:TSS_CONSOLE_LTF
        MOV     ESI,DWORD PTR LTF_CUR_LINE[ESI]
        MOV     ESI,DWORD PTR LTFL_LAST[ESI]
        MOV     ESI,DWORD PTR LTFL_LINE_DATA[ESI]

        PUSH    EDI     ;Skip prompt
        MOV     EDI,DWORD PTR FS:TSS_STRING_TABLE
        MOV     EDI,DWORD PTR 3*4[EDI]
        OR      EDI,EDI
        JZ      @@23
@@24:   MOV     AL,BYTE PTR [EDI]
        OR      AL,AL
        JZ      @@23
        CMP     BYTE PTR [ESI],AL
        JNE     @@23
        MOV     AL,BYTE PTR[ESI]
        OR      AL,AL
        JZ      @@23
        INC     ESI 
        INC     EDI
        JMP     @@24
@@23:   POP     EDI

@@22:   LODSB
        STOSB
        OR      AL,AL
        JNZ     @@22

        POP     EDX                     ;-****
        POP     EBX                     ;-****
        POP     EAX                     ;-****
        POP     ESI                     ;-****
        JMP     @@30

@@20:   POP     ESI                     ;+****
        MOV     ECX,79
        CALL    GET_STRING
@@30:   DEC     EDI     ;BACK-UP TO ZERO
        POP     EAX             ;+****
        TEST    EAX,LF_DOUBLE_SEMICOLON
        JE      @@15    
        DEC     EDI
        MOV     AL,BYTE PTR [EDI]
        CMP     AL,';'
        JNE     @@14
        MOV     AX,';;'
        STOSW
        JMP     @@15
@@14:   INC     EDI        
@@15:   MOV     EAX,0A0Dh
        STOSD   
        MOV     EDI,DWORD PTR FS:TSS_CONSOLE_LTF
        OR      EDI,EDI
        JNZ     @@16
        CALL    CRLF
@@16:   POP     EDI     ;+****
        POP     ECX     ;+****
        JMP     @@1

LEX_GET_CHAR ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
;       ESI=INCLUDE FILENAME
        ALIGN   4
        PUBLIC  LEX_INCLUDE
LEX_INCLUDE PROC NEAR
        PUSH    EAX
        PUSH    EBX
        PUSH    ESI                     ;-****
        PUSH    EDI                     ;-****
        CALL    LOAD_TEXT_FILE
        MOV     ESI,EDI
        POP     EDI                     ;+****
        MOV     EBX,DWORD PTR LEX_CUR_LFN[EDI]
        MOV     EAX,DWORD PTR LEX_CUR_BUF_PTR[EDI]
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_USE_LAST_CHAR
        JZ      @@1
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        DEC     EAX
@@1:    MOV     DWORD PTR LFN_BUF_PTR[EBX],EAX
        ADD     EBX,LFN_STRUCT_SIZE             ;TODO:CHECK FOR OVERFLOW
        MOV     DWORD PTR LEX_CUR_LFN[EDI],EBX
        MOV     DWORD PTR LFN_BUFFER[EBX],ESI
        MOV     DWORD PTR LFN_BUF_PTR[EBX],ESI
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ESI
        MOV     DWORD PTR LFN_LINE_NUM[EBX],1
        POP     ESI
        MOV     DWORD PTR LFN_NAME[EBX],ESI
        POP     EBX
        POP     EAX
        RET
LEX_INCLUDE ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
        ALIGN   4
        PUBLIC  LEX_PUSH
LEX_PUSH PROC NEAR
        PUSH    EAX
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR LEX_CUR_LFN[EDI]
        MOV     EAX,DWORD PTR LEX_CUR_BUF_PTR[EDI]
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_USE_LAST_CHAR
        JZ      @@1
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        DEC     EAX
@@1:    MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],EAX
        MOV     DWORD PTR LFN_BUF_PTR[ESI],EAX
        MOV     EDI,DWORD PTR LEX_STACK_PTR[EDI]
        MOV     EAX,EDI
        MOV     ECX,LFN_STRUCT_SIZE/4
        REP MOVSD
        POP     EDI
        MOV     ESI,DWORD PTR LEX_STACK_PTR[EDI]
        ADD     ESI,LFN_STRUCT_SIZE
        MOV     DWORD PTR LEX_STACK_PTR[EDI],ESI
        POP     ESI
        POP     ECX
        POP     EAX
        RET
LEX_PUSH ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
        ALIGN   4
        PUBLIC  LEX_POP
LEX_POP PROC NEAR
        PUSH    EAX
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR LEX_STACK_PTR[EDI]
        SUB     ESI,LFN_STRUCT_SIZE
        MOV     DWORD PTR LEX_STACK_PTR[EDI],ESI
        MOV     EAX,DWORD PTR LFN_BUF_PTR[ESI]
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],EAX
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        MOV     EDI,DWORD PTR LEX_CUR_LFN[EDI]
        MOV     ECX,LFN_STRUCT_SIZE/4
        REP MOVSD
        POP     EDI
        POP     ESI
        POP     ECX
        POP     EAX
        RET
LEX_POP ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
        ALIGN   4
        PUBLIC  LEX_SUB_STACK_PTR
LEX_SUB_STACK_PTR PROC NEAR
        SUB     DWORD PTR LEX_STACK_PTR[EDI],LFN_STRUCT_SIZE
        RET
LEX_SUB_STACK_PTR ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
; OUT:  EAX=TOKEN
;       ZERO SET=EOF
        ALIGN   4
        PUBLIC  LEX
LEX PROC NEAR
        PUSH    EBX     ;-****
        PUSH    EDX     ;-****
        PUSH    ESI     ;-****
        PUSH    EDI     ;-****
        PUSH    EBP
        SUB     ESP,LEX_MAX_IDENT       ;-****
@@2B:   MOV     EBP,ESP
        XOR     EBX,EBX ;INDEX INTO IDENT
@@2:    CALL    LEX_GET_CHAR
@@2A:   MOV     BYTE PTR [EBP],AL
        JZ      @@100   ;JMP EOF
@@1:    TEST    LEX_FLAGS[EDI],LF_EXPECTING_HEX
        JNZ     @@82
@@1A:   BT      DWORD PTR ALPHA_BITMAP,EAX
        JC      @@90
        BT      DWORD PTR DEC_NUMERIC_BITMAP,EAX
        JC      @@70
        CMP     EAX,'"'
        JE      @@51
        CMP     EAX,'#'
        JE      @@200
        CMP     EAX,027H     ;"'"
        JE      @@300
                
        CMP     EAX,021H
        JB      @@2
        CMP     EAX,07EH
        JA      @@2
        
        MOV     DX,WORD PTR DUEL_CHAR_TOKENS1[EAX*2]
        OR      DX,DX
        JZ      @@35
        MOV     EBX,EAX
        CALL    LEX_GET_CHAR
        JZ      @@4
        CMP     AL,DL
        JNE     @@3
@@3A:   MOV     AL,DH
        JMP     @@100
@@3:    MOV     DX,WORD PTR DUEL_CHAR_TOKENS2[EBX*2]
        CMP     AL,DL
        JE      @@3A
        OR      LEX_FLAGS[EDI],LF_USE_LAST_CHAR
@@4:    MOV     EAX,EBX
        JMP     @@100

@@35:   CMP     EAX,'/'
        JNE     @@100
        CALL    LEX_GET_CHAR
        JZ      @@39
        CMP     EAX,'/'
        JNE     @@38
LEX_SKIP_EOL:
@@37:   CALL    LEX_GET_CHAR
        JZ      @@100
        CMP     EAX,13
        JE      @@2
        CMP     EAX,10
        JNE     @@37
        JMP     @@2     
@@38:   CMP     EAX,'*'
        JNE     @@39A
@@38A:  CALL    LEX_GET_CHAR
        JZ      @@100
        CMP     EAX,'*'
        JNE     @@38A
        CALL    LEX_GET_CHAR
        JZ      @@100
        CMP     EAX,'/'
        JNE     @@38A
        JMP     @@2

@@39A:  OR      LEX_FLAGS[EDI],LF_USE_LAST_CHAR
@@39:   MOV     EAX,'/'
        JMP     @@100

@@50:   INC     EBX
        CMP     EBX,LEX_MAX_IDENT-1
        JE      @@69     
@@51:   CALL    LEX_GET_CHAR
        JZ      @@69
        CMP     AL,'"'
        JE      @@69
        CMP     AL,'\'
        JE      @@52
        MOV     BYTE PTR [EBP+EBX],AL
        JMP     @@50
@@52:   CALL    LEX_GET_CHAR
        CMP     AL,'n'
        JNE     @@53
        MOV     BYTE PTR [EBP+EBX],10
        JMP     @@50
@@53:   CMP     AL,'r'
        JNE     @@54
        MOV     BYTE PTR [EBP+EBX],13
        JMP     @@50
@@54:   CMP     AL,'"'
        JNE     @@55
        MOV     BYTE PTR [EBP+EBX],'"'
        JMP     @@50
@@55:   CMP     AL,'\'
        JNE     @@56
        MOV     BYTE PTR [EBP+EBX],'\'
        JMP     @@50
@@56:   CMP     AL,'t'
        JNE     @@69
        MOV     BYTE PTR [EBP+EBX],9
        JMP     @@50
                                           ;TODO: Add \## (Be careful \0)

@@69:   MOV     EAX,TK_STRING
        JMP     @@98

@@300:  CALL    LEX_GET_CHAR            ; Char const
        JZ      @@310
        CMP     AL,'\'
        JNE     @@305
        CALL    LEX_GET_CHAR
        JZ      @@305
        CMP     AL,'n'
        JNE     @@301
        MOV     EAX,10
        JMP     @@305
@@301:  CMP     AL,'r'
        JNE     @@302
        MOV     EAX,13
        JMP     @@305
@@302:  CMP     AL,'"'
        JNE     @@303
        MOV     EAX,'"'
        JMP     @@305
@@303:  CMP     AL,'\'
        JNE     @@304
        MOV     EAX,'\'
        JMP     @@305
@@304:  CMP     AL,'t'
        JNE     @@305
        MOV     EAX,9

@@305:  MOV     EBX,EAX
        CALL    LEX_GET_CHAR
        JZ      @@75
        CMP     AL,027H
        JE      @@75
        JMP     @@73
@@310:  MOV     AL,027H
        JMP     @@100

@@70:   MOV     EBX,EAX        ;INT VAL
        SUB     EBX,'0'
@@71:   CALL    LEX_GET_CHAR
        JZ      @@75
        CMP     AL,'X'         ;TODO: NO 0x alone or 1x123
        JE      @@80
        CMP     AL,'x'
        JE      @@80
        BT      DWORD PTR DEC_NUMERIC_BITMAP,EAX
        JNC     @@73
        SUB     EAX,'0'
        IMUL    EBX,10
        ADD     EBX,EAX
        JMP     @@71
@@73:   OR      LEX_FLAGS[EDI],LF_USE_LAST_CHAR
@@75:   MOV     DWORD PTR LEX_CUR_I[EDI],EBX
        MOV     EAX,TK_INTEGER
        JMP     @@100

@@80:   CALL    LEX_GET_CHAR
        JZ      @@75
@@82:   BT      DWORD PTR HEX_NUMERIC_BITMAP,EAX
        JNC     @@73
        SUB     AL,'0'
        CMP     AL,10
        JB      @@81
        SUB     AL,'A'-'0'-10
        CMP     AL,16
        JB      @@81
        SUB     AL,'a'-'A'
@@81:   IMUL    EBX,16
        ADD     EBX,EAX
        JMP     @@80
            
@@90:   INC     EBX
        CMP     EBX,LEX_MAX_IDENT-1
        JE      @@99     
        CALL    LEX_GET_CHAR
        JZ      @@99
        MOV     BYTE PTR [EBP+EBX],AL
        BT      DWORD PTR ALPHA_NUMERIC_BITMAP,EAX
        JC      @@90
        OR      LEX_FLAGS[EDI],LF_USE_LAST_CHAR
@@99:   MOV     BYTE PTR [EBP+EBX],0

        MOV     DWORD PTR LEX_LOCAL_VAR_ENTRY[EDI],0
        PUSH    EBP
        MOV     ESI,EBP
        MOV     EBP,DWORD PTR LEX_LOCAL_VAR_LIST[EDI]
        OR      EBP,EBP
        JZ      @@99A
        CALL    FIND_MEMBER
        OR      EDX,EDX
        JZ      @@99A
        MOV     DWORD PTR LEX_LOCAL_VAR_ENTRY[EDI],EDX
        POP     EBP
        MOV     LEX_HASH_ENTRY[EDI],0
        JMP     @@93A
@@99A:  POP     EBP

        PUSH    EDI     ;-****
        MOV     LEX_HASH_ENTRY[EDI],0
        MOV     EDI,LEX_HASH_TABLE_LIST[EDI]
@@96:   OR      EDI,EDI
        JE      @@93
        MOV     ESI,EBP
        MOV     EBX,HTT_ALL
        CALL    FIND_HASH_ENTRY
        JZ      @@94
        POP     EDI     ;+****
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_NO_DEFINES
        JNZ     @@94A
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_STRING_CONSTANT
        JE      @@250
@@94A:  MOV     LEX_HASH_ENTRY[EDI],ESI
        MOV     EAX,TK_IDENT
        JMP     @@97
@@94:   MOV     EDI,DWORD PTR HT_NEXT[EDI]
        JMP     @@96
@@93:   POP     EDI     ;+****
@@93A:  MOV     EAX,TK_IDENT
        JMP     @@97
        

@@98:   MOV     BYTE PTR [EBP+EBX],0
@@97:   PUSH    EDI                     ;-****
        MOV     BX,SS
        MOV     ES,BX
        MOV     ESI,EBP
        CALL    NEW_STRING
        POP     ESI                     ;+****
        PUSH    ESI                     ;-****
        MOV     ESI,DWORD PTR LEX_IDENT[ESI]
        CALL    FREE
        POP     ESI                     ;+****
        MOV     DWORD PTR LEX_IDENT[ESI],EDI
        MOV     EDI,ESI

@@100:  MOV     DWORD PTR LEX_TOKEN[EDI],EAX
        ADD     ESP,LEX_MAX_IDENT       ;-****
        POP     EBP     ;+****
        POP     EDI     ;+****
        POP     ESI     ;+****
        POP     EDX     ;+****
        POP     EBX     ;+****
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_LEX_TRACE
        JNC     @@101
        CALL    PUT_TOKEN
@@101:  OR      EAX,EAX
        RET

@@200:  CALL    LEX     ;SKIP #
        CMP     EAX,TK_IDENT
        JNE     @@100
        MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@100
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_KEYWORD
        JNE     @@100
        MOV     EBX,DWORD PTR HTE_USER0[ESI]
        CMP     EBX,KW_INCLUDE
        JNE     @@210
        CALL    LEX     ;SKIP KEYWORD
        CMP     EAX,TK_STRING
        JNE     @@100
        MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        CALL    LEX_INCLUDE
        JMP     @@2B     
        
@@210:  CMP     EBX,KW_DEFINE
        JNE     @@220
        PUSH    DWORD PTR LEX_FLAGS[EDI]
        OR      DWORD PTR LEX_FLAGS[EDI],LF_NO_DEFINES
        CALL    LEX     ;SKIP KEYWORD
        POP     DWORD PTR LEX_FLAGS[EDI]
        CMP     EAX,TK_IDENT
        JNE     @@100
        MOV     EAX,HTE_STRUCT_SIZE
        CALL    MALLOC
        MOV     EDX,ESI
        MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        MOV     DWORD PTR HTE_STRING[EDX],ESI
        MOV     DWORD PTR HTE_TYPE[EDX],HTT_STRING_CONSTANT
        PUSH    EDI
        MOV     EDI,DWORD PTR FS:TSS_HASH_TABLE
        CALL    FIND_HASH_BUCKET
        POP     EDI
        MOV     EAX,DWORD PTR [ESI]
        MOV     DWORD PTR HTE_NEXT[EDX],EAX
        MOV     DWORD PTR [ESI],EDX
        MOV     EBP,ESP
        XOR     EBX,EBX ;INDEX INTO IDENT
@@211:  CMP     EBX,LEX_MAX_IDENT-1
        JE      @@212     
        CALL    LEX_GET_CHAR
        JZ      @@212
        CMP     EAX,13
        JE      @@212
        CMP     EAX,10
        JE      @@212
        MOV     BYTE PTR [EBP+EBX],AL
        INC     EBX
        JMP     @@211
@@212:  XOR     EAX,EAX
        MOV     BYTE PTR [EBP+EBX],AL
        MOV     ESI,EBP
        PUSH    EDI
        CALL    NEW_STRING
        MOV     DWORD PTR HTE_USER0[EDX],EDI
        POP     EDI
        JMP     @@2B

@@220:  JMP     @@100

;STRING CONSTANT
@@250:  MOV     ESI,DWORD PTR HTE_USER0[ESI]
        PUSH    EDI
        CALL    NEW_STRING
        MOV     ESI,EDI
        POP     EDI
        MOV     EBX,DWORD PTR LEX_CUR_LFN[EDI]
        MOV     EAX,DWORD PTR LEX_CUR_BUF_PTR[EDI]
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_USE_LAST_CHAR
        JZ      @@251
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        DEC     EAX
@@251:  MOV     DWORD PTR LFN_BUF_PTR[EBX],EAX
        ADD     EBX,LFN_STRUCT_SIZE             ;TODO:CHECK FOR OVERFLOW
        MOV     DWORD PTR LEX_CUR_LFN[EDI],EBX
        MOV     DWORD PTR LFN_BUFFER[EBX],ESI
        MOV     DWORD PTR LFN_BUF_PTR[EBX],ESI
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ESI
        MOV     DWORD PTR LFN_LINE_NUM[EBX],1
        JMP     @@2B

LEX ENDP

;;**************************PROCEDURE*************************
        PUBLIC  LEX_CB_PUSH
        ALIGN   4
LEX_CB_PUSH PROC NEAR
        PUSH    ESI
        PUSH    EAX
        PUSH    ECX

        MOV     EAX,LEX_STRUCT_SIZE
        CALL    MALLOC
        PUSH    EDI
        PUSH    ESI
        LEA     ESI,DWORD PTR LEX_CB_PUSH_START[EDI]
        POP     EDI
        PUSH    EDI
        LEA     EDI,DWORD PTR LEX_CB_PUSH_START[EDI]
        MOV     ECX,(LEX_CB_PUSH_END-LEX_CB_PUSH_START)/4
        MOV     EAX,ECX
        REP MOVSD
        POP     ESI
        POP     EDI
        MOV     DWORD PTR LEX_CB_STACK[EDI],ESI

        POP     ECX
        POP     EAX
        POP     ESI
        RET     
LEX_CB_PUSH ENDP
;;**************************PROCEDURE*************************
        PUBLIC  LEX_CB_POP
        ALIGN   4
LEX_CB_POP PROC NEAR
        PUSH    ESI
        PUSH    EAX
        PUSH    ECX

        MOV     ESI,DWORD PTR LEX_CB_STACK[EDI]
        PUSH    ESI
        LEA     ESI,DWORD PTR LEX_CB_PUSH_START[ESI]
        PUSH    EDI
        LEA     EDI,DWORD PTR LEX_CB_PUSH_START[EDI]
        MOV     ECX,(LEX_CB_PUSH_END-LEX_CB_PUSH_START)/4
        MOV     EAX,ECX
        REP MOVSD
        POP     EDI
        POP     ESI
        CALL    FREE

        POP     ECX
        POP     EAX
        POP     ESI
        RET     
LEX_CB_POP ENDP
;;**************************PROCEDURE*************************
        PUBLIC  LEX_CB_POP_NO_FREE
        ALIGN   4
LEX_CB_POP_NO_FREE PROC NEAR
        PUSH    EAX
        PUSH    ECX

        MOV     ESI,DWORD PTR LEX_CB_STACK[EDI]
        PUSH    ESI
        LEA     ESI,DWORD PTR LEX_CB_PUSH_START[ESI]
        PUSH    EDI
        LEA     EDI,DWORD PTR LEX_CB_PUSH_START[EDI]
        MOV     ECX,(LEX_CB_PUSH_END-LEX_CB_PUSH_START)/4
        MOV     EAX,ECX
        REP MOVSD
        POP     EDI
        POP     ESI

        POP     ECX
        POP     EAX
        RET     
LEX_CB_POP_NO_FREE ENDP
;;**************************PROCEDURE*************************
;IN:    EDI: MAIN LEX NODE
;       ESI: LEX NODE TO APPEND
        PUBLIC  LEX_CB_APPEND
        ALIGN   4
LEX_CB_APPEND PROC NEAR
        PUSH    EDI
        MOV     EDI,ESI
        CALL    INIT_CB_IN_PTRS
        POP     EDI
@@1:    PUSH    EDI
        MOV     EDI,ESI    
        CALL    GET_INTERMEDIATE_CODE
        POP     EDI     
        OR      EAX,EAX
        JZ      @@100
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@1
@@100:  PUSH    ESI
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[ESI]
        CALL    DELETE_LINKED_LIST
        POP     ESI
        LEA     EAX,DWORD PTR LEX_CB_LABEL_LIST[EDI]
        MOV     EBX,DWORD PTR LEX_CB_LABEL_LIST[ESI]
@@105:  MOV     ECX,DWORD PTR LB_NEXT[EAX]
        OR      ECX,ECX
        JZ      @@110
        MOV     EAX,ECX
        JMP     @@105
@@110:  MOV     DWORD PTR LB_NEXT[EAX],EBX
        CALL    FREE
        RET
LEX_CB_APPEND ENDP
        ENDS
        END
