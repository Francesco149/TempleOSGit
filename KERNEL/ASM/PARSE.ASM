        INCLUDE OSINC.INC                
        INCLUDE COMPILE.INC
        EXTRN   PUT_STRING              :NEAR
        EXTRN   SPACE                   :NEAR
        EXTRN   OPTIMIZE_CODE           :NEAR
        EXTRN   LEX                     :NEAR
        EXTRN   MALLOC                  :NEAR
        EXTRN   FREE                    :NEAR
        EXTRN   SPACES                  :NEAR
        EXTRN   PUT_HEX                 :NEAR
        EXTRN   CRLF                    :NEAR
        EXTRN   DEBUG_SPIN_WAIT         :NEAR
        EXTRN   PUT_DEC                 :NEAR
        EXTRN   PUT_HEX                 :NEAR
        EXTRN   PUT_HEX_WORD            :NEAR
        EXTRN   SYS_INTERNAL_TYPES      :DWORD
        EXTRN   FIND_MEMBER             :NEAR
        EXTRN   PUT_EXPRESSION          :NEAR
        EXTRN   REGISTER_TRAP           :NEAR
        EXTRN   END_TRAP                :NEAR
        EXTRN   TRAP                    :NEAR
        EXTRN   PUT_TOKEN               :NEAR
        EXTRN   COMPILE_CODE            :NEAR
        EXTRN   DELETE_LINKED_LIST      :NEAR
        EXTRN   HEX_DUMP                :NEAR
        EXTRN   ADD_INTERMEDIATE_CODE   :NEAR
        EXTRN   INIT_CB_OUT_PTRS        :NEAR
        EXTRN   FIXUP_CODE              :NEAR
        EXTRN   LEX_CB_PUSH             :NEAR
        EXTRN   LEX_CB_POP              :NEAR
        EXTRN   GET_KEY                 :NEAR
        EXTRN   ZEROS                   :DWORD
        EXTRN   LEX_CB_APPEND           :NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

        ALIGN   4
BINARY_OPS_TABLE:
        DW      000h,000h,000h,000h,000h,509h,506h,508h ;00
        DW      603h,602h,000h,329h,32Ah,000h,000h,507h ;08
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;10
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;18
        DW      000h,000h,000h,000h,000h,30Eh,328h,000h ;20
        DW      000h,000h,30Ch,40Ah,000h,40Bh,000h,30Dh ;28
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;30
        DW      000h,000h,000h,000h,504h,722h,505h,000h ;38
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;40
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;48
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;50
        DW      000h,000h,000h,000h,000h,000h,601h,000h ;58
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;60
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;68
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;70
        DW      000h,000h,000h,000h,327h,000h,000h,000h ;78
UNARY_OPS_TABLE:
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;00
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;08
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;10
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;18
        DW      000h,110h,000h,000h,000h,000h,13Eh,000h ;20
        DW      000h,000h,114h,000h,000h,10Fh,000h,000h ;28
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;30
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;38
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;40
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;48
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;50
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;58
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;60
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;68
        DW      000h,000h,000h,000h,000h,000h,000h,000h ;70
        DW      000h,000h,000h,000h,000h,000h,12Bh,000h ;78

DEREF_TABLE:    ;DEREFERENCE OPERATORS BITMAP
        DD      000100000h,03FFF0000h,03FF00000h,000000000h

PREINC_TABLE:
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,54h,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      56h,58h,55h,57h,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
PREDEC_TABLE:
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,59h,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      5Bh,5Dh,5Ah,5Ch,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
POSTINC_TABLE:
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,34h,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      36h,38h,35h,37h,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
POSTDEC_TABLE:
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,39h,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      3Bh,3Dh,3Ah,3Ch,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        DB      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
;       EAX = NUMBER
        PUBLIC  PARSE_ERROR
        ALIGN   4
PARSE_ERROR PROC NEAR
        PUSH    EAX
        MOV     EAX,ST_PARSE_ERROR
        INT     I_PUT_SYSTEXT
        POP     EAX
        INT     I_PUT_SYSTEXT
        CALL    PUT_TOKEN
        CALL    CRLF
        PUSH    EDI
        MOV     EDI,DWORD PTR LEX_CUR_LFN[EDI]
        MOV     ESI,DWORD PTR LFN_NAME[EDI]
        CALL    PUT_STRING
        MOV     EAX,','
        INT     I_PUT_CHAR
        MOV     EAX,LFN_LINE_NUM[EDI]
        CALL    PUT_DEC
        MOV     EAX,':'
        INT     I_PUT_CHAR
        MOV     ESI,LFN_CUR_LINE_START[EDI]
@@1:    LODSB   
        OR      AL,AL
        JZ      @@2
        CMP     AL,10
        JE      @@2
        INT     I_PUT_CHAR
        JMP     @@1
@@2:    CALL    CRLF
        POP     EDI
        LEA     ESI,DWORD PTR LEX_LFNS[EDI]
        MOV     DWORD PTR LEX_CUR_LFN[EDI],ESI
        LEA     ESI,ZEROS
        MOV     DWORD PTR LEX_CUR_BUF_PTR[EDI],ESI
        MOV     DWORD PTR LEX_LFNS+LFN_BUF_PTR[EDI],ESI
        LEA     ESI,DWORD PTR LEX_STACK[EDI]
        MOV     DWORD PTR LEX_STACK_PTR[EDI],ESI
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_USE_LAST_CHAR
        MOV     DWORD PTR LEX_TOKEN[EDI],TK_EOF
        CALL    LEX
        STC
        RET
PARSE_ERROR ENDP
;;**************************PROCEDURE*************************
; IN:   ESI = EXPRESSION
        PUBLIC  DELETE_EXPRESSION
        ALIGN   4
DELETE_EXPRESSION PROC NEAR
        JMP     FREE
DELETE_EXPRESSION ENDP

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
        ALIGN   4
        PUBLIC  CONT_PARSE_EXPRESSION
CONT_PARSE_EXPRESSION PROC NEAR
        XOR     EAX,EAX         ;TERMINATE OP STACK
        PUSH    EAX

UNARY_TERM:
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_PREINC-LF_PREDEC-LF_POSTINC-LF_POSTDEC-LF_FUNCTION
UNARY_TERM2:
        MOV     EAX,DWORD PTR LEX_TOKEN[EDI]
        CMP     EAX,TK_INTEGER
        JNE     @@1
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR LEX_CUR_I[EDI]
        OR      EBX,EBX
        JS      @@1A
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_int)
        JMP     @@1B
@@1A:   MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
@@1B:   CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP INT                
        JMP     CHECK_FOR_BINARY_OP

@@1:    CMP     EAX,TK_STRING
        JNE     @@2
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR LEX_IDENT[EDI]
        MOV     DWORD PTR LEX_IDENT[EDI],0
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_char)
        ADD     ECX,C_STRUCT_SIZE
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP INT                
        JMP     CHECK_FOR_BINARY_OP

@@2:    CMP     EAX,'('
        JNE     @@4
        CALL    LEX     ;SKIP (
        CALL    CONT_PARSE_EXPRESSION
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@3
        CALL    LEX     ;SKIP )
        OR      DWORD PTR LEX_FLAGS[EDI],LF_EAX
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_ARRAY-LF_FUNCTION
        JMP     UNARY_MODIFIERS
@@3:    MOV     EAX,ST_MISSING_RIGHT_PAREN
        JMP     TRAP

@@4:    MOVZX   EDX,WORD PTR UNARY_OPS_TABLE[EAX*2]
        OR      DH,DH
        JZ      @@5
        CALL    LEX     ;SKIP OP
        CMP     DL,EC_ADDRESS           ;CHECK FOR &FUNCTION
        JNE     @@4A
        CMP     EAX,TK_IDENT
        JNE     @@4A
        MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@4A
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_FUNCTION
        JNE     @@4A
        BT      DWORD PTR C_FLAGS[ESI],CF_INTERNAL
        JC      @@4A
        CALL    LEX     ;SKIP FUNCTION NAME
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR C_STATEMENTS[ESI]
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
        CALL    ADD_INTERMEDIATE_CODE
        JMP     CHECK_FOR_BINARY_OP
          
@@4A:   PUSH    EDX
        JMP     UNARY_TERM2

@@5:    CMP     EAX,TK_IDENT
        JE      @@6
        CMP     EAX,TK_PLUS_PLUS
        JNE     @@5B
        CALL    LEX ;SKIP ++
        OR      DWORD PTR LEX_FLAGS[EDI],LF_PREINC
        JMP     UNARY_TERM2      
@@5B:   CMP     EAX,TK_MINUS_MINUS
        JNE     @@5A
        CALL    LEX ;SKIP --
        OR      DWORD PTR LEX_FLAGS[EDI],LF_PREDEC
        JMP     UNARY_TERM2      
@@5A:   MOV     EAX,ST_MISSING_EXPRESSION
        JMP     TRAP
@@6:    MOV     EDX,LEX_LOCAL_VAR_ENTRY[EDI]
        OR      EDX,EDX
        JZ      @@10
        XOR     EBX,EBX
        MOV     ECX,DWORD PTR ML_CLASS[EDX]
        ADD     ECX,C_STRUCT_SIZE  
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_EAX-LF_ARRAY-LF_FUNCTION
        TEST    WORD PTR ML_FLAGS[EDX],MLF_FUNCTION
        JZ      @@8
        OR      DWORD PTR LEX_FLAGS[EDI],LF_FUNCTION
        MOVZX   EAX,WORD PTR ML_PARAM_COUNT[EDX]
        PUSH    EAX             ;-****
@@8:    MOV     EAX,ML_COUNT[EDX]
        OR      EAX,EAX
        JZ      @@9
        OR      DWORD PTR LEX_FLAGS[EDI],LF_ARRAY
        ADD     ECX,C_STRUCT_SIZE  
@@9:    MOV     EAX,EC_LOCAL_FRAME_PTR    
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR ML_OFFSET[EDX]
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_ADDITION
        XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP VARIABLE NAME
        JMP     UNARY_MODIFIERS

@@10:   MOV     ESI,LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JNZ     @@10B
        MOV     EAX,ST_INVALID_LVALUE
        JMP     TRAP

@@10B:  CMP     DWORD PTR HTE_TYPE[ESI],HTT_SYS_SYMBOL
        JNE     @@10A
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR HTE_USER0[ESI]
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP SYMBOL                
        JMP     CHECK_FOR_BINARY_OP

@@10A:  CMP     DWORD PTR HTE_TYPE[ESI],HTT_FUNCTION
        JNE     @@15
        PUSH    0               ;VARIABLE PARAM COUNT
        PUSH    0               ;VARIABLE PARAM LOC
        CALL    LEX     ;SKIP FUNCTION NAME
        CALL    LEX_CB_PUSH
        PUSH    DWORD PTR LEX_CB_STACK[EDI]
        MOV     DWORD PTR LEX_CB_STACK[EDI],0
        MOVZX   ECX,WORD PTR C_PARAM_COUNT[ESI]
        OR      ECX,ECX
        JNZ     @@11B
        MOV     EBX,DWORD PTR C_MEM_LIST[ESI]
        OR      EBX,EBX
        JZ      @@13C
        TEST    WORD PTR ML_FLAGS[EBX],MLF_DOT_DOT_DOT
        JZ      @@13C
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JE      @@13B
        PUSH    EBX             ;ARGC=0 
        PUSH    ECX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        MOV     EAX,EC_INT32
        MOV     ECX,DWORD PTR ML_CLASS[EBX]
        MOV     EBX,0
        CALL    ADD_INTERMEDIATE_CODE
        MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ESI,ESP
        MOV     16[ESI],ECX
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        POP     ESI
        POP     ECX
        POP     EBX
        JMP     @@14

@@13C:  CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@14
        CALL    LEX ;SKIP (
        JMP     @@14B

@@11B:  MOV     EBX,DWORD PTR C_MEM_LIST[ESI]
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JE      @@11
        TEST    WORD PTR ML_FLAGS[EBX],MLF_DEFAULT_AVAILABLE
        JNZ     @@13A    
@@11A:  MOV     EAX,ST_EXPECTING_LEFT_PAREN
        JMP     TRAP
@@11:   CALL    LEX     ;SKIP (
        JMP     @@13
@@12:   CMP     DWORD PTR LEX_TOKEN[EDI],','
        JE      @@12A
        TEST    WORD PTR ML_FLAGS[EBX],MLF_DEFAULT_AVAILABLE
        JNZ     @@13A    
@@6F:   MOV     EAX,ST_EXPECTING_COMMA
        JMP     TRAP

@@12A:  CALL    LEX     ;SKIP ,
@@13:   PUSH    EBX
        PUSH    ECX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        CALL    CONT_PARSE_EXPRESSION
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        POP     ESI
        POP     ECX
        POP     EBX
        MOV     EBX,DWORD PTR ML_NEXT[EBX]
        LOOP    @@12
        OR      EBX,EBX
        JZ      @@14B
        TEST    WORD PTR ML_FLAGS[EBX],MLF_DOT_DOT_DOT
        JZ      @@14B
        CMP     DWORD PTR LEX_TOKEN[EDI],','
        JE      @@13B
        JMP     @@13H

@@13B:  CALL    LEX             ;skip '('
@@13H:  PUSH    EBX             ;ARGC=0 
        PUSH    ECX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        MOV     EAX,EC_INT32
        MOV     ECX,DWORD PTR ML_CLASS[EBX]
        MOV     EBX,0
        CALL    ADD_INTERMEDIATE_CODE
        MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ESI,ESP
        MOV     16[ESI],ECX
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        POP     ESI
        POP     ECX
        POP     EBX
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JE      @@14B
        JMP     @@13E

@@13D:  CMP     DWORD PTR LEX_TOKEN[EDI],','
        JNE     @@14B

        CALL    LEX     ;SKIP ,
@@13E:  PUSH    EBX
        PUSH    ECX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        CALL    CONT_PARSE_EXPRESSION
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        MOV     ESI,ESP
        INC     DWORD PTR 20[ESI]
        POP     ESI
        POP     ECX
        POP     EBX
        JMP     @@13D


@@14B:  CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@3
        CALL    LEX     ;SKIP )
        JMP     @@14

@@13A:  PUSH    EBX             ;LOAD DEFAULT PARAMS
        PUSH    ECX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        MOV     EAX,EC_INT32
        MOV     ECX,DWORD PTR ML_CLASS[EBX]
        MOV     EBX,DWORD PTR ML_DEFAULT[EBX]
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        POP     ESI
        POP     ECX
        POP     EBX
        MOV     EBX,DWORD PTR ML_NEXT[EBX]
        LOOP    @@13A
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@14
        CALL    LEX     ;SKIP )

@@14:   POP     EAX     
        POP     ECX     ;ADDRESS OF ARGC
        POP     EBX     ;ARGC VALUE
        OR      ECX,ECX
        JZ      @@14H
        MOV     DWORD PTR IC_DATA[ECX],EBX
        INC     EBX
@@14H:  PUSH    EBX
        PUSH    ESI
        MOV     ESI,EAX
        PUSH    DWORD PTR LEX_CB_STACK[ESI]
        MOV     EAX,DWORD PTR LEX_CB_STACK[EDI]
        MOV     DWORD PTR LEX_CB_STACK[ESI],EAX
        MOV     DWORD PTR LEX_CB_STACK[EDI],ESI
        CALL    LEX_CB_POP
        MOV     ESI,DWORD PTR LEX_CB_STACK[EDI]
        POP     DWORD PTR LEX_CB_STACK[EDI]
@@14E:  OR      ESI,ESI
        JZ      @@14F
        PUSH    DWORD PTR LEX_CB_STACK[ESI]
        CALL    LEX_CB_APPEND
        POP     ESI
        JMP     @@14E
@@14F:  POP     ESI
        BT      DWORD PTR C_FLAGS[ESI],CF_INTERNAL
        JNC     @@14A
        MOV     EAX,DWORD PTR C_STATEMENTS[ESI]
        XOR     EBX,EBX
        JMP     @@14C
@@14A:  MOV     EAX,EC_CALL
        MOV     EBX,DWORD PTR C_STATEMENTS[ESI]
@@14C:  MOV     ECX,C_RETURN_CLASS[ESI]
        CALL    ADD_INTERMEDIATE_CODE
        POP     EBX
@@14G:  BT      DWORD PTR C_FLAGS[ESI],CF_INTERNAL
        JC      @@14D
        MOVZX   EAX,WORD PTR C_PARAM_COUNT[ESI]
        ADD     EBX,EAX
        MOV     EAX,EC_ADD_ESP
        SHL     EBX,1        
        SHL     EBX,1        
        CALL    ADD_INTERMEDIATE_CODE
@@14D:  OR      DWORD PTR LEX_FLAGS[EDI],LF_EAX
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_ARRAY-LF_FUNCTION
        JMP     UNARY_MODIFIERS

@@15:   TEST    DWORD PTR HTE_TYPE[ESI],HTT_GLOBAL_VARIABLE
        JZ      @@21
        LEA     EBX,GVA_DATA[ESI]
        MOV     ECX,GVA_CLASS[ESI]
        ADD     ECX,C_STRUCT_SIZE
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_EAX-LF_ARRAY-LF_FUNCTION
        MOV     EAX,GVA_COUNT[ESI]
        OR      EAX,EAX
        JZ      @@15A
        ADD     ECX,C_STRUCT_SIZE
        OR      DWORD PTR LEX_FLAGS[EDI],LF_ARRAY
@@15A:  TEST    WORD PTR GVA_FLAGS[ESI],GVAF_FUNCTION
        JZ      @@15B
        OR      DWORD PTR LEX_FLAGS[EDI],LF_FUNCTION
        MOVZX   EAX,WORD PTR GVA_PARAM_COUNT[ESI]
        PUSH    EAX                     ;-****
@@15B:  MOV     EAX,EC_INT32    
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP VARIABLE NAME           
        JMP     UNARY_MODIFIERS

@@21:   CMP     DWORD PTR HTE_TYPE[ESI],HTT_KEYWORD
        JNE     @@5A
        CMP     DWORD PTR HTE_USER0[ESI],KW_SIZEOF
        JNE     @@5A
        CALL    LEX     ;SKIP "sizeof"
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@11A
        CALL    LEX     ;SKIP '('
        CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JE      @@22
@@21A:  MOV     EAX,ST_INVALID_CLASS
        JMP     TRAP
@@22:   MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@21A
        AND     DWORD PTR HTE_TYPE[ESI],HTT_CLASS+HTT_INTERNAL_TYPE
        JZ      @@21A
        MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR C_SIZE[ESI]
        MOV     ECX,DWORD PTR SYS_INTERNAL_TYPES+(4*IT_DWORD)
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX     ;SKIP CLASS
        CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@3
        CALL    LEX     ;SKIP ')'
        JMP     CHECK_FOR_BINARY_OP

UNARY_MODIFIERS:
        CMP     DWORD PTR LEX_TOKEN[EDI],'.'
        JNE     @@20
@@16:   MOV     EBP,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     EBP,DWORD PTR IC_CLASS[EBP]
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_EAX
        JNZ     @@16A
        SUB     EBP,C_STRUCT_SIZE
@@16A:  CMP     DWORD PTR HTE_TYPE[EBP],HTT_CLASS
        JE      @@17
        MOV     EAX,ST_INVALID_CLASS
        JMP     TRAP 
@@17:   CALL    LEX     ;SKIP '.' 
        CMP     EAX,TK_IDENT
        JE      @@18
@@17A:  MOV     EAX,ST_INVALID_MEMBER
        JMP     TRAP
@@18:   MOV     ESI,DWORD PTR LEX_IDENT[EDI]
        CALL    FIND_MEMBER
        OR      EDX,EDX
        JZ      @@17A
        CALL    LEX     ;SKIP MEMBER NAME       
        MOV     EAX,EC_INT32
        MOV     EBX,ML_OFFSET[EDX]
        MOV     ECX,ML_CLASS[EDX]
        ADD     ECX,C_STRUCT_SIZE
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EBX,EBX
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_EAX-LF_ARRAY-LF_FUNCTION
        MOV     EAX,ML_COUNT[EDX]
        OR      EAX,EAX
        JZ      @@18A
        ADD     ECX,C_STRUCT_SIZE
        OR      DWORD PTR LEX_FLAGS[EDI],LF_ARRAY
@@18A:  TEST    WORD PTR ML_FLAGS[EDX],MLF_FUNCTION
        JZ      @@18B
        MOVZX   EAX,WORD PTR ML_PARAM_COUNT[EDX]
        PUSH    EAX
        OR      DWORD PTR LEX_FLAGS[EDI],LF_FUNCTION
@@18B:  MOV     EAX,EC_ADDITION
        CALL    ADD_INTERMEDIATE_CODE   
        JMP     UNARY_MODIFIERS      

@@20:   CMP     DWORD PTR LEX_TOKEN[EDI],'['
        JNE     @@30
        CALL    LEX     ;SKIP [
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_ARRAY+LF_EAX
        JNZ     @@20B
        MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ECX]
        SUB     ECX,C_STRUCT_SIZE
        MOVZX   EAX,WORD PTR C_DEREFERENCE[ECX]
        XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@20A
@@20B:  MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ECX]
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_EAX
        JNZ     @@20A
        SUB     ECX,C_STRUCT_SIZE
@@20A:  MOV     EAX,EC_INT32
        MOV     EBX,DWORD PTR C_SIZE-C_STRUCT_SIZE[ECX]
        CALL    ADD_INTERMEDIATE_CODE
        PUSH    ECX
        CALL    CONT_PARSE_EXPRESSION
        POP     ECX
        CMP     DWORD PTR LEX_TOKEN[EDI],']'
        JE      @@25
        MOV     EAX,ST_MISSING_RIGHT_BRACKET
        JMP     TRAP
@@25:   CALL    LEX     ;SKIP ]
        XOR     EBX,EBX
        MOV     EAX,EC_MULTIPLICATION
        CALL    ADD_INTERMEDIATE_CODE   
        MOV     EAX,EC_ADDITION
        CALL    ADD_INTERMEDIATE_CODE   
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_EAX-LF_ARRAY
        JMP     UNARY_MODIFIERS      

@@30:   CMP     DWORD PTR LEX_TOKEN[EDI],TK_DEREFERENCE
        JNE     @@32
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_EAX+LF_ARRAY
        JNZ     @@31
        MOV     EAX,EC_DEREFERENCE+0100h
        XOR     EBX,EBX
        MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ECX]
        SUB     ECX,C_STRUCT_SIZE
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@16
@@31:   MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     EAX,DWORD PTR IC_CLASS[ECX]
        SUB     EAX,C_STRUCT_SIZE
        MOV     DWORD PTR IC_CLASS[ECX],EAX
        JMP     @@16

@@32:   TEST    DWORD PTR LEX_FLAGS[EDI],LF_FUNCTION
        JZ      @@40
        CMP     DWORD PTR LEX_TOKEN[EDI],'('
        JNE     @@40
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_EAX+LF_ARRAY
        JNZ     @@33
        MOV     EAX,EC_DEREFERENCE+0100h
        XOR     EBX,EBX
        MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ECX]
        SUB     ECX,C_STRUCT_SIZE
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@34
@@33:   MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     EAX,DWORD PTR IC_CLASS[ECX]
        SUB     EAX,C_STRUCT_SIZE
        MOV     DWORD PTR IC_CLASS[ECX],EAX

@@34:   POP     EDX
        CALL    LEX ;SKIP (
        PUSH    ECX             ;-****
        CALL    LEX_CB_PUSH
        PUSH    DWORD PTR LEX_CB_STACK[EDI]
        MOV     DWORD PTR LEX_CB_STACK[EDI],0
        MOV     ECX,EDX
        OR      ECX,ECX
        JZ      @@6D
        JMP     @@6E
@@6G:   CMP     DWORD PTR LEX_TOKEN[EDI],','
        JNE     @@6F    ;EXPECTING ,
        CALL    LEX     ;SKIP ,
@@6E:   PUSH    ECX
        PUSH    EDX
        PUSH    ESI
        CALL    INIT_CB_OUT_PTRS
        CALL    CONT_PARSE_EXPRESSION
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE
        CALL    LEX_CB_PUSH
        POP     ESI
        POP     EDX
        POP     ECX
        LOOP    @@6G
@@6D:   CMP     DWORD PTR LEX_TOKEN[EDI],')'
        JNE     @@3
        CALL    LEX     ;SKIP )
@@6C:   POP     EAX
        PUSH    ESI
        MOV     ESI,EAX
        PUSH    DWORD PTR LEX_CB_STACK[ESI]
        MOV     EAX,DWORD PTR LEX_CB_STACK[EDI]
        MOV     DWORD PTR LEX_CB_STACK[ESI],EAX
        MOV     DWORD PTR LEX_CB_STACK[EDI],ESI
        CALL    LEX_CB_POP
        MOV     ESI,DWORD PTR LEX_CB_STACK[EDI]
        POP     DWORD PTR LEX_CB_STACK[EDI]
@@6Z:   OR      ESI,ESI
        JZ      @@6Y
        PUSH    DWORD PTR LEX_CB_STACK[ESI]
        CALL    LEX_CB_APPEND
        POP     ESI
        JMP     @@6Z
@@6Y:   POP     ESI
        MOV     EAX,EC_CALL_INDIRECT
        MOV     EBX,EDX
        SHL     EBX,1        
        SHL     EBX,1        
        POP     ECX             ;+****
        PUSH    EBX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_ADD_ESP
        POP     EBX
        ADD     EBX,4
        CALL    ADD_INTERMEDIATE_CODE
        OR      DWORD PTR LEX_FLAGS[EDI],LF_EAX
        AND     DWORD PTR LEX_FLAGS[EDI],-1-LF_ARRAY-LF_FUNCTION
        JMP     UNARY_MODIFIERS

@@40:   CMP     DWORD PTR LEX_TOKEN[EDI],TK_PLUS_PLUS
        JNE     @@45
        CALL    LEX ;SKIP ++
        OR      DWORD PTR LEX_FLAGS[EDI],LF_POSTINC
        JMP     DEREFERENCE_VARIABLE

@@45:   CMP     DWORD PTR LEX_TOKEN[EDI],TK_MINUS_MINUS
        JNE     DEREFERENCE_VARIABLE
        CALL    LEX ;SKIP --
        OR      DWORD PTR LEX_FLAGS[EDI],LF_POSTDEC

DEREFERENCE_VARIABLE:
        MOV     EDX,DWORD PTR LEX_FLAGS[EDI]
        TEST    EDX,LF_FUNCTION
        JZ      @@46E
        POP     EAX
@@46E:  TEST    EDX,LF_EAX+LF_ARRAY
        JZ      @@46
        TEST    EDX,LF_EAX
        JNZ     CHECK_FOR_BINARY_OP
        MOV     EAX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[EAX]
        SUB     ECX,C_STRUCT_SIZE
        MOV     DWORD PTR IC_CLASS[EAX],ECX
        JMP     CHECK_FOR_BINARY_OP
@@46:   MOV     ECX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ECX]
        SUB     ECX,C_STRUCT_SIZE
        MOVZX   EAX,BYTE PTR C_DEREFERENCE[ECX]
        TEST    EDX,LF_PREINC
        JZ      @@46A
        MOV     AL,BYTE PTR PREINC_TABLE[EAX]
        JMP     @@46D
@@46A:  TEST    EDX,LF_PREDEC
        JZ      @@46B
        MOV     AL,BYTE PTR PREDEC_TABLE[EAX]
        JMP     @@46D
@@46B:  TEST    EDX,LF_POSTINC
        JZ      @@46C
        MOV     AL,BYTE PTR POSTINC_TABLE[EAX]
        JMP     @@46D
@@46C:  TEST    EDX,LF_POSTDEC
        JZ      @@46D
        MOV     AL,BYTE PTR POSTDEC_TABLE[EAX]
@@46D:  XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE

CHECK_FOR_BINARY_OP:
        POP     EAX             ;POP UNARY OPS
        OR      EAX,EAX
        JZ      @@85
        CMP     AH,1
        JA      @@85
        MOV     ESI,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ESI]
        MOVZX   EAX,AL
        BT      DWORD PTR DEREF_TABLE,EAX
        JNC     @@83
        CMP     BYTE PTR C_PTR_COUNT[ECX],0
        JE      @@83
        SUB     ECX,C_STRUCT_SIZE
        MOVZX   EAX,WORD PTR C_DEREFERENCE[ECX]
        JMP     @@84
@@83:   CMP     EAX,EC_ADDRESS
        JNE     @@84
        TEST    DWORD PTR LEX_FLAGS[EDI],LF_EAX+LF_ARRAY
        JNZ     @@84
        MOVZX   EBX,BYTE PTR IC_CODE[ESI]
        BT      DWORD PTR DEREF_TABLE,EBX
        JC      @@83A
@@110:  MOV     EAX,ST_INVALID_LVALUE
        JMP     TRAP

@@83A:  MOV     IC_CODE[ESI],EC_NOP
        ADD     ECX,C_STRUCT_SIZE
@@84:   XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     CHECK_FOR_BINARY_OP
        
@@85:   PUSH    EAX
        MOV     EAX,DWORD PTR LEX_TOKEN[EDI]
        MOVZX   EDX,WORD PTR BINARY_OPS_TABLE[EAX*2]
        OR      DH,DH
        JZ      EXP_FINISHED
        CALL    LEX     ;SKIP OP
        CMP     DL,EC_ASSIGN32
        JNE     @@92

        MOV     EAX,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[EAX]
        MOVZX   EBX,BYTE PTR IC_CODE[EAX]
        BT      DWORD PTR DEREF_TABLE,EBX
        JNC     @@110
        MOV     DWORD PTR IC_CODE[EAX],EC_NOP
        MOVZX   EDX,WORD PTR C_ASSIGN[ECX]
@@92:   POP     EAX             ;POP OPS OF HIGHER PRECIDENCE
        OR      EAX,EAX
        JZ      @@91
        CMP     DH,AH
        JB      @@91
@@90:   MOV     ESI,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ESI]
        XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@92
@@91:   PUSH    EAX
        PUSH    EDX
        JMP     UNARY_TERM

CONT_PARSE_EXPRESSION ENDP
EXP_FINISHED PROC NEAR
@@100:  POP     EAX
        OR      EAX,EAX
        JZ      @@101
        MOV     ESI,DWORD PTR LEX_CB_LAST_OUT_PTR[EDI]
        MOV     ECX,DWORD PTR IC_CLASS[ESI]
        XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        JMP     @@100
@@101:  RET
@@110:  MOV     EAX,ST_INVALID_LVALUE
        JMP     TRAP
EXP_FINISHED ENDP

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  ESI = EXPRESSION NODE
        PUBLIC  COMPILE_EXPRESSION
        ALIGN   4
COMPILE_EXPRESSION PROC NEAR
        MOV     ECX,DWORD PTR FS:TSS_OPTIMIZATION
UNOPT_COMPILE_EXPRESSION:
        PUSHAD
        LEA     EBX,PARSE_ERROR
        MOV     ECX,ESP
        ADD     ECX,SP_STRUCT_SIZE
        CALL    REGISTER_TRAP
        
        CALL    INIT_CB_OUT_PTRS
        CALL    CONT_PARSE_EXPRESSION
        MOV     EAX,EC_ADD_ESP
        MOV     EBX,4
        XOR     ECX,ECX
        CALL    ADD_INTERMEDIATE_CODE
        MOV     EAX,EC_RET
        XOR     EBX,EBX
        CALL    ADD_INTERMEDIATE_CODE
        XOR     EAX,EAX
        CALL    ADD_INTERMEDIATE_CODE

        CALL    FIXUP_CODE
        CALL    END_TRAP
        MOV     EAX,ESP
        MOV     ECX,DWORD PTR SP_ECX[EAX]   ;GET OPTIMIZATION
        CALL    COMPILE_CODE
        PUSH    ESI
        MOV     ESI,DWORD PTR LEX_CB_OUT_FIRST[EDI]
        CALL    FREE
        MOV     ESI,DWORD PTR LEX_CB_LABEL_LIST[EDI]
        CALL    DELETE_LINKED_LIST
        POP     ESI
        MOV     EAX,ESP
        MOV     DWORD PTR SP_ESI[EAX],ESI
        POPAD
        CLC
        RET
COMPILE_EXPRESSION ENDP

;;**************************PROCEDURE*************************
; IN:   ESI = EXPRESSION
; OUT:  EAX = VALUE
;       FLAGS SET BY EAX
        PUBLIC  EVAL_EXPRESSION
        ALIGN   4
EVAL_EXPRESSION PROC NEAR
        PUSHAD
        CALL    ESI
        MOV     ESI,ESP
        MOV     DWORD PTR SP_EAX[ESI],EAX
        POPAD
        RET
EVAL_EXPRESSION ENDP
;;**************************PROCEDURE*************************
; IN:   ESI = STATEMENT
;       FLAGS SET BY EAX
        PUBLIC  RUN_STATEMENT
        ALIGN   4
RUN_STATEMENT PROC NEAR
        PUSHAD
        CALL    ESI
        MOV     EBP,ESP
        MOV     SP_EAX[EBP],EAX
        POPAD
        RET
RUN_STATEMENT ENDP
;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  EAX = VALUE
;       FLAGS SET BY EAX
        PUBLIC  GET_EXPRESSION
        ALIGN   4
GET_EXPRESSION PROC NEAR
        CALL    LEX_CB_PUSH
        PUSH    ESI
        PUSH    ECX
        XOR     ECX,ECX
        CALL    UNOPT_COMPILE_EXPRESSION
        JC      @@100
        CALL    EVAL_EXPRESSION
        CALL    DELETE_EXPRESSION
        CLC
@@100:  POP     ECX
        POP     ESI
        CALL    LEX_CB_POP
        OR      EAX,EAX        
        RET
GET_EXPRESSION ENDP
        ENDS
        END
