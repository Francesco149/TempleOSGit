        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        INCLUDE COMPILE.INC
        EXTRN   SYS_MEMORY_BASE :DWORD
        EXTRN   SYS_INTERNAL_TYPES      :DWORD
        EXTRN   CODE_TEXT_TABLE:DWORD
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

;;**************************PROCEDURE*************************
; IN:   ECX=CLASS
        PUBLIC PUT_CLASS
        ALIGN   4
PUT_CLASS PROC NEAR
        NEG     EAX
        OR      ECX,ECX
        JZ      @@1
        PUSH    EAX
        PUSH    EBX
        PUSH    EDX
        PUSH    ESI
        MOVZX   EBX,BYTE PTR C_PTR_COUNT[ECX]
@@10:   OR      EBX,EBX
        JZ      @@20
        MOV     EAX,'*'
        INT     I_PUT_CHAR
        DEC     EBX
        JMP     @@10
@@20:   MOVZX   EAX,BYTE PTR C_PTR_COUNT[ECX]
        MOV     EBX,-C_STRUCT_SIZE
        IMUL    EBX
        ADD     EAX,ECX
        MOV     ESI,DWORD PTR C_HASH_STRING[EAX]
        CALL    PUT_STRING
        CALL    SPACE
        POP     ESI
        POP     EDX
        POP     EBX
        POP     EAX
@@1:    RET
PUT_CLASS ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=IC OPCODE
;       EBX=ARG
;       ECX=CLASS
        PUBLIC PUT_IC
        ALIGN   4
PUT_IC PROC NEAR
        CMP     EAX,EC_NOP
        JE      @@1
        PUSH    EAX
        PUSH    ESI
        AND     EAX,0FFh
        LEA     ESI,CODE_TEXT_TABLE[EAX*8]
        CALL    PUT_STRING
        CALL    SPACE
        MOV     EAX,EBX
        CALL    PUT_HEX
        CALL    SPACE
        MOV     EAX,ECX
        CALL    PUT_HEX
        CALL    SPACE
        CALL    PUT_CLASS
@@1A:   CALL    CRLF
        POP     ESI
;        CALL    GET_KEY
        POP     EAX
@@1:    RET
PUT_IC  ENDP
;;**************************PROCEDURE*************************
        PUBLIC  PUT_ALL_IC
PUT_ALL_IC PROC NEAR
        PUSHAD
        CALL    PUT_STRING
        CALL    CRLF
        CALL    NEAR PTR INIT_CB_IN_PTRS
        MOV     ESI,DWORD PTR LEX_CB_IN_PTR[EDI]
        LEA     ESI,DWORD PTR -IC_STRUCT_SIZE[ESI]
@@1B:   ADD     ESI,IC_STRUCT_SIZE
        MOV     EAX,DWORD PTR IC_CODE[ESI]
        OR      EAX,EAX
        JZ      @@1A
        MOV     EBX,DWORD PTR IC_DATA[ESI]
        MOV     ECX,DWORD PTR IC_CLASS[ESI]
        CALL    PUT_IC
        JMP     @@1B
@@1A:
        CALL    GET_KEY
        POPAD
        RET
PUT_ALL_IC ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=IC OPCODE
;       EBX=ARG
;       ECX=CLASS
        PUBLIC ADD_INTERMEDIATE_CODE
        ALIGN   4
ADD_INTERMEDIATE_CODE PROC NEAR
        PUSH    EAX
        PUSH    EBX
        PUSH    ECX
        PUSH    EDX
        MOV     EDX,DWORD PTR LEX_CB_OUT_PTR[EDI]
        MOV     DWORD PTR LEX_CB_LAST_OUT_PTR[EDI],EDX
        MOV     DWORD PTR IC_CODE[EDX],EAX
        MOV     DWORD PTR IC_DATA[EDX],EBX
        MOV     DWORD PTR IC_CLASS[EDX],ECX
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_PARSE_TRACE
        JNC     @@1
        CALL    PUT_IC
        
@@1:    ADD     EDX,IC_STRUCT_SIZE
        CMP     DWORD PTR LEX_CB_OUT_END[EDI],EDX
        JNE     @@100
        PUSH    ESI
        MOV     EAX,CB_STRUCT_SIZE
        CALL    MALLOC
        MOV     DWORD PTR CB_NEXT[ESI],0
        MOV     EAX,DWORD PTR LEX_CB_OUT_LAST[EDI]
        MOV     DWORD PTR CB_NEXT[EAX],ESI
        MOV     DWORD PTR LEX_CB_OUT_LAST[EDI],ESI
        LEA     EDX,DWORD PTR CB_DATA[ESI]
        LEA     EAX,DWORD PTR CB_STRUCT_SIZE[ESI]
        MOV     DWORD PTR LEX_CB_OUT_END[EDI],EAX
        POP     ESI
@@100:  MOV     DWORD PTR LEX_CB_OUT_PTR[EDI],EDX
        POP     EDX
        POP     ECX
        POP     EBX
        POP     EAX
        RET
ADD_INTERMEDIATE_CODE ENDP

;;**************************PROCEDURE*************************
        PUBLIC  INIT_CB_OUT_PTRS
INIT_CB_OUT_PTRS PROC NEAR
        MOV     EAX,CB_STRUCT_SIZE
        CALL    MALLOC
        MOV     DWORD PTR CB_NEXT[ESI],0
        MOV     DWORD PTR LEX_CB_OUT_FIRST[EDI],ESI
        MOV     DWORD PTR LEX_CB_OUT_LAST[EDI],ESI
        MOV     DWORD PTR LEX_CB_LABEL_LIST[EDI],0
        LEA     EAX,DWORD PTR CB_STRUCT_SIZE[ESI]
        MOV     DWORD PTR LEX_CB_OUT_END[EDI],EAX
        LEA     EAX,DWORD PTR CB_DATA[ESI]
        MOV     DWORD PTR LEX_CB_OUT_PTR[EDI],EAX
        MOV     DWORD PTR LEX_CB_LAST_OUT_PTR[EDI],EAX
        RET
INIT_CB_OUT_PTRS ENDP


;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  EAX=IC OPCODE
;       EBX=ARG
;       ECX=CLASS
        PUBLIC  GET_INTERMEDIATE_CODE
        ALIGN   4
GET_INTERMEDIATE_CODE PROC NEAR
        PUSH    EDX
        MOV     EDX,DWORD PTR LEX_CB_IN_PTR[EDI]
        MOV     EAX,DWORD PTR IC_CODE[EDX]
        MOV     EBX,DWORD PTR IC_DATA[EDX]
        MOV     ECX,DWORD PTR IC_CLASS[EDX]
        ADD     EDX,IC_STRUCT_SIZE
        CMP     DWORD PTR LEX_CB_IN_END[EDI],EDX
        JNE     @@100
        PUSH    EAX
        LEA     EDX,DWORD PTR -CB_STRUCT_SIZE[EDX]
        MOV     EDX,DWORD PTR CB_NEXT[EDX]
        LEA     EAX,DWORD PTR CB_STRUCT_SIZE[EDX]
        MOV     DWORD PTR LEX_CB_IN_END[EDI],EAX
        LEA     EDX,DWORD PTR CB_DATA[EDX]
        POP     EAX
@@100:  MOV     DWORD PTR LEX_CB_IN_PTR[EDI],EDX
        POP     EDX
        RET
GET_INTERMEDIATE_CODE ENDP
;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  EAX=IC OPCODE
;       EBX=ARG
;       ECX=CLASS
        PUBLIC  GET_INTERMEDIATE_CODE2
        ALIGN   4
GET_INTERMEDIATE_CODE2 PROC NEAR
        PUSH    EDX
        MOV     EDX,DWORD PTR LEX_CB_IN_PTR[EDI]
        MOV     EAX,DWORD PTR IC_CODE[EDX]
        MOV     EBX,DWORD PTR IC_DATA[EDX]
        MOV     ECX,DWORD PTR IC_CLASS[EDX]
        ADD     EDX,IC_STRUCT_SIZE
        MOV     DWORD PTR LEX_CB_IN_PTR[EDI],EDX
        POP     EDX
        RET
GET_INTERMEDIATE_CODE2 ENDP


;;**************************PROCEDURE*************************
        PUBLIC  INIT_CB_IN_PTRS
INIT_CB_IN_PTRS PROC NEAR
        MOV     EBX,DWORD PTR LEX_CB_OUT_FIRST[EDI]
        MOV     DWORD PTR LEX_CB_IN_FIRST[EDI],EBX
        LEA     EAX,DWORD PTR CB_STRUCT_SIZE[EBX]
        MOV     DWORD PTR LEX_CB_IN_END[EDI],EAX
        LEA     EBX,DWORD PTR CB_DATA[EBX]
        MOV     DWORD PTR LEX_CB_IN_PTR[EDI],EBX
        RET
INIT_CB_IN_PTRS ENDP

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
        ALIGN   4
COMBINE_CODE_BLOCKS PROC NEAR
        XOR     EAX,EAX         ;COUNT CB NODES IN LIST
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[EDI]
@@1:    OR      ESI,ESI
        JZ      @@10
        INC     EAX
        MOV     ESI,DWORD PTR CB_NEXT[ESI]
        JMP     @@1
@@10:   IMUL    EAX,(CB_STRUCT_SIZE-CB_DATA)
        ADD     EAX,CB_DATA
        CALL    MALLOC          ;COPY CB NODE LIST DATA TO ONE BIG NODE
        PUSH    ESI
        PUSH    EDI
        XCHG    ESI,EDI
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[ESI]
        XOR     EAX,EAX
        STOSD
@@11:   OR      ESI,ESI
        JZ      @@20
        LEA     ESI,DWORD PTR CB_DATA[ESI]
        MOV     ECX,(CB_STRUCT_SIZE-CB_DATA)/4
        REP MOVSD
        LEA     ESI,DWORD PTR CB_NEXT-CB_STRUCT_SIZE[ESI]
        MOV     EAX,DWORD PTR CB_NEXT[ESI]
        CALL    FREE
        MOV     ESI,EAX
        JMP     @@11
@@20:   POP     EDI
        POP     DWORD PTR LEX_CB_OUT_FIRST[EDI]
        RET
COMBINE_CODE_BLOCKS ENDP
;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
        PUBLIC  FIXUP_CODE
        ALIGN   4
FIXUP_CODE PROC NEAR
;IN: EDI LEX_NODE
;DURING:
;    ESI PROGRAM COUNTER
; STACK: DATATYPES
        CALL    COMBINE_CODE_BLOCKS
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_OPT_TRACE
        JNC     @@1B
        CALL    PUT_ALL_IC
@@1B:   PUSHAD
        PUSH    EDI
        CALL    INIT_CB_IN_PTRS
        MOV     ESI,DWORD PTR LEX_CB_IN_PTR[EDI]
        LEA     ESI,DWORD PTR -IC_STRUCT_SIZE[ESI]
        PUSH    0
FIXUP_CODE ENDP
        
FIXUP_MAIN_LOOP PROC NEAR
        ADD     ESI,IC_STRUCT_SIZE
        MOV     EAX,DWORD PTR IC_CODE[ESI]
        OR      EAX,EAX
        JNZ     @@1
        ADD     ESP,4
        POP     EDI

        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_OPT_TRACE
        JNC     @@1A
        CALL    PUT_ALL_IC
@@1A:   POPAD
        RET

@@1:    AND     EAX,0FFh
        JMP     DWORD PTR FIXUP_CODE_TABLE[EAX*4]
FIXUP_MAIN_LOOP ENDP

FIXUP_CODE_TABLE:
        DD      FCT_NULL
        DD      FCT_XOR
        DD      FCT_OR_OR 
        DD      FCT_AND_AND
        DD      FCT_LESS
        DD      FCT_GREATER
        DD      FCT_EQUAL_EQUAL
        DD      FCT_NOT_EQUAL_TO
        DD      FCT_GREATER_OR_EQUAL
        DD      FCT_LESS_OR_EQUAL
        DD      FCT_ADD
        DD      FCT_SUB
        DD      FCT_MUL
        DD      FCT_DIV
        DD      FCT_MOD
        DD      FCT_UNARY_MINUS
        DD      FCT_NOT
        DD      FCT_INT
        DD      FCT_DOUBLE
        DD      FCT_LOCAL_FRAME_PTR
        DD      FCT_DEREFERENCE
        DD      FCT_CALL
        DD      FCT_RET
        DD      FCT_INC
        DD      FCT_DEC
        DD      FCT_INT8
        DD      FCT_ADD32
        DD      FCT_SUB32
        DD      FCT_ADD8
        DD      FCT_SUB8
        DD      FCT_MUL32
        DD      FCT_DIV32
        DD      FCT_MUL8
        DD      FCT_DIV8
        DD      FCT_ASSIGN32
        DD      FCT_ASSIGN16
        DD      FCT_ASSIGN8
        DD      FCT_ADD_ESP
        DD      FCT_NOP
        DD      FCT_OR
        DD      FCT_AND
        DD      FCT_SHL
        DD      FCT_SHR
        DD      FCT_COM
        DD      FCT_PRINTF
        DD      FCT_LABEL
        DD      FCT_SKIPZ
        DD      FCT_JMP
        DD      FCT_ZBD
        DD      FCT_SBD
        DD      FCT_ZWD
        DD      FCT_SWD
        DD      FCT_DWORD_PP
        DD      FCT_WORD_PP
        DD      FCT_BYTE_PP
        DD      FCT_short_PP
        DD      FCT_char_PP
        DD      FCT_DWORD_MM
        DD      FCT_WORD_MM
        DD      FCT_BYTE_MM
        DD      FCT_short_MM
        DD      FCT_char_MM
        DD      FCT_ADDRESS
        DD      FCT_FS
        DD      FCT_CLI
        DD      FCT_STI
        DD      FCT_GET_FLAGS
        DD      FCT_SET_FLAGS
        DD      FCT_BT
        DD      FCT_BTS
        DD      FCT_BTR
        DD      FCT_INSQUE
        DD      FCT_INSQUE_REV
        DD      FCT_REMQUE
        DD      FCT_INP
        DD      FCT_OUTP
        DD      FCT_REP_INSB
        DD      FCT_REP_INSW
        DD      FCT_REP_OUTSB
        DD      FCT_REP_OUTSW
        DD      FCT_MEMCPY
        DD      FCT_MEMSET
        DD      FCT_INPW
        DD      FCT_OUTPW
        DD      FCT_PP_DWORD
        DD      FCT_PP_WORD
        DD      FCT_PP_BYTE
        DD      FCT_PP_short
        DD      FCT_PP_char
        DD      FCT_MM_DWORD
        DD      FCT_MM_WORD
        DD      FCT_MM_BYTE
        DD      FCT_MM_short
        DD      FCT_MM_char
        DD      FCT_INPD
        DD      FCT_OUTPD
        DD      FCT_ENTER
        DD      FCT_LEAVE
        DD      FCT_TYPE
        DD      FCT_DS
        DD      FCT_ADD_ESP8
        DD      FCT_DWORD_DISP32   
        DD      FCT_WORD_DISP32    
        DD      FCT_BYTE_DISP32    
        DD      FCT_DWORD_DISP8    
        DD      FCT_WORD_DISP8     
        DD      FCT_BYTE_DISP8 
        DD      FCT_CALL_INDIRECT   
        DD      FCT_TABLE_END
FIXUP_BINARY_OP PROC NEAR
        MOV     EAX,DWORD PTR IC_CLASS[ESI]
        TEST    BYTE PTR C_PTR_COUNT[EAX],0FFh
        JNZ     @@1A
        MOV     EAX,DWORD PTR IC_CLASS[EDI]
        MOV     EBX,DWORD PTR IC_CLASS[EDX]
        MOV     ECX,DWORD PTR HTE_USER0[EAX]
        CMP     ECX,DWORD PTR HTE_USER0[EBX]
        JAE     @@1B
        MOV     EAX,EBX
@@1B:   MOV     DWORD PTR IC_CLASS[ESI],EAX

@@1A:   CMP     BYTE PTR IC_CODE[EDI],EC_INT32
        JNZ     @@1
        CMP     BYTE PTR IC_CODE[EDX],EC_INT32
        JNZ     @@1
@@1:    MOV     EAX,DWORD PTR IC_CLASS[ESI]
        BT      DWORD PTR HTE_USER0[EAX],0      
        RET     ;Z=1 means both const  C=1 means unsigned
FIXUP_BINARY_OP ENDP
FIXUP_BINARY_OP2 PROC NEAR
        CMP     BYTE PTR IC_CODE[EDI],EC_INT32
        JE      @@1D
        CMP     BYTE PTR IC_CODE[EDX],EC_INT32
        JNE     @@1E
        XCHG    EDX,EDI
@@1D:   STC
        RET     ;C=1 means EDI const
@@1E:   CLC
        RET
FIXUP_BINARY_OP2 ENDP
FIXUP_BINARY_OP3 PROC NEAR
        CMP     BYTE PTR IC_CODE[EDI],EC_INT32
        JNE     @@1F
        STC
        RET     ;C=1 means EDI const
@@1F:   CLC
        RET
FIXUP_BINARY_OP3 ENDP
FIXUP_CHECK_BP_DISP PROC NEAR
        CMP     BYTE PTR IC_CODE[EDI],EC_ADD32
        JNE     @@1G
        LEA     EDX,DWORD PTR -2*IC_STRUCT_SIZE[EDI]
        CMP     BYTE PTR IC_CODE[EDX],EC_LOCAL_FRAME_PTR
        JNE     @@1G
        STC
        RET     ;C=1 means has potential
@@1G:   CLC
        RET
FIXUP_CHECK_BP_DISP ENDP
FCT_NULL:
FCT_XOR:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@2
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        XOR     EAX,DWORD PTR IC_DATA[EDX]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@2:    PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_OR_OR:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@2
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        OR      EAX,DWORD PTR IC_DATA[EDX]
        JZ      @@3
        MOV     EAX,1
@@3:    MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@2:    PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_AND_AND:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@4
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        AND     EAX,EAX
        JZ      @@5
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        AND     EAX,EAX
        JZ      @@5
        MOV     EAX,1
@@5:    MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@4:    PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_LESS:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@6
        JC      @@7A
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JLE     @@7
        INC     EAX
        JMP     @@7
@@7A:   MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JBE     @@7
        INC     EAX
@@7:    MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@6:    PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_GREATER:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@8
        JC      @@9A
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JGE     @@9
        INC     EAX
        JMP     @@9
@@9A:   MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JAE     @@9
        INC     EAX
@@9:    MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@8:    PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_EQUAL_EQUAL:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@10
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JNE     @@11
        INC     EAX
@@11:   MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@10:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_NOT_EQUAL_TO:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@12
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JE      @@13
        INC     EAX
@@13:   MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@12:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_GREATER_OR_EQUAL:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@14
        JC      @@15A
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JG      @@15
        INC     EAX
        JMP     @@15
@@15A:  MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JA      @@15
        INC     EAX
@@15:   MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@14:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_LESS_OR_EQUAL:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@16
        JC      @@17A
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JL      @@17
        INC     EAX
        JMP     @@17
@@17A:  MOV     EAX,DWORD PTR IC_DATA[EDI]
        CMP     EAX,DWORD PTR IC_DATA[EDX]
        MOV     EAX,0
        JB      @@17
        INC     EAX
@@17:   MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@16:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_ADD:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@18
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        ADD     EAX,DWORD PTR IC_DATA[EDX]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
@@18:   CALL    FIXUP_BINARY_OP2
        JNC     @@18A
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_ADD32
@@18A:  PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_SUB:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@19
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        SUB     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
@@19:   CALL    FIXUP_BINARY_OP3
        JNC     @@19A
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_SUB32
@@19A:  PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_MUL:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@20
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        JC      @@20A
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        IMUL    DWORD PTR IC_DATA[EDI]
        JMP     @@20B 
@@20A:  MOV     EAX,DWORD PTR IC_DATA[EDX]
        MUL     DWORD PTR IC_DATA[EDI]

@@20B:  MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
@@20:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DIV:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@21
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        JC      @@21A
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        XOR     EDX,EDX
        OR      EAX,EAX
        JNS     @@21C
        NOT     EDX
@@21C:  IDIV    DWORD PTR IC_DATA[EDI]
        JMP     @@21B
@@21A:  MOV     EAX,DWORD PTR IC_DATA[EDX]
        XOR     EDX,EDX
        DIV     DWORD PTR IC_DATA[EDI]
@@21B:  MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
@@21:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_MOD:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@22
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        JC      @@22A
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        XOR     EDX,EDX
        OR      EAX,EAX
        JNS     @@22C
        NOT     EDX
@@22C:  IDIV    DWORD PTR IC_DATA[EDI]
        JMP     @@22B
@@22A:  MOV     EAX,DWORD PTR IC_DATA[EDX]
        XOR     EDX,EDX
        DIV     DWORD PTR IC_DATA[EDI]
@@22B:  MOV     DWORD PTR IC_DATA[ESI],EDX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
@@22:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_UNARY_MINUS:
        POP     EDX
        MOV     EBX,DWORD PTR IC_CLASS[EDX]
        MOV     ECX,DWORD PTR HTE_USER0[EBX]
        TEST    ECX,1
        JZ      @@23A
        DEC     ECX            ;Convert to signed
        MOV     EAX,DWORD PTR SYS_INTERNAL_TYPES[ECX*4]
        MOV     DWORD PTR IC_CLASS[ESI],EAX

@@23A:  CMP     DWORD PTR IC_CODE[EDX],EC_INT32
        JNZ     @@23
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        NEG     EAX
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@23:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_NOT:
        POP     EDX
        CMP     DWORD PTR IC_CODE[EDX],EC_INT32
        JNZ     @@24
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        OR      EAX,EAX
        MOV     EAX,1
        JZ      @@24A
        XOR     EAX,EAX
@@24A:  MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@24:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_INT:
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DOUBLE:
        JMP     FIXUP_MAIN_LOOP
FCT_LOCAL_FRAME_PTR:
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DEREFERENCE:
        POP     EDI
        CALL    FIXUP_CHECK_BP_DISP
        JNC     @@33
        MOV     DWORD PTR IC_CODE[ESI],EC_DWORD_DISP32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
@@33:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_CALL:
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_RET:
        JMP     FIXUP_MAIN_LOOP
FCT_INC:
        POP     EDX
        CMP     DWORD PTR IC_CODE[EDX],EC_INT32
        JNZ     @@25
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        INC     EAX
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@25:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DEC:
        POP     EDX
        CMP     DWORD PTR IC_CODE[EDX],EC_INT32
        JNZ     @@26
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        DEC     EAX
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@26:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_INT8:
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_ADD32:
FCT_SUB32:
FCT_ADD8:
FCT_SUB8:
FCT_MUL32:
FCT_DIV32:
FCT_MUL8:
FCT_DIV8:
;NOT APPLICABLE
        JMP     FIXUP_MAIN_LOOP
FCT_ASSIGN32:
FCT_ASSIGN16:
FCT_ASSIGN8:
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_ADD_ESP:
        POP     EDX
        ADD     ESP,DWORD PTR IC_DATA[ESI]
        LEA     EDX,DWORD PTR -IC_STRUCT_SIZE[ESI]
        CMP     DWORD PTR IC_CODE[EDX],EC_ADD_ESP
        JNE     @@27A
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        ADD     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@27A:  PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_NOP:
        JMP     FIXUP_MAIN_LOOP
FCT_OR:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@27
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        OR      EAX,DWORD PTR IC_DATA[EDX]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@27:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_AND:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@28
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        AND     EAX,DWORD PTR IC_DATA[EDX]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@28:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_SHL:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@28
        MOV     ECX,DWORD PTR IC_DATA[EDI]
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        SHL     EAX,CL
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@28:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_SHR:
        POP     EDI
        POP     EDX
        CALL    FIXUP_BINARY_OP
        JNZ     @@29
        MOV     ECX,DWORD PTR IC_DATA[EDI]
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        SHL     EAX,CL
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@29:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_COM:
        POP     EDX
        CMP     DWORD PTR IC_CODE[EDX],EC_INT32
        JNZ     @@30
        MOV     EAX,DWORD PTR IC_DATA[EDX]
        NOT     EAX
        MOV     DWORD PTR IC_DATA[ESI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_INT32
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
@@30:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_PRINTF:
FCT_LABEL:
FCT_SKIPZ:
FCT_SKIPZ2:
FCT_JMP:
        JMP     FIXUP_MAIN_LOOP
FCT_ZBD:
FCT_SBD:
        POP     EDI
        CALL    FIXUP_CHECK_BP_DISP
        JNC     @@31
        MOV     DWORD PTR IC_CODE[ESI],EC_BYTE_DISP32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
@@31:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_ZWD:
FCT_SWD:
        POP     EDI
        CALL    FIXUP_CHECK_BP_DISP
        JNC     @@32
        MOV     DWORD PTR IC_CODE[ESI],EC_WORD_DISP32
        MOV     DWORD PTR IC_CODE[EDI],EC_NOP
        MOV     DWORD PTR IC_CODE[EDX],EC_NOP
        MOV     EAX,DWORD PTR IC_DATA[EDI]
        MOV     DWORD PTR IC_DATA[ESI],EAX
@@32:   PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DWORD_PP:
FCT_WORD_PP:
FCT_BYTE_PP:
FCT_short_PP:
FCT_char_PP:
FCT_DWORD_MM:
FCT_WORD_MM:
FCT_BYTE_MM:
FCT_short_MM:
FCT_char_MM:
        POP     EDX
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_ADDRESS:
        JMP     FIXUP_MAIN_LOOP
FCT_FS:
FCT_CLI:
FCT_STI:
FCT_GET_FLAGS:
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_SET_FLAGS:
        JMP     FIXUP_MAIN_LOOP
FCT_BT:
FCT_BTS:
FCT_BTR:
FCT_INSQUE:
FCT_INSQUE_REV:
        POP     EDI
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_REMQUE:    
FCT_INP:    
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_OUTP:    
        POP     EDI
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_REP_INSB:    
FCT_REP_INSW:    
FCT_REP_OUTSB:    
FCT_REP_OUTSW:    
FCT_MEMCPY:    
FCT_MEMSET:    
        POP     EDI
        POP     EDX
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_INPW:    
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_OUTPW:    
        POP     EDI
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_PP_DWORD:
FCT_PP_WORD:
FCT_PP_BYTE:
FCT_PP_short:
FCT_PP_char:
FCT_MM_DWORD:
FCT_MM_WORD:
FCT_MM_BYTE:
FCT_MM_short:
FCT_MM_char:
FCT_INPD:    
        POP     EDX
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_OUTPD:    
        POP     EDI
        POP     EDX
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_ENTER:    
FCT_LEAVE:    
        JMP     FIXUP_MAIN_LOOP
FCT_TYPE:
        POP     EDX
        MOV     EAX,DWORD PTR IC_CLASS[EDX]
        MOV     DWORD PTR IC_DATA[ESI],EAX
        PUSH    EDX
        PUSH    ESI
        JMP     FIXUP_MAIN_LOOP
FCT_DS:
        PUSH    ESI
;UNFINISHED--SET TYPE
        JMP     FIXUP_MAIN_LOOP
FCT_ADD_ESP8:
        ADD     ESP,DWORD PTR IC_DATA[ESI]
        JMP     OPT_MAIN_LOOP
FCT_DWORD_DISP32:
FCT_WORD_DISP32:
FCT_BYTE_DISP32:
FCT_DWORD_DISP8:
FCT_WORD_DISP8:
FCT_BYTE_DISP8:
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
FCT_CALL_INDIRECT:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP   
FCT_TABLE_END:

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  CARRY CLEAR MEANS NO OPT FOUND
        PUBLIC  OPTIMIZE_CODE
        ALIGN   4
OPTIMIZE_CODE PROC NEAR
;IN: EDI LEX_NODE 
;DURING:
;    ESI PROGRAM COUNTER
; STACK: DATATYPES
        PUSHAD
        PUSH    EDI
        CALL    INIT_CB_IN_PTRS
        MOV     ESI,DWORD PTR LEX_CB_IN_PTR[EDI]
        LEA     ESI,DWORD PTR -IC_STRUCT_SIZE[ESI]
        PUSH    0
OPTIMIZE_CODE ENDP
OPT_MAIN_LOOP PROC NEAR
        ADD     ESI,IC_STRUCT_SIZE
        MOV     EAX,DWORD PTR IC_CODE[ESI]
        OR      EAX,EAX
        JNZ     @@1
        ADD     ESP,4
        POP     EDI

        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_OPT_TRACE
        JNC     @@1A
        CALL    PUT_ALL_IC
@@1A:   POPAD
        CLC
        RET

@@1:    AND     EAX,0FFh
        JMP     DWORD PTR OPT_CODE_TABLE[EAX*4]
OPT_MAIN_LOOP ENDP

OPT_CODE_TABLE:
        DD      OCT_NULL
        DD      OCT_XOR
        DD      OCT_OR_OR 
        DD      OCT_AND_AND
        DD      OCT_LESS
        DD      OCT_GREATER
        DD      OCT_EQUAL_EQUAL
        DD      OCT_NOT_EQUAL_TO
        DD      OCT_GREATER_OR_EQUAL
        DD      OCT_LESS_OR_EQUAL
        DD      OCT_ADD
        DD      OCT_SUB
        DD      OCT_MUL
        DD      OCT_DIV
        DD      OCT_MOD
        DD      OCT_UNARY_MINUS
        DD      OCT_NOT
        DD      OCT_INT
        DD      OCT_DOUBLE
        DD      OCT_LOCAL_FRAME_PTR
        DD      OCT_DEREFERENCE
        DD      OCT_CALL
        DD      OCT_RET
        DD      OCT_INC
        DD      OCT_DEC
        DD      OCT_INT8
        DD      OCT_ADD32
        DD      OCT_SUB32
        DD      OCT_ADD8
        DD      OCT_SUB8
        DD      OCT_MUL32
        DD      OCT_DIV32
        DD      OCT_MUL8
        DD      OCT_DIV8
        DD      OCT_ASSIGN32
        DD      OCT_ASSIGN16
        DD      OCT_ASSIGN8
        DD      OCT_ADD_ESP
        DD      OCT_NOP
        DD      OCT_OR
        DD      OCT_AND
        DD      OCT_SHL
        DD      OCT_SHR
        DD      OCT_COM
        DD      OCT_PRINTF
        DD      OCT_LABEL
        DD      OCT_SKIPZ
        DD      OCT_JMP
        DD      OCT_ZBD
        DD      OCT_SBD
        DD      OCT_ZWD
        DD      OCT_SWD
        DD      OCT_DWORD_PP
        DD      OCT_WORD_PP
        DD      OCT_BYTE_PP
        DD      OCT_short_PP
        DD      OCT_char_PP
        DD      OCT_DWORD_MM
        DD      OCT_WORD_MM
        DD      OCT_BYTE_MM
        DD      OCT_short_MM
        DD      OCT_char_MM
        DD      OCT_ADDRESS
        DD      OCT_FS
        DD      OCT_CLI
        DD      OCT_STI
        DD      OCT_GET_FLAGS
        DD      OCT_SET_FLAGS
        DD      OCT_BT
        DD      OCT_BTS
        DD      OCT_BTR
        DD      OCT_INSQUE
        DD      OCT_INSQUE_REV
        DD      OCT_REMQUE
        DD      OCT_INP
        DD      OCT_OUTP
        DD      OCT_REP_INSB
        DD      OCT_REP_INSW
        DD      OCT_REP_OUTSB
        DD      OCT_REP_OUTSW
        DD      OCT_MEMCPY
        DD      OCT_MEMSET
        DD      OCT_INPW
        DD      OCT_OUTPW
        DD      OCT_PP_DWORD
        DD      OCT_PP_WORD
        DD      OCT_PP_BYTE
        DD      OCT_PP_short
        DD      OCT_PP_char
        DD      OCT_MM_DWORD
        DD      OCT_MM_WORD
        DD      OCT_MM_BYTE
        DD      OCT_MM_short
        DD      OCT_MM_char
        DD      OCT_INPD
        DD      OCT_OUTPD
        DD      OCT_ENTER
        DD      OCT_LEAVE
        DD      OCT_TYPE
        DD      OCT_DS
        DD      OCT_ADD_ESP8
        DD      OCT_DWORD_DISP32   
        DD      OCT_WORD_DISP32    
        DD      OCT_BYTE_DISP32    
        DD      OCT_DWORD_DISP8    
        DD      OCT_WORD_DISP8     
        DD      OCT_BYTE_DISP8     
        DD      OCT_CALL_INDIRECT
        DD      OCT_TABLE_END
OCT_NULL:
OCT_XOR:
OCT_OR_OR:
OCT_AND_AND:
OCT_LESS:
OCT_GREATER:
OCT_EQUAL_EQUAL:
OCT_NOT_EQUAL_TO:
OCT_GREATER_OR_EQUAL:
OCT_LESS_OR_EQUAL:
OCT_ADD:
OCT_SUB:
OCT_MUL:
OCT_DIV:
OCT_MOD:
        POP     EDI
        POP     EDX
@@22:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_UNARY_MINUS:
OCT_NOT:
        POP     EDX
@@24:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_INT:
        PUSH    ESI
@@24A:  JMP     OPT_MAIN_LOOP
OCT_DOUBLE:
        JMP     OPT_MAIN_LOOP
OCT_LOCAL_FRAME_PTR:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_DEREFERENCE:
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_CALL:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_RET:
        JMP     OPT_MAIN_LOOP
OCT_INC:
        POP     EDX
@@25:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_DEC:
        POP     EDX
@@26:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_INT8:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ADD32:
        POP     EDI
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        OR      EAX,EAX         ;CHeck +0
        JNZ     @@26C
        MOV     EAX,DWORD PTR IC_CLASS[ESI]
        MOV     DWORD PTR IC_CLASS[EDI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_NOP
        PUSH    EDI
        JMP     OPT_MAIN_LOOP

@@26C:  CMP     EAX,1
        JNE     @@26D
        MOV     DWORD PTR IC_CODE[ESI],EC_INC
        JMP     @@26A
@@26D:  CMP     EAX,0FFFFFFFFh
        JNE     @@26E
        MOV     DWORD PTR IC_CODE[ESI],EC_DEC
        JMP     @@26A

@@26E:  CMP     EAX,07Fh
        JG      @@26A
        CMP     EAX,0FFFFFF80h
        JL      @@26A
        MOV     DWORD PTR IC_CODE[ESI],EC_ADD8
@@26A:  PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_SUB32:
        POP     EDI
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        OR      EAX,EAX         ;CHeck +0
        JNZ     @@26F
        MOV     EAX,DWORD PTR IC_CLASS[ESI]
        MOV     DWORD PTR IC_CLASS[EDI],EAX
        MOV     DWORD PTR IC_CODE[ESI],EC_NOP
        PUSH    EDI
        JMP     OPT_MAIN_LOOP

@@26F:  CMP     EAX,1
        JNE     @@26G
        MOV     DWORD PTR IC_CODE[ESI],EC_DEC
        JMP     @@26B
@@26G:  CMP     EAX,0FFFFFFFFh
        JNE     @@26H
        MOV     DWORD PTR IC_CODE[ESI],EC_DEC
        JMP     @@26B

@@26H:  CMP     EAX,07Fh
        JG      @@26B
        CMP     EAX,0FFFFFF80h
        JL      @@26B
        MOV     DWORD PTR IC_CODE[ESI],EC_SUB8
@@26B:  PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ADD8:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_SUB8:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_MUL32:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_DIV32:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_MUL8:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_DIV8:
;NOT APPLICABLE
        JMP     OPT_MAIN_LOOP
OCT_ASSIGN32:
OCT_ASSIGN16:
OCT_ASSIGN8:
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ADD_ESP:
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        ADD     ESP,EAX
        CMP     EAX,07Fh
        JG      @@27A
        CMP     EAX,0FFFFFF80h
        JL      @@27A
        MOV     DWORD PTR IC_CODE[ESI],EC_ADD_ESP8
@@27A:  JMP     OPT_MAIN_LOOP
OCT_NOP:
        JMP     OPT_MAIN_LOOP
OCT_OR:
OCT_AND:
OCT_SHL:
OCT_SHR:
        POP     EDI
        POP     EDX
@@29:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_COM:
        POP     EDX
@@30:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_PRINTF:
        JMP     OPT_MAIN_LOOP
OCT_LABEL:
        JMP     OPT_MAIN_LOOP
OCT_SKIPZ:
        JMP     OPT_MAIN_LOOP
OCT_SKIPZ2:
        JMP     OPT_MAIN_LOOP
OCT_JMP:
        JMP     OPT_MAIN_LOOP
OCT_ZBD:
OCT_SBD:
OCT_ZWD:
OCT_SWD:
OCT_DWORD_PP:
OCT_WORD_PP:
OCT_BYTE_PP:
OCT_short_PP:
OCT_char_PP:
OCT_DWORD_MM:
OCT_WORD_MM:
OCT_BYTE_MM:
OCT_short_MM:
OCT_char_MM:
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ADDRESS:
        JMP     OPT_MAIN_LOOP
OCT_FS:
OCT_CLI:
OCT_STI:
OCT_GET_FLAGS:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_SET_FLAGS:
        JMP     OPT_MAIN_LOOP
OCT_BT:
OCT_BTS:
OCT_BTR:
OCT_INSQUE:
OCT_INSQUE_REV:
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_REMQUE:    
OCT_INP:    
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_OUTP:    
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_REP_INSB:    
OCT_REP_INSW:    
OCT_REP_OUTSB:    
OCT_REP_OUTSW:    
OCT_MEMCPY:    
OCT_MEMSET:    
        POP     EDI
        POP     EDX
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_INPW:    
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_OUTPW:    
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_PP_DWORD:
OCT_PP_WORD:
OCT_PP_BYTE:
OCT_PP_short:
OCT_PP_char:
OCT_MM_DWORD:
OCT_MM_WORD:
OCT_MM_BYTE:
OCT_MM_short:
OCT_MM_char:
OCT_INPD:    
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_OUTPD:    
        POP     EDI
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ENTER:    
        JMP     OPT_MAIN_LOOP
OCT_LEAVE:    
        JMP     OPT_MAIN_LOOP
OCT_TYPE:
OCT_DS:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_ADD_ESP8:
        ADD     ESP,DWORD PTR IC_DATA[ESI]
        JMP     OPT_MAIN_LOOP
OCT_DWORD_DISP32:
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        CMP     EAX,07Fh
        JG      @@31
        CMP     EAX,0FFFFFF80h
        JL      @@31
        MOV     DWORD PTR IC_CODE[ESI],EC_DWORD_DISP8
@@31:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_WORD_DISP32:
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        CMP     EAX,07Fh
        JG      @@32
        CMP     EAX,0FFFFFF80h
        JL      @@32
        MOV     DWORD PTR IC_CODE[ESI],EC_WORD_DISP8
@@32:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_BYTE_DISP32:
        MOV     EAX,DWORD PTR IC_DATA[ESI]
        CMP     EAX,07Fh
        JG      @@33
        CMP     EAX,0FFFFFF80h
        JL      @@33
        MOV     DWORD PTR IC_CODE[ESI],EC_BYTE_DISP8
@@33:   PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_DWORD_DISP8:
OCT_WORD_DISP8:
OCT_BYTE_DISP8:
        POP     EDX
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_CALL_INDIRECT:
        PUSH    ESI
        JMP     OPT_MAIN_LOOP
OCT_TABLE_END:
        ENDS
        END

