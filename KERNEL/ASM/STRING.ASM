        INCLUDE OSINC.INC
        EXTRN   PUT_HEX:NEAR
        EXTRN   PUT_STRING:NEAR
        EXTRN   SYS_HASH_TABLE:DWORD
        EXTRN   DEBUG_SPIN_WAIT:NEAR
        EXTRN   FREE:NEAR
        EXTRN   CALL_EXTERN:NEAR
        EXTRN   GET_KEY:NEAR
        EXTRN   MALLOC                  :NEAR
        EXTRN   EVAL_EXPRESSION:NEAR
        EXTRN   RMALLOC                 :NEAR
        EXTRN   NEW_SYSFILES_STRING      :NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

        PUBLIC  ZEROS
ZEROS           DD      0,0

        PUBLIC  SYSTEM_STRING_TABLE,SYSTEM_EXTERN_TABLE
        ALIGN   4
SYSTEM_STRING_TABLE:
        DD      0
SYSTEM_EXTERN_TABLE:
        DD      0
ROOT_STARTUP_MSG:
        DB  'OS Compiled ',??date,' ',??time,13,10,0
;;**************************PROCEDURE*************************
; IN:   ESI=NAME
; OUT:  EDI=NEW_STRING
        ALIGN   4
        PUBLIC  LOAD_TEXT_FILE
LOAD_TEXT_FILE  PROC NEAR
        PUSH    EAX
        PUSH    EBX
        PUSH    ECX
        PUSH    ESI
        MOV     EDI,DWORD PTR SYS_HASH_TABLE
        MOV     EBX,HTT_FILE
        CALL    NEAR PTR FIND_HASH_ENTRY
        JNZ     @@50

        POP     ESI
        PUSH    ESI
        PUSH    0
        PUSH    ESI
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_READ_FILE*4[EAX]
        ADD     ESP,8
        OR      EAX,EAX
        JZ      @@60
        MOV     EDI,EAX
        JMP     @@80

@@50:   MOV     ESI,DWORD PTR FHTE_MEMORY_ADDRESS[ESI]
        CALL    NEW_SYSFILES_STRING
        JMP     @@80
@@60:   MOV     EAX,ST_FILE_NOT_FOUND
        INT     I_PUT_SYSTEXT
        LEA     ESI,BYTE PTR ZEROS
        CALL    NEAR PTR NEW_STRING
@@80:   POP     ESI
        POP     ECX
        POP     EBX
        POP     EAX
        RET
LOAD_TEXT_FILE  ENDP
;;**************************PROCEDURE*************************
; IN:   [ESI]
; OUT:  EAX=COUNT
        ALIGN   4
        PUBLIC  STRLEN
STRLEN  PROC NEAR
        PUSH    ESI     ;-****
@@1:    LODSB
        OR      AL,AL
        JNZ     @@1
        MOV     EAX,ESI
        POP     ESI     ;+****
        SUB     EAX,ESI
        DEC     EAX
        RET
STRLEN  ENDP

;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_STRCAT
CMD_STRCAT  PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
@@2:    LODSB
        OR      AL,AL
        JNZ     @@2
        MOV     EDI,ESI
        DEC     EDI
        MOV     ESI,DWORD PTR SF_PARAM2[EBP]
@@1:    LODSB
        STOSB
        OR      AL,AL
        JNZ     @@1
@@100:  POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_STRCAT  ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_STRCPY
CMD_STRCPY  PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM2[EBP]
        MOV     EDI,DWORD PTR SF_PARAM1[EBP]
@@1:    LODSB
        STOSB
        OR      AL,AL
        JNZ     @@1
@@100:  POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_STRCPY  ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_STRCMP
CMD_STRCMP  PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM2[EBP]
        MOV     EDI,DWORD PTR SF_PARAM1[EBP]
@@1:    LODSB
        OR      AL,AL
        JZ      @@100
        SCASB
        JE      @@1
        JA      @@90
@@80:   MOV     EAX,1
        JMP     @@101
@@90:   MOV     EAX,-1
        JMP     @@101
@@100:  SCASB
        JNE     @@80
        XOR     EAX,EAX
@@101:  POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_STRCMP  ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_ATOI
CMD_ATOI PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        XOR     EBX,EBX
        XOR     EAX,EAX
@@1:    LODSB
        SUB     AL,'0'
        CMP     AL,9
        JA      @@100
        IMUL    EBX,10
        ADD     EBX,EAX
        JMP     @@1
@@100:  MOV     EAX,EBX
        POP     ESI
        POP     EBP
        RET
CMD_ATOI  ENDP
;;**************************PROCEDURE*************************
; IN:   [ESI]
; OUT:  EDI
        ALIGN   4
        PUBLIC  NEW_STRING
NEW_STRING PROC NEAR
        PUSH    EAX     ;-****
        PUSH    ECX     ;-****

        CALL    STRLEN
        INC     EAX
        PUSH    EAX     ;-****
        PUSH    ESI     ;-****
        CALL    MALLOC
        MOV     EDI,ESI
        POP     ESI     ;+****
        POP     ECX     ;+****

        PUSH    ESI     ;-****
        PUSH    EDI     ;-****
        REP     MOVSB
        POP     EDI     ;+****
        POP     ESI     ;+****

        POP     ECX     ;+****
        POP     EAX     ;+****
        RET
NEW_STRING ENDP
;;**************************PROCEDURE*************************
; IN:   ESI+EDI, FREE's 1st string
; OUT:  ESI
        ALIGN   4
        PUBLIC  ADDS
ADDS PROC NEAR
        PUSH    EAX     ;-****
        PUSH    EBX     ;-****
        PUSH    ECX     ;-****
        PUSH    EDX     ;-****
        PUSH    EBP     ;-****
        MOV     EBX,ESI ;EBX=1st string
        MOV     EDX,EDI ;EDX=2nd string

        CALL    STRLEN
        MOV     ECX,EAX ;ECX=1st strlen
        MOV     ESI,EDX
        CALL    STRLEN
        INC     EAX
        PUSH    EAX     ;-**** STack has 2nd strlen+1

        ADD     EAX,ECX
        CALL    MALLOC
        MOV     EDI,ESI
        MOV     EBP,ESI ;EBP=Result

        MOV     ESI,EBX
        REP     MOVSB   ;Copy 1st string
        POP     ECX     ;+****
        MOV     ESI,EDX
        REP     MOVSB   ;Copy 2nd string

        MOV     ESI,EBX
        CALL    FREE    ;Free 1st string

        MOV     ESI,EBP
        MOV     EDI,EDX
        POP     EBP     ;+****
        POP     EDX     ;+****
        POP     ECX     ;+****
        POP     EBX     ;+****
        POP     EAX     ;+****
        RET
ADDS ENDP
;;**************************PROCEDURE*************************
; IN:   [ESI]
; OUT:  EAX
        ALIGN   4
        PUBLIC  HASH_STRING
HASH_STRING PROC NEAR
        PUSH    ESI     ;-****
        PUSH    EBX     ;-****
        PUSH    ECX     ;-****
        XOR     EAX,EAX
        XOR     EBX,EBX
@@1:    LODSB
        OR      AL,AL
        JZ      @@100
        SHL     EBX,1
        ADC     EBX,EAX
        JMP     @@1

@@100:  MOV     CL,16
        MOV     EAX,EBX 
        SHR     EBX,CL
        ADC     EAX,EBX      
        POP     ECX     ;+**** 
        POP     EBX     ;+****
        POP     ESI     ;+****
        RET
HASH_STRING ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=SIZE
; OUT:  [EDI]=TABLE
        ALIGN   4
        PUBLIC  NEW_HASH_TABLE
NEW_HASH_TABLE PROC NEAR
        PUSH    EAX     ;-****
        PUSH    ECX     ;-****
        MOV     ECX,EAX
        LEA     EAX,HT_BODY[EAX*4]
        CALL    MALLOC
        MOV     DWORD PTR HT_MASK[ESI],ECX
        DEC     DWORD PTR HT_MASK[ESI]
        MOV     EDI,ESI
        MOV     DWORD PTR HT_NEXT[EDI],0

        PUSHFD          ;+****
        PUSH    EDI     ;+****
        XOR     EAX,EAX
        LEA     EDI,HT_BODY[EDI]
        REP     STOSD
        POP     EDI     ;+****
        POPFD           ;+****
    
        POP     ECX     ;+****
        POP     EAX     ;+****
        RET
NEW_HASH_TABLE ENDP
;;**************************PROCEDURE*************************
; IN:   [ESI]=STRING
;       EBX=TYPE MASK
;       [EDI]=TABLE
; OUT:  [ESI]=ENTRY OR ZERO NOT FOUND
;       ZERO FLAG SET NOT FOUND
;       EAX=INDEX
        ALIGN   4
        PUBLIC  FIND_HASH_ENTRY
FIND_HASH_ENTRY PROC NEAR
        PUSH    EDX     ;-****
        MOV     EDX,ESI
        CALL    HASH_STRING
        AND     EAX,HT_MASK[EDI]
        PUSH    EAX     ;-****
        LEA     ESI,HT_BODY[EDI]
        MOV     ESI,DWORD PTR[ESI+EAX*4]
@@1:    OR      ESI,ESI
        JZ      @@100
        MOV     EAX,DWORD PTR HTE_TYPE[ESI]
        AND     EAX,EBX
        JE      @@90
        PUSH    ESI     ;-****
        PUSH    EDI     ;-****
        MOV     EDI,HTE_STRING[ESI]
        MOV     ESI,EDX
@@2:    LODSB
        MOV     AH,BYTE PTR [EDI]
        INC     EDI
        CMP     AL,AH
        JNE     @@80
        OR      AL,AL
        JNZ     @@2
        POP     EDI     ;+****
        POP     ESI     ;+****
        JMP     @@100
             
@@80:   POP     EDI     ;+****
        POP     ESI     ;+****

@@90:   MOV     ESI,DWORD PTR HTE_NEXT[ESI]
        JMP     @@1
        
@@100:  POP     EAX     ;+****
        POP     EDX     ;+****
        OR      ESI,ESI
        RET
FIND_HASH_ENTRY ENDP
;;**************************PROCEDURE*************************
; IN:   ES:[ESI]=STRING
;       [EDI]=TABLE
; OUT:  [ESI]=BUCKET
        ALIGN   4
        PUBLIC  FIND_HASH_BUCKET
FIND_HASH_BUCKET PROC NEAR
        PUSH    EAX
        CALL    HASH_STRING
        AND     EAX,HT_MASK[EDI]
        LEA     ESI,DWORD PTR HT_BODY[EDI+EAX*4]
        POP     EAX
        RET
FIND_HASH_BUCKET ENDP
;;**************************PROCEDURE*************************
; OUT:  ESI=STRING TABLE
        PUBLIC  INIT_SYSTEXT_AND_EXTERNS
        ALIGN   4
INIT_SYSTEXT_AND_EXTERNS     PROC NEAR
        MOV     EAX,EXT_TABLE_SIZE*4
        CALL    RMALLOC
        MOV     DWORD PTR SYSTEM_EXTERN_TABLE,ESI
        MOV     EDI,ESI
        MOV     ECX,EXT_TABLE_SIZE
        XOR     EAX,EAX
        REP STOSD
        MOV     EAX,ST_TABLE_SIZE*4
        CALL    RMALLOC
        MOV     DWORD PTR SYSTEM_STRING_TABLE,ESI
        MOV     EDI,ESI
        MOV     ECX,ST_TABLE_SIZE
        XOR     EAX,EAX
        REP STOSD
        MOV     EAX,ST_ROOT_STARTUP
        LEA     ECX,ROOT_STARTUP_MSG
        MOV     DWORD PTR [ESI+EAX*4],ECX
        RET
INIT_SYSTEXT_AND_EXTERNS     ENDP
;;**************************PROCEDURE*************************
        PUBLIC  PUT_SYSTEXT
        ALIGN   4
PUT_SYSTEXT     PROC NEAR
        PUSH    ESI
        MOV     ESI,DWORD PTR FS:TSS_STRING_TABLE
        MOV     ESI,DWORD PTR [ESI+EAX*4]
        OR      ESI,ESI
        JZ      @@10
@@1:    LODSB   
        OR      AL,AL
        JZ      @@2
        INT     I_PUT_CHAR
        JMP     @@1
@@10:   PUSH    EAX
        MOV     AL,'M'
        INT     I_PUT_CHAR
        POP     EAX
        CALL    PUT_HEX
        CALL    NEAR PTR SPACE
@@2:    POP     ESI
        RET
PUT_SYSTEXT     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CRLF
CRLF    PROC NEAR
        PUSH    EAX
        MOV     EAX,13
        INT     I_PUT_CHAR
        MOV     EAX,10
        INT     I_PUT_CHAR
        POP     EAX
        RET
CRLF    ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  SPACE
SPACE   PROC NEAR
        PUSH    EAX
        MOV     EAX,32
        INT     I_PUT_CHAR
        POP     EAX
        RET
SPACE   ENDP
;;**************************PROCEDURE*************************
; IN:   ECX=COUNT
        ALIGN   4
        PUBLIC  SPACES
SPACES  PROC NEAR
        OR      ECX,ECX
        JZ      @@100
        PUSH    EAX
        PUSH    ECX
        MOV     EAX,32
@@1:    INT     I_PUT_CHAR
        LOOP    @@1
        POP     ECX
        POP     EAX
@@100:  RET
SPACES  ENDP
;;**************************PROCEDURE*************************
; IN:   ECX=COUNT
        ALIGN   4
        PUBLIC  CMD_SPACES
CMD_SPACES  PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        MOV     ECX,SF_PARAM1[EBP]
        CALL    SPACES
        POP     ECX
        POP     EBP
@@100:  RET
CMD_SPACES  ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_LOADSTR
CMD_LOADSTR PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDX
        PUSH    ESI
        PUSH    EDI
        MOV     EDX,SF_PARAM1[EBP]
        MOV     ESI,SF_PARAM2[EBP]
        CALL    NEW_STRING
        MOV     ESI,DWORD PTR FS:TSS_STRING_TABLE
        MOV     DWORD PTR [ESI+EDX*4],EDI
        POP     EDI
        POP     ESI
        POP     EDX
        POP     EBP
        RET
CMD_LOADSTR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_DEFINE_EXTERN
        ALIGN   4
CMD_DEFINE_EXTERN PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        MOV     EBX,DWORD PTR SF_PARAM1[EBP]
        OR      EBX,EBX
        JZ      @@5
        CMP     EBX,EXT_TABLE_SIZE
        JB      @@10
@@5:    MOV     EAX,ST_INVALID_EXTERN_NUMBER
        INT     I_PUT_SYSTEXT
        RET

@@10:   LEA     EBX,DWORD PTR [EAX+4*EBX]
        MOV     EAX,DWORD PTR SF_PARAM2[EBP]
        MOV     DWORD PTR [EBX],EAX
        POP     EBP
        RET
CMD_DEFINE_EXTERN ENDP
        ENDS
        END
