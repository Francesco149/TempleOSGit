        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        INCLUDE COMPILE.INC
        EXTRN   SYS_INTERNAL_TYPES:DWORD
        EXTRN   ADD_INTERMEDIATE_CODE:NEAR
        EXTRN   COMPILE_CODE:NEAR
        EXTRN   FIXUP_CODE:NEAR
        EXTRN   INIT_CB_OUT_PTRS:NEAR
        
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

;;**************************PROCEDURE*************************
;OUT:   ESI
        ALIGN   4
NEW_LABEL PROC NEAR
        PUSH    EAX
        MOV     EAX,LB_STRUCT_SIZE
        CALL    MALLOC
        MOV     EAX,DWORD PTR LEX_CB_LABEL_LIST[EDI]
        MOV     DWORD PTR LB_NEXT[ESI],EAX
        MOV     DWORD PTR LEX_CB_LABEL_LIST[EDI],ESI
        MOV     DWORD PTR LB_STRING[ESI],0
        MOV     DWORD PTR LB_ADDRESS[ESI],0
        POP     EAX
        RET
NEW_LABEL ENDP
;;**************************PROCEDURE*************************
;IN:    EBP=CLASS
;       ESI=STRING
;OUT:   EDX=MEMBER_LIST_NODE
        PUBLIC  FIND_MEMBER
        ALIGN   4
FIND_MEMBER PROC NEAR
        PUSH    EAX
        PUSH    ECX
        PUSH    EDI
        PUSH    ESI
        MOV     EDX,DWORD PTR C_MEM_LIST[EBP]
@@1:    OR      EDX,EDX
        JZ      @@100
        XOR     ECX,ECX
        MOV     EDI,DWORD PTR ML_STRING[EDX]
@@10:   MOV     AL,BYTE PTR [ESI+ECX]
        CMP     BYTE PTR [EDI+ECX],AL
        JNE     @@50
        INC     ECX
        OR      AL,AL
        JNZ     @@10
        JMP     @@100
@@50:   MOV     EDX,DWORD PTR ML_NEXT[EDX]
        JMP     @@1
@@100:  POP     ESI
        POP     EDI
        POP     ECX
        POP     EAX
        RET
FIND_MEMBER ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
;OUT:   EBP=CLASS
        PUBLIC  NEW_CLASS
STD_DEREF       EQU     EC_DEREFERENCE+100h+65536*(EC_ASSIGN32+700h)
NEW_CLASS PROC NEAR
        PUSH    EAX
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     EAX,C_STRUCT_SIZE*4
        CALL    MALLOC
        MOV     EBP,ESI

        MOV     EDI,ESI
        XOR     EAX,EAX
        MOV     ECX,C_STRUCT_SIZE*4/4
        REP STOSD

        MOV     DWORD PTR C_HASH_TYPE[ESI],HTT_CLASS
        LEA     EAX,DWORD PTR C_MEM_LIST[ESI]
        MOV     DWORD PTR C_MEM_LIST_LAST[ESI],EAX
        MOV     DWORD PTR C_DEREFERENCE[ESI],STD_DEREF

        ADD     ESI,C_STRUCT_SIZE
        MOV     DWORD PTR C_HASH_TYPE[ESI],HTT_CLASS
        MOV     DWORD PTR C_SIZE[ESI],4
        MOV     DWORD PTR C_DEREFERENCE[ESI],STD_DEREF
        MOV     BYTE PTR C_PTR_COUNT[ESI],1

        ADD     ESI,C_STRUCT_SIZE
        MOV     DWORD PTR C_HASH_TYPE[ESI],HTT_CLASS
        MOV     DWORD PTR C_SIZE[ESI],4
        MOV     DWORD PTR C_DEREFERENCE[ESI],STD_DEREF
        MOV     BYTE PTR C_PTR_COUNT[ESI],2

        ADD     ESI,C_STRUCT_SIZE
        MOV     DWORD PTR C_HASH_TYPE[ESI],HTT_CLASS
        MOV     DWORD PTR C_SIZE[ESI],4
        MOV     DWORD PTR C_DEREFERENCE[ESI],STD_DEREF
        MOV     BYTE PTR C_PTR_COUNT[ESI],3

        POP     EDI
        POP     ESI
        POP     ECX
        POP     EAX
        RET
NEW_CLASS ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
;IN:    EBP=CLASS_MEMBER_NODE
;       EAX=0 LOCAL VAR, 1 FUN PARAM, 2 CLASS
PARSE_VAR_LIST PROC NEAR
        PUSH    EAX
        OR      EAX,EAX
        JZ      @@2A
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JE      @@1B
        CMP     DWORD PTR LEX_TOKEN[EDI],'{'
        JNE     @@170
@@1B:   CALL    LEX     ;SKIP (
@@1:    CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@1A
        CALL    LEX    ;SKIP ;
        JMP     @@1
@@1A:   CMP     DWORD PTR LEX_TOKEN[EDI],'}'
        JE      @@100
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JE      @@100

        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@140
        MOV     EBX,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      EBX,EBX
        JZ      @@140
@@2A:   XOR     EDX,EDX ;PTR FLAG COUNT
@@2:    CALL    LEX_PUSH
        CALL    LEX     ;SKIP TYPE
        CMP     EAX,'*'
        JNE     @@3
        INC     EDX
        CALL    LEX_SUB_STACK_PTR
        JMP     @@2
@@3:    CMP     EAX,TK_IDENT
        JNE     @@130
        MOV     EAX,ML_STRUCT_SIZE
        CALL    MALLOC
        XOR     EAX,EAX
        MOV     WORD PTR ML_FLAGS[ESI],AX
        MOV     WORD PTR ML_PARAM_COUNT[ESI],AX
        MOV     DWORD PTR ML_CLASS[ESI],EBX
        MOV     EAX,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR ML_STRING[ESI],EAX
        MOV     DWORD PTR LEX_IDENT[EDI],0
        MOV     DWORD PTR ML_COUNT[ESI],0
        MOV     DWORD PTR ML_NEXT[ESI],0
        MOV     ECX,DWORD PTR C_MEM_LIST_LAST[EBP]
        MOV     DWORD PTR ML_NEXT[ECX],ESI
        INC     WORD PTR C_MEM_COUNT[EBP]
        MOV     DWORD PTR C_MEM_LIST_LAST[EBP],ESI
        MOV     ECX,DWORD PTR C_SIZE[EBX]
        CALL    LEX     ;SKIP NAME
        CMP     EAX,'('
        JNE     @@4
        OR      WORD PTR ML_FLAGS[ESI],MLF_FUNCTION
        CALL    LEX     ;SKIP '('
        CMP     EAX,')'
        JE      @@6
        INC     WORD PTR ML_PARAM_COUNT[ESI]
        CALL    LEX
@@5:    CMP     EAX,')'
        JE      @@6
        CMP     EAX,','
        JNE     @@7
@@8:    INC     WORD PTR ML_PARAM_COUNT[ESI]
@@7:    CALL    LEX
        JMP     @@5

@@6:    CALL    LEX     ;SKIP ')'
@@4:    CMP     EAX,'['
        JNE     @@80
        CALL    LEX     ;SKIP [
        CALL    GET_EXPRESSION
        JC      @@120
        MOV     DWORD PTR ML_COUNT[ESI],EAX
        CMP     DWORD PTR LEX_TOKEN[EDI],']'
        JNE     @@105
        CALL    LEX     ;SKIP ]
@@80:   MOV     EAX,C_STRUCT_SIZE
        MUL     EDX
        MOV     EDX,EAX
        ADD     ML_CLASS[ESI],EAX
        MOV     ECX,ML_CLASS[ESI]
        MOV     ECX,C_SIZE[ECX]
        TEST    WORD PTR ML_FLAGS[ESI],MLF_FUNCTION
        JZ      @@72
        MOV     ECX,4
@@72:   MOV     EAX,DWORD PTR ML_COUNT[ESI]
        OR      EAX,EAX
        JNZ     @@71
        INC     EAX
@@71:   MUL     ECX

        MOV     ECX,ESP
        MOV     ECX,[ECX]
        OR      ECX,ECX
        JNZ     @@75
        SUB     DWORD PTR C_SIZE[EBP],EAX       ;FUN LOCAL VAR
        MOV     EAX,DWORD PTR C_SIZE[EBP]
        MOV     DWORD PTR ML_OFFSET[ESI],EAX
        CMP     DWORD PTR LEX_TOKEN[EDI],'='
        JNE     @@98
        CALL    LEX_POP
        CALL    LEX
        PUSH    EBX
        PUSH    EBP
        MOV     EAX,3
        CALL    NEAR PTR PARSE_STATEMENT
        POP     EBP
        POP     EBX
        JMP     @@99
                
@@75:   CMP     ECX,1
        JNE     @@76
        MOV     EAX,DWORD PTR C_SIZE[EBP]       ;FUN PARAM
        MOV     DWORD PTR ML_OFFSET[ESI],EAX
        ADD     DWORD PTR C_SIZE[EBP],4
        CMP     DWORD PTR LEX_TOKEN[EDI],'='
        JNE     @@98
        CALL    LEX
        CALL    GET_EXPRESSION
        JC      @@120
        MOV     DWORD PTR ML_DEFAULT[ESI],EAX
        OR      WORD PTR ML_FLAGS[ESI],MLF_DEFAULT_AVAILABLE
        JMP     @@98

@@76:   MOV     ECX,DWORD PTR C_SIZE[EBP]       ;CLASS
        MOV     DWORD PTR ML_OFFSET[ESI],ECX
        ADD     DWORD PTR C_SIZE[EBP],EAX
        JMP     @@98

@@98:   CALL    LEX_SUB_STACK_PTR       ;(GROWS UP)
@@99:   MOV     ECX,ESP
        MOV     ECX,[ECX]
        CMP     DWORD PTR LEX_TOKEN[EDI],','
        JNE     @@99A
        CMP     ECX,1
        JNE     @@2A
        CALL    LEX     ;SKIP ,
        JMP     @@1
@@99A:  OR      ECX,ECX
        JZ      @@101
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JE      @@100
        CMP     DWORD PTR LEX_TOKEN[EDI],'}'
        JE      @@100        
        CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@110
        CALL    LEX     ;SKIP ;
        JMP     @@1

@@100:  CALL    LEX     ;SKIP } or )
@@101:  POP     EAX
        RET
@@105:  MOV     EAX,ST_MISSING_RIGHT_BRACKET
        JMP     TRAP
@@110:  MOV     EAX,ST_MISSING_SEMICOLON
@@120:  JMP     TRAP
@@130:  MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
@@140:  MOV     EAX,ST_EXPECTING_TYPE
        JMP     TRAP
@@170:  MOV     EAX,ST_EXPECTING_LEFT_BRACE
        JMP     TRAP
PARSE_VAR_LIST ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
;IN:    EBP=CLASS MEMBER NODE
PARSE_LOCAL_VAR_LIST PROC NEAR
        XOR     EAX,EAX
        CALL    PARSE_VAR_LIST
        CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@110
        CALL    LEX     ;SKIP ;
        RET
@@110:  MOV     EAX,ST_MISSING_SEMICOLON
        JMP     TRAP
PARSE_LOCAL_VAR_LIST ENDP
;;**************************PROCEDURE*************************
STRING_EQUAL:
        DB      '=',0
;IN:    EDX=KEYWORD
;       EBX=CLASS HASH ENTRY
        ALIGN   4
PARSE_GLOBAL_LIST        PROC NEAR
        PUSH    EBP
@@2A:   PUSH    EBX
@@2:    CALL    LEX_PUSH
        CALL    LEX 
        CMP     EAX,'*'
        JNE     @@10
        ADD     EBX,C_STRUCT_SIZE
        CALL    LEX_SUB_STACK_PTR
        JMP     @@2
@@10:   CMP     EAX,TK_IDENT
        JNE     @@120
        MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@11
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_KEYWORD
        JNE     @@11
        CMP     DWORD PTR HTE_USER0[ESI],KW_SYSFUN
        JE      PARSE_SYSFUN
        CMP     DWORD PTR HTE_USER0[ESI],KW_INTERNAL
        JE      PARSE_INTERNAL
@@11:   PUSH    DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        CALL    LEX     ;SKIP NAME
        CMP     EAX,'('
        JNE     @@12
        POP     ESI
        JMP     PARSE_FUN
@@12:   XOR     ECX,ECX
        CMP     EAX,'['
        JNE     @@50
        CALL    LEX     ;SKIP '['
        CALL    GET_EXPRESSION
        JC      @@115
        MOV     ECX,EAX
        CMP     DWORD PTR LEX_TOKEN[EDI],']'
        JNE     @@130
        CALL    LEX     ;SKIP ']'
@@50:   MOV     EAX,DWORD PTR C_SIZE[EBX]
        OR      ECX,ECX
        JZ      @@81
        MUL     ECX 
@@81:   ADD     EAX,GVA_STRUCT_SIZE
        CALL    MALLOC
        XOR     EAX,EAX
        MOV     WORD PTR GVA_FLAGS[ESI],AX
        MOV     WORD PTR GVA_PARAM_COUNT[ESI],AX        ;This may never work
        MOV     DWORD PTR GVA_CLASS[ESI],EBX
        POP     DWORD PTR GVA_HASH_STRING[ESI]        ;+****
        MOV     DWORD PTR GVA_COUNT[ESI],ECX
        MOV     EBP,ESI
        MOV     ESI,GVA_HASH_STRING[EBP]
        MOV     DWORD PTR GVA_HASH_TYPE[EBP],HTT_GLOBAL_VARIABLE
        PUSH    EDI     ;-****
        MOV     EDI,DWORD PTR FS:TSS_HASH_TABLE
        CALL    FIND_HASH_BUCKET
        MOV     EAX,DWORD PTR HTE_NEXT[ESI]
        MOV     DWORD PTR GVA_HASH_NEXT[EBP],EAX
        MOV     DWORD PTR HTE_NEXT[ESI],EBP
        POP     EDI     ;+****
@@90:   CMP     DWORD PTR LEX_TOKEN[EDI],'='
        JNE     @@98
        CALL    LEX_POP
        CALL    LEX
        PUSH    EBX
        XOR     EBP,EBP
        MOV     EAX,3
        CALL    NEAR PTR PARSE_STATEMENT
        POP     EBX
        JMP     @@99
                        
@@98:   CALL    LEX_SUB_STACK_PTR       ;(GROWS UP)
@@99:   POP     EBX
        CMP     DWORD PTR LEX_TOKEN[EDI],','
        JE      @@2A
        CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@140
        CALL    LEX     ;SKIP ';'
        POP     EBP
        RET
@@115:  JMP     TRAP
@@120:  MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
@@130:  MOV     EAX,ST_MISSING_RIGHT_BRACKET
        JMP     TRAP
@@140:  MOV     EAX,ST_MISSING_SEMICOLON
        JMP     TRAP
PARSE_GLOBAL_LIST        ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_CLASS   PROC NEAR
        PUSH    EAX
        PUSH    EBP
        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@110
        CALL    NEW_CLASS
        MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR C_HASH_STRING[EBP],ESI
        MOV     DWORD PTR LEX_IDENT[EDI],0
        PUSH    EDI
        MOV     EDI,DWORD PTR FS:TSS_HASH_TABLE
        CALL    FIND_HASH_BUCKET
        POP     EDI
        MOV     EAX,DWORD PTR [ESI]
        MOV     DWORD PTR C_HASH_NEXT[EBP],EAX
        MOV     DWORD PTR [ESI],EBP
        CALL    LEX     ;SKIP NAME
        MOV     EAX,2
        CALL    PARSE_VAR_LIST
@@100:  POP     EBP
        POP     EAX
        RET
@@110:  MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
PARSE_CLASS   ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_FUN_COMMON PROC NEAR
@@3:    CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JE      @@5
        MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
@@5:    MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        CALL    LEX     ;SKIP NAME
PARSE_FUN_COMMON ENDP
PARSE_FUN_COMMON2 PROC NEAR
        CALL    LEX_SUB_STACK_PTR
        CALL    NEW_CLASS
        MOV     DWORD PTR C_RETURN_CLASS[EBP],EBX
        MOV     DWORD PTR C_HASH_TYPE[EBP],HTT_FUNCTION
        MOV     DWORD PTR C_HASH_STRING[EBP],ESI
        PUSH    EDI
        MOV     EDI,DWORD PTR FS:TSS_HASH_TABLE
        CALL    FIND_HASH_BUCKET
        POP     EDI
        MOV     EAX,DWORD PTR [ESI]
        MOV     DWORD PTR C_HASH_NEXT[EBP],EAX
        MOV     DWORD PTR [ESI],EBP
        MOV     EAX,1
        CALL    PARSE_VAR_LIST
        MOV     AX,WORD PTR C_MEM_COUNT[EBP]
        MOV     WORD PTR C_PARAM_COUNT[EBP],AX
        MOV     EDX,DWORD PTR C_MEM_LIST[EBP]
        MOV     EBX,DWORD PTR C_SIZE[EBP]
        ADD     EBX,SF_EIP
@@10:   OR      EDX,EDX
        JZ      @@20
        MOV     EAX,EBX
        SUB     EAX,DWORD PTR ML_OFFSET[EDX]
        MOV     DWORD PTR ML_OFFSET[EDX],EAX
        MOV     EDX,DWORD PTR ML_NEXT[EDX]
        JMP     @@10
@@20:   MOV     DWORD PTR C_SIZE[EBP],0
        RET
@@110:  MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
PARSE_FUN_COMMON2 ENDP

;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_FUN     PROC NEAR
        CALL    PARSE_FUN_COMMON2
        MOV     DWORD PTR LEX_LOCAL_VAR_LIST[EDI],EBP

        CALL    LEX_CB_PUSH
        CALL    INIT_CB_OUT_PTRS
        MOV     EAX,EC_ENTER
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX
        CALL    NEAR PTR PARSE_STATEMENT
        JC      TRAP
        MOV     EAX,EC_LEAVE
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    FIXUP_CODE
        MOV     ECX,DWORD PTR FS:TSS_OPTIMIZATION
        CALL    COMPILE_CODE
        PUSH    ESI
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[EDI]
        CALL    FREE
        MOV     ESI,DWORD PTR LEX_CB_LABEL_LIST[EDI]
        CALL    DELETE_LINKED_LIST
        POP     ESI
        CALL    LEX_CB_POP
        MOV     EAX,DWORD PTR C_SIZE[EBP]
        SUB     EAX,3
        AND     EAX,-4
        MOV     DWORD PTR C_SIZE[EBP],EAX

        MOV     DWORD PTR LEX_LOCAL_VAR_LIST[EDI],0
        MOV     DWORD PTR C_STATEMENTS[EBP],ESI
        POP     EBX
        POP     EBP
        RET
PARSE_FUN     ENDP

;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_SYSFUN     PROC NEAR
        CALL    LEX     ;SKIP "sysfun"
        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@110
        MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@130
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_SYS_SYMBOL
        JNE     @@130
        PUSH    DWORD PTR HTE_USER0[ESI]
        CALL    LEX     ;SKIP SYMBOL NAME
        CALL    PARSE_FUN_COMMON
        POP     DWORD PTR C_STATEMENTS[EBP]
        POP     EBX
        POP     EBP
        RET
@@110:  MOV     EAX,ST_EXPECTING_IDENTIFIER
        JMP     TRAP
@@130:  MOV     EAX,ST_EXPECTING_SYS_SYMBOL
        JMP     TRAP
PARSE_SYSFUN     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_INTERNAL PROC NEAR
        CALL    LEX     ;SKIP "internal"
        CALL    GET_EXPRESSION
        JC      TRAP
        PUSH    EAX
        CALL    PARSE_FUN_COMMON
        POP     DWORD PTR C_STATEMENTS[EBP]
        BTS     DWORD PTR C_FLAGS[EBP],CF_INTERNAL
        POP     EBX
        POP     EBP
        RET
PARSE_INTERNAL ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_IF     PROC NEAR
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@120
        CALL    LEX     ;SKIP (
        CALL    CONT_PARSE_EXPRESSION
        JC      @@110
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@130
        CALL    LEX     ;SKIP )
        CALL    NEW_LABEL
        PUSH    ESI             ;-****
        MOV     EAX,EC_SKIPZ
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX 
        CALL    NEAR PTR PARSE_STATEMENT
        JC      @@110
        POP     EDX             ;+****
        CALL    NEW_LABEL
        PUSH    ESI             ;-****
        MOV     EAX,EC_JMP
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_LABEL
        MOV     EBX,EDX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@50
        MOV     ESI,LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@50
        CMP     HTE_TYPE[ESI],HTT_KEYWORD
        JNE     @@50
        CMP     HTE_USER0[ESI],KW_ELSE
        JNE     @@50
        CALL    LEX     ;SKIP "ELSE"
        XOR     EAX,EAX
        CALL    NEAR PTR PARSE_STATEMENT
        JC      @@110
@@50:   MOV     EAX,EC_LABEL
        POP     EBX             ;+****
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
@@100:  RET
@@110:  JMP     TRAP
@@120:  MOV     EAX,ST_EXPECTING_LEFT_PAREN
        JMP     TRAP
@@130:  MOV     EAX,ST_MISSING_RIGHT_PAREN
        JMP     TRAP
PARSE_IF     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_WHILE     PROC NEAR
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@120
        CALL    NEW_LABEL
        MOV     EAX,EC_LABEL
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        PUSH    ESI     ;-****
        CALL    NEW_LABEL
        PUSH    ESI     ;-****
        CALL    LEX     ;SKIP (
        CALL    CONT_PARSE_EXPRESSION
        JC      @@110
        POP     ESI     ;+****
        POP     EDX     ;+****
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_SKIPZ
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@130
        CALL    LEX     ;SKIP )
        PUSH    ESI     ;-****
        PUSH    EDX     ;-****
        XOR     EAX,EAX
        CALL    NEAR PTR PARSE_STATEMENT
        JC      @@110
        MOV     EAX,EC_JMP
        POP     EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_LABEL
        POP     EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
@@100:  RET
@@110:  JMP     TRAP
@@120:  MOV     EAX,ST_EXPECTING_LEFT_PAREN
        JMP     TRAP
@@130:  MOV     EAX,ST_MISSING_RIGHT_PAREN
        JMP     TRAP
PARSE_WHILE     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
PARSE_FOR     PROC NEAR
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@120
        CALL    LEX     ;SKIP (
        XOR     EAX,EAX
        CALL    NEAR PTR PARSE_STATEMENT    
        CALL    NEW_LABEL
        MOV     EAX,EC_LABEL
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        PUSH    ESI     ;-****
        CALL    NEW_LABEL
        PUSH    ESI     ;-****
        CALL    CONT_PARSE_EXPRESSION
        JC      @@140
        POP     ESI     ;+****
        POP     EDX     ;+****
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_SKIPZ
        MOV     EBX,ESI
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        PUSH    ESI     ;-****
        PUSH    EDX     ;-****

        CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@140
        CALL    LEX     ;SKIP ;
        CALL    LEX_CB_PUSH
        CALL    INIT_CB_OUT_PTRS
        MOV     EAX,1
        CALL    NEAR PTR PARSE_STATEMENT
        XOR     EAX,EAX
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        CALL    LEX_CB_POP_NO_FREE
        PUSH    ESI     ;-****
        CALL    LEX_CB_POP

        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@130
        CALL    LEX     ;SKIP )
        XOR     EAX,EAX
        CALL    NEAR PTR PARSE_STATEMENT
        JC      @@110
        POP     ESI     ;+****
        CALL    LEX_CB_APPEND
        MOV     EAX,EC_JMP
        POP     EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_LABEL
        POP     EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
@@100:  RET
@@110:  JMP     TRAP
@@120:  MOV     EAX,ST_EXPECTING_LEFT_PAREN
        JMP     TRAP
@@130:  MOV     EAX,ST_MISSING_RIGHT_PAREN
        JMP     TRAP
@@140:  MOV     EAX,ST_MISSING_SEMICOLON
        JMP     TRAP
PARSE_FOR     ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
;       EBP=LOCAL VARIABLE CLASS OR NULL
;       EAX=FLAGS, 1=DON'T CHECK FOR ; 2=STOP ON ','
; OUT:  ESI=STATEMENT NODE
        PUBLIC  PARSE_STATEMENT
        ALIGN   4
PARSE_STATEMENT PROC NEAR
        PUSH    EBP
        PUSH    EAX
@@1A:   CMP     DWORD PTR LEX_TOKEN[EDI],'{'
        JNE     @@5
        CALL    LEX     ;SKIP {
        XOR     EAX,EAX
@@1:    CALL    PARSE_STATEMENT
        JC      @@120
        CMP     DWORD PTR LEX_TOKEN[EDI],'}'
        JNE     @@1
        CALL    LEX     ;SKIP {
        JMP     @@100

@@5:    CMP     DWORD PTR LEX_TOKEN[EDI],'?'
        JNE     @@6
        CALL    LEX     ;SKIP ?
        MOV     EDX,1   ;PARAM COUNT
@@5B:   PUSH    EDX     ;-****
        CALL    CONT_PARSE_EXPRESSION
        JC      @@120
        MOV     EAX,EC_TYPE
        MOV     EBX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     EBX,DWORD PTR IC_CLASS[EBX]
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
        CALL    ADD_INTERMEDIATE_CODE
        POP     EDX     ;+****
        CMP     DWORD PTR LEX_TOKEN[EDI],','
        JNE     @@5A
        CALL    LEX     ;SKIP ,
        INC     EDX
        JMP     @@5B
@@5A:   MOV     EAX,EC_INT32
        MOV     EBX,EDX 
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_PRINTF
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,EDX
        SHL     EBX,1   ;CLASS+VALUE
        INC     EBX     ;PARAM COUNT
        SHL     EBX,1
        SHL     EBX,1
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@90

@@6:    CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@10
        POP     EAX
        PUSH    EAX
        TEST    EAX,1
        JNZ     @@100
        CALL    LEX     ;SKIP ;
        JMP     @@100
        
@@10:   CMP     DWORD PTR LEX_TOKEN[EDI],TK_EOF
        JE      @@100
        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@55
        MOV     EBX,LEX_HASH_ENTRY[EDI]
        OR      EBX,EBX
        JE      @@55
        CMP     HTE_TYPE[EBX],HTT_KEYWORD
        JNE     @@50
@@20:   CMP     HTE_USER0[EBX],KW_CLASS
        JNE     @@30
        CALL    LEX     ;SKIP IDENT
        CALL    PARSE_CLASS
        JMP     @@1A
@@30:   CMP     HTE_USER0[EBX],KW_IF
        JNE     @@46
        CALL    LEX     ;SKIP IDENT
        CALL    PARSE_IF
        JMP     @@100
@@46:   CMP     HTE_USER0[EBX],KW_WHILE
        JNE     @@47
        CALL    LEX     ;SKIP IDENT
        CALL    PARSE_WHILE
        JMP     @@100
@@47:   CMP     HTE_USER0[EBX],KW_FOR
        JNE     @@49
        CALL    LEX     ;SKIP IDENT
        CALL    PARSE_FOR
        JMP     @@100
@@49:   CMP     HTE_USER0[EBX],KW_RETURN
        JNE     @@55
        CALL    LEX     ;SKIP IDENT
        CMP     EAX,';'
        JE      @@49A
        CALL    CONT_PARSE_EXPRESSION
        JC      @@120
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
@@49A:  OR      EBP,EBP
        JZ      @@49B
        MOV     EAX,EC_LEAVE
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@90
@@49B:  MOV     EAX,EC_RET
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@90

@@50:   TEST    DWORD PTR HTE_TYPE[EBX],HTT_CLASS+HTT_INTERNAL_TYPE
        JZ      @@55
        OR      EBP,EBP
        JZ      @@53
        CALL    PARSE_LOCAL_VAR_LIST
        JMP     @@1A
@@53:   CALL    PARSE_GLOBAL_LIST
        JMP     @@100
@@55:   CALL    CONT_PARSE_EXPRESSION
        JC      @@120
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE

@@90:   POP     EAX
        PUSH    EAX
        CMP     DWORD PTR LEX_TOKEN[EDI],','
        JNE     @@91
        TEST    EAX,2
        JNZ     @@100
        CALL    LEX     ;SKIP ','
        JMP     @@1A
@@91:   TEST    EAX,1
        JNZ     @@100
        CMP     DWORD PTR LEX_TOKEN[EDI],';'
        JNE     @@150
        CALL    LEX     ;SKIP ';'

@@100:  CLC
        POP     EAX
        POP     EBP
        RET


@@110:  MOV     EAX,ST_MISSING_RIGHT_BRACE
@@120:  JMP     TRAP
@@130:  MOV     EAX,ST_INVALID_LVALUE
        JMP     TRAP
@@150:  MOV     EAX,ST_MISSING_SEMICOLON
        JMP     TRAP
PARSE_STATEMENT ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX_NODE
; OUT:  ESI=STATEMENT CODE
;       ECX=CODE LENGTH
        PUBLIC  COMPILE_STATEMENT
        ALIGN   4
COMPILE_STATEMENT PROC NEAR
        PUSH    EBX
        LEA     EBX,PARSE_ERROR
        MOV     ECX,ESP
        ADD     ECX,4
        CALL    REGISTER_TRAP
        PUSH    EDX
        PUSH    EDI
        PUSH    EBP

        CALL    INIT_CB_OUT_PTRS
        XOR     EAX,EAX
        CALL    PARSE_STATEMENT

        MOV     EAX,EC_RET
        XOR     EBX,EBX
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE

        CALL    FIXUP_CODE
        MOV     ECX,DWORD PTR FS:TSS_OPTIMIZATION
        CALL    COMPILE_CODE
        PUSH    ESI
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[EDI]
        CALL    FREE
        MOV     ESI,DWORD PTR LEX_CB_LABEL_LIST[EDI]
        CALL    DELETE_LINKED_LIST
        POP     ESI

@@100:  CALL    END_TRAP
        CLC
        POP     EBP
        POP     EDI
        POP     EDX
        POP     EBX
        RET
COMPILE_STATEMENT ENDP


        ENDS
        END
