        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        INCLUDE COMPILE.INC
        EXTRN SYS_MEMORY_BASE:DWORD
        EXTRN INIT_CB_IN_PTRS:NEAR
        EXTRN GET_INTERMEDIATE_CODE:NEAR
        EXTRN GET_INTERMEDIATE_CODE2:NEAR
        EXTRN RUN_STATEMENT:NEAR

        EXTRN   OPTIMIZE_CODE   :NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE
        ALIGN   4
CODE_TABLE:
        DD      ECT_NULL
        DD      ECT_XOR
        DD      ECT_OR_OR 
        DD      ECT_AND_AND
        DD      ECT_LESS
        DD      ECT_GREATER
        DD      ECT_EQUAL_EQUAL
        DD      ECT_NOT_EQUAL_TO
        DD      ECT_GREATER_OR_EQUAL
        DD      ECT_LESS_OR_EQUAL
        DD      ECT_ADD
        DD      ECT_SUB
        DD      ECT_MUL
        DD      ECT_DIV
        DD      ECT_MOD
        DD      ECT_UNARY_MINUS
        DD      ECT_NOT
        DD      ECT_INT
        DD      ECT_DOUBLE
        DD      ECT_LOCAL_FRAME_PTR
        DD      ECT_DEREFERENCE
        DD      ECT_CALL
        DD      ECT_RET
        DD      ECT_INC
        DD      ECT_DEC
        DD      ECT_INT8
        DD      ECT_ADD32
        DD      ECT_SUB32
        DD      ECT_ADD8
        DD      ECT_SUB8
        DD      ECT_MUL32
        DD      ECT_DIV32
        DD      ECT_MUL8
        DD      ECT_DIV8
        DD      ECT_ASSIGN32
        DD      ECT_ASSIGN16
        DD      ECT_ASSIGN8
        DD      ECT_ADD_ESP
        DD      ECT_NOP
        DD      ECT_OR
        DD      ECT_AND
        DD      ECT_SHL
        DD      ECT_SHR
        DD      ECT_COM
        DD      ECT_PRINTF
        DD      ECT_LABEL
        DD      ECT_SKIPZ
        DD      ECT_JMP
        DD      ECT_ZBD
        DD      ECT_SBD
        DD      ECT_ZWD
        DD      ECT_SWD
        DD      ECT_DWORD_PP
        DD      ECT_WORD_PP
        DD      ECT_BYTE_PP
        DD      ECT_short_PP
        DD      ECT_char_PP
        DD      ECT_DWORD_MM
        DD      ECT_WORD_MM
        DD      ECT_BYTE_MM
        DD      ECT_short_MM
        DD      ECT_char_MM
        DD      ECT_ADDRESS
        DD      ECT_FS
        DD      ECT_CLI
        DD      ECT_STI
        DD      ECT_GET_FLAGS
        DD      ECT_SET_FLAGS
        DD      ECT_BT
        DD      ECT_BTS
        DD      ECT_BTR
        DD      ECT_INSQUE
        DD      ECT_INSQUE_REV
        DD      ECT_REMQUE
        DD      ECT_INP
        DD      ECT_OUTP
        DD      ECT_REP_INSB
        DD      ECT_REP_INSW
        DD      ECT_REP_OUTSB
        DD      ECT_REP_OUTSW
        DD      ECT_MEMCPY
        DD      ECT_MEMSET
        DD      ECT_INPW
        DD      ECT_OUTPW
        DD      ECT_PP_DWORD
        DD      ECT_PP_WORD
        DD      ECT_PP_BYTE
        DD      ECT_PP_short
        DD      ECT_PP_char
        DD      ECT_MM_DWORD
        DD      ECT_MM_WORD
        DD      ECT_MM_BYTE
        DD      ECT_MM_short
        DD      ECT_MM_char
        DD      ECT_INPD
        DD      ECT_OUTPD
        DD      ECT_ENTER
        DD      ECT_LEAVE
        DD      ECT_TYPE
        DD      ECT_DS
        DD      ECT_ADD_ESP8
        DD      ECT_int_DISP32   
        DD      ECT_short_DISP32   
        DD      ECT_char_DISP32    
        DD      ECT_int_DISP8    
        DD      ECT_short_DISP8    
        DD      ECT_char_DISP8
        DD      ECT_CALL_INDIRECT     
        DD      ECT_TABLE_END
UNSIGNED_CODE_TABLE:
        DD      UECT_NULL
        DD      UECT_XOR
        DD      UECT_OR_OR 
        DD      UECT_AND_AND
        DD      UECT_LESS
        DD      UECT_GREATER
        DD      UECT_EQUAL_EQUAL
        DD      UECT_NOT_EQUAL_TO
        DD      UECT_GREATER_OR_EQUAL
        DD      UECT_LESS_OR_EQUAL
        DD      UECT_ADD
        DD      UECT_SUB
        DD      UECT_MUL
        DD      UECT_DIV
        DD      UECT_MOD
        DD      UECT_UNARY_MINUS
        DD      UECT_NOT
        DD      UECT_INT
        DD      UECT_DOUBLE
        DD      UECT_LOCAL_FRAME_PTR
        DD      UECT_DEREFERENCE
        DD      UECT_CALL
        DD      UECT_RET
        DD      UECT_INC
        DD      UECT_DEC
        DD      UECT_INT8
        DD      UECT_ADD32
        DD      UECT_SUB32
        DD      UECT_ADD8
        DD      UECT_SUB8
        DD      UECT_MUL32
        DD      UECT_DIV32
        DD      UECT_MUL8
        DD      UECT_DIV8
        DD      UECT_ASSIGN32
        DD      UECT_ASSIGN16
        DD      UECT_ASSIGN8
        DD      UECT_ADD_ESP
        DD      UECT_NOP
        DD      UECT_OR
        DD      UECT_AND
        DD      UECT_SHL
        DD      UECT_SHR
        DD      UECT_COM
        DD      UECT_PRINTF
        DD      UECT_LABEL
        DD      UECT_SKIPZ
        DD      UECT_JMP
        DD      UECT_ZBD
        DD      UECT_SBD
        DD      UECT_ZWD
        DD      UECT_SWD
        DD      UECT_DWORD_PP
        DD      UECT_WORD_PP
        DD      UECT_BYTE_PP
        DD      UECT_short_PP
        DD      UECT_char_PP
        DD      UECT_DWORD_MM
        DD      UECT_WORD_MM
        DD      UECT_BYTE_MM
        DD      UECT_short_MM
        DD      UECT_char_MM
        DD      UECT_ADDRESS
        DD      UECT_FS
        DD      UECT_CLI
        DD      UECT_STI
        DD      UECT_GET_FLAGS
        DD      UECT_SET_FLAGS
        DD      UECT_BT
        DD      UECT_BTS
        DD      UECT_BTR
        DD      UECT_INSQUE
        DD      UECT_INSQUE_REV
        DD      UECT_REMQUE
        DD      UECT_INP
        DD      UECT_OUTP
        DD      UECT_REP_INSB
        DD      UECT_REP_INSW
        DD      UECT_REP_OUTSB
        DD      UECT_REP_OUTSW
        DD      UECT_MEMCPY
        DD      UECT_MEMSET
        DD      UECT_INPW
        DD      UECT_OUTPW
        DD      UECT_PP_DWORD
        DD      UECT_PP_WORD
        DD      UECT_PP_BYTE
        DD      UECT_PP_short
        DD      UECT_PP_char
        DD      UECT_MM_DWORD
        DD      UECT_MM_WORD
        DD      UECT_MM_BYTE
        DD      UECT_MM_short
        DD      UECT_MM_char
        DD      UECT_INPD
        DD      UECT_OUTPD
        DD      UECT_ENTER
        DD      UECT_LEAVE
        DD      UECT_TYPE
        DD      UECT_DS
        DD      UECT_ADD_ESP8
        DD      UECT_DWORD_DISP32   
        DD      UECT_WORD_DISP32    
        DD      UECT_BYTE_DISP32    
        DD      UECT_DWORD_DISP8    
        DD      UECT_WORD_DISP8     
        DD      UECT_BYTE_DISP8     
        DD      UECT_CALL_INDIRECT     
        DD      UECT_TABLE_END

        PUBLIC  CODE_TEXT_TABLE
CODE_TEXT_TABLE:
        DB      'NULL   ',0
        DB      '^32    ',0
        DB      '||32   ',0 
        DB      '&&32   ',0
        DB      '<      ',0
        DB      '>      ',0
        DB      '==     ',0
        DB      '!=     ',0
        DB      '>=     ',0
        DB      '<=     ',0
        DB      '+32    ',0
        DB      '-32    ',0
        DB      '*32    ',0
        DB      '/32    ',0
        DB      '%32    ',0
        DB      '- UNARY',0
        DB      '! UNARY',0
        DB      'DWORD  ',0
        DB      'DOUBLE ',0
        DB      'BP     ',0
        DB      '*DWORD ',0
        DB      'CALL   ',0
        DB      'RET    ',0
        DB      'INC    ',0
        DB      'DEC    ',0
        DB      'BYTE   ',0
        DB      '+32CON ',0
        DB      '-32CON ',0
        DB      '+7CONST',0
        DB      '-7CONST',0
        DB      '*32CON ',0
        DB      '/32CON ',0
        DB      '*7CONST',0
        DB      '/7CONST',0
        DB      ':=32   ',0
        DB      ':=16   ',0
        DB      ':=8    ',0
        DB      'ESP+=  ',0
        DB      'NOP    ',0
        DB      '|      ',0
        DB      '&      ',0
        DB      '<<     ',0
        DB      '>>     ',0
        DB      '~      ',0
        DB      'PRINTF ',0
        DB      'LABEL  ',0
        DB      'SKIPZ  ',0
        DB      'JMP    ',0
        DB      '*BYTE  ',0
        DB      '*char  ',0
        DB      '*WORD  ',0
        DB      '*short ',0
        DB      'DWORD++',0 
        DB      'WORD ++',0
        DB      'BYTE ++',0
        DB      'short++',0
        DB      'char ++',0
        DB      'DWORD--',0 
        DB      'WORD --',0
        DB      'BYTE --',0
        DB      'short--',0
        DB      'char --',0
        DB      '&      ',0
        DB      'FS     ',0
        DB      'CLI    ',0 
        DB      'STI    ',0
        DB      'GTFLAGS',0
        DB      'STFLAGS',0
        DB      'BT     ',0
        DB      'BTS    ',0
        DB      'BTR    ',0
        DB      'INSQUE ',0
        DB      'INSQUER',0
        DB      'REMQUE ',0
        DB      'INP    ',0
        DB      'OUTP   ',0
        DB      'REPINSB',0
        DB      'REPINSW',0
        DB      'RPOUTSB',0
        DB      'RPOUTSW',0
        DB      'MEMCPY ',0
        DB      'MEMSET ',0
        DB      'INPW   ',0
        DB      'OUTPW  ',0
        DB      '++DWORD',0 
        DB      '++WORD ',0
        DB      '++BYTE ',0
        DB      '++short',0
        DB      '++char ',0
        DB      '--DWORD',0 
        DB      '--WORD ',0
        DB      '--BYTE ',0
        DB      '--short',0
        DB      '--char ',0
        DB      'INPD   ',0
        DB      'OUTPD  ',0
        DB      'ENTER  ',0
        DB      'LEAVE  ',0
        DB      'TYPE   ',0
        DB      'DS     ',0
        DB      'ESP8+= ',0
        DB      '[DW+32]',0
        DB      '[WO+32]',0
        DB      '[BY+32]',0
        DB      '[DW+8] ',0
        DB      '[WO+8] ',0
        DB      '[BY+8] ',0
        DB      '*CALL  ',0
FIX_UP_8_TABLE:
        DW      0,0,0,0,0,0,0,0  ;00
        DW      0,0,0,0,0,0,0,0  ;08
        DW      0,0,0,0,0,0,0,0  ;10
        DW      0,-1,0,0,-1,-1,0,0 ;18
        DW      0,0,0,0,0,0,0,0  ;20
        DW      0,0,0,0,0,0,0,0  ;28
        DW      0,0,0,0,0,0,0,0  ;30
        DW      0,0,0,0,0,0,0,0  ;38
        DW      0,0,0,0,0,0,0,0  ;40
        DW      0,0,0,0,0,0,0,0  ;48
        DW      0,0,0,0,0,0,0,0  ;50
        DW      0,0,0,0,0,0,0,0  ;58
        DW      0,0,0,0,-1,0,0,0  ;60
        DW      -1,-1,-1,0,0,0,0,0 ;68
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
FIX_UP_32_TABLE:
        DW      0,0,0,0,0,0,0,0         ;00     
        DW      0,0,0,0,0,0,0,0         ;08     
        DW      0,-4,0,0,0,-4,0,0       ;10
        DW      0,0,-4,-4,0,0,0,0       ;18
        DW      0,0,0,0,0,-4,0,0        ;20
        DW      0,0,0,0,0,0,0,0         ;28
        DW      0,0,0,0,0,0,0,0         ;30
        DW      0,0,0,0,0,0,0,0         ;38
        DW      0,0,0,0,0,0,0,0         ;40
        DW      0,0,0,0,0,0,0,0         ;48
        DW      0,0,0,0,0,0,0,0         ;50
        DW      0,0,0,0,0,0,0,0         ;58
        DW      -4,-6,-4,0,0,-4,-4,-4      ;60
        DW      0,0,0,-4,0,0,0,0         ;68
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
FIX_UP_32_JMP_TABLE:
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,-4,-4
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0
ECT_NULL:
ECT_XOR:
        POP     EDX
        XOR     EAX,EDX
ECT_OR_OR:
        POP     EDX
        OR      EAX,EDX
        JZ      ECT_AND_AND
        MOV     EAX,1
ECT_AND_AND:
        POP     EDX
        AND     EAX,EAX
        JZ      ECT_LESS
        XOR     EAX,EAX
        AND     EDX,EDX
        JZ      ECT_LESS
        INC     EAX
ECT_LESS:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JLE     ECT_GREATER
        INC     EAX
ECT_GREATER:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JGE     ECT_EQUAL_EQUAL
        INC     EAX
ECT_EQUAL_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JNE     ECT_NOT_EQUAL_TO
        INC     EAX
ECT_NOT_EQUAL_TO:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JE      ECT_GREATER_OR_EQUAL
        INC     EAX
ECT_GREATER_OR_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JG      ECT_LESS_OR_EQUAL
        INC     EAX
ECT_LESS_OR_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JL      ECT_ADD
        INC     EAX
ECT_ADD:
        POP     EDX
        ADD     EAX,EDX
ECT_SUB:
        POP     EDX
        SUB     EAX,EDX
        NEG     EAX
ECT_MUL:
        POP     EBX
        MUL     EBX
ECT_DIV:
        MOV     EBX,EAX
        POP     EAX
        XOR     EDX,EDX
        OR      EAX,EAX
        JNS     @@3 
        NOT     EDX    
@@3:    IDIV    EBX
ECT_MOD:
        MOV     EBX,EAX
        POP     EAX
        XOR     EDX,EDX
        OR      EAX,EAX
        JNS     @@4
        NOT     EDX
@@4:    IDIV    EBX
        MOV     EAX,EDX
ECT_UNARY_MINUS:
        NEG     EAX
ECT_NOT:
        OR      EAX,EAX
        MOV     EAX,1
        JZ      ECT_INT
        XOR     EAX,EAX
ECT_INT:
        PUSH    EAX
        MOV     EAX,12345678h
ECT_DOUBLE:
ECT_LOCAL_FRAME_PTR:
        PUSH    EAX
        MOV     EAX,EBP
ECT_DEREFERENCE:
        MOV     EAX,DWORD PTR [EAX]
ECT_CALL:
        PUSH    EAX
        CALL    NEAR PTR ECT_CALL
ECT_RET PROC NEAR
        RET
ECT_RET ENDP
ECT_INC:
        INC     EAX
ECT_DEC:
        DEC     EAX
ECT_INT8:
        MOV     EAX,12h
ECT_ADD32:
        ADD     EAX,12345678h
ECT_SUB32:
        SUB     EAX,12345678h
ECT_ADD8:
        ADD     EAX,12h
ECT_SUB8:
        SUB     EAX,12h
ECT_MUL32:
        MOV     EBX,12345678h
        IMUL    EBX
ECT_DIV32:
        MOV     EBX,12345678h
        XOR     EDX,EDX
        IDIV    EBX
ECT_MUL8:
        MOV     EBX,12h
        IMUL    EBX
ECT_DIV8:
        MOV     EBX,12h
        XOR     EDX,EDX
        IDIV    EBX
ECT_ASSIGN32:
        POP     EBX
        MOV     DWORD PTR [EBX],EAX
ECT_ASSIGN16:
        POP     EBX
        MOV     WORD PTR [EBX],AX
ECT_ASSIGN8:
        POP     EBX
        MOV     BYTE PTR [EBX],AL
ECT_ADD_ESP:
        ADD     ESP,12345678h
ECT_NOP:
ECT_OR:
        POP     EDX
        OR      EAX,EDX
ECT_AND:
        POP     EDX
        AND     EAX,EDX
ECT_SHL:
        POP     EBX
        PUSH    ECX
        MOV     ECX,EAX
        MOV     EAX,EBX
        SHL     EAX,CL
        POP     ECX
ECT_SHR:
        POP     EBX
        PUSH    ECX
        MOV     ECX,EAX
        MOV     EAX,EBX
        SAR     EAX,CL
        POP     ECX
ECT_COM:
        NOT     EAX
ECT_PRINTF:
        INT     I_PRINTF
ECT_LABEL:
ECT_SKIPZ:
        OR      EAX,EAX
        JNZ     ECT_JMP
ECT_SKIPZ2:
        DB      0E9h,01,02,03,04
ECT_JMP:
        DB      0E9h,01,02,03,04
ECT_ZBD:
        MOVZX   EAX,BYTE PTR [EAX]
ECT_SBD:
        MOVSX   EAX,BYTE PTR [EAX]
ECT_ZWD:
        MOVZX   EAX,WORD PTR [EAX]
ECT_SWD:
        MOVSX   EAX,WORD PTR [EAX]
ECT_DWORD_PP:
        MOV     EBX,DWORD PTR [EAX]
        INC     DWORD PTR [EAX]
        MOV     EAX,EBX
ECT_WORD_PP:
        MOVZX   EBX,WORD PTR [EAX]
        INC     WORD PTR [EAX]
        MOV     EAX,EBX
ECT_BYTE_PP:
        MOVZX   EBX,BYTE PTR [EAX]
        INC     BYTE PTR [EAX]
        MOV     EAX,EBX
ECT_short_PP:
        MOVSX   EBX,WORD PTR [EAX]
        INC     WORD PTR [EAX]
        MOV     EAX,EBX
ECT_char_PP:
        MOVSX   EBX,BYTE PTR [EAX]
        INC     BYTE PTR [EAX]
        MOV     EAX,EBX
ECT_DWORD_MM:
        MOV     EBX,DWORD PTR [EAX]
        DEC     DWORD PTR [EAX]
        MOV     EAX,EBX
ECT_WORD_MM:
        MOVZX   EBX,WORD PTR [EAX]
        DEC     WORD PTR [EAX]
        MOV     EAX,EBX
ECT_BYTE_MM:
        MOVZX   EBX,BYTE PTR [EAX]
        DEC     BYTE PTR [EAX]
        MOV     EAX,EBX
ECT_short_MM:
        MOVSX   EBX,WORD PTR [EAX]
        DEC     WORD PTR [EAX]
        MOV     EAX,EBX
ECT_char_MM:
        MOVSX   EBX,BYTE PTR [EAX]
        DEC     BYTE PTR [EAX]
        MOV     EAX,EBX
ECT_ADDRESS:
ECT_FS:
        PUSH    EAX
        MOV     EAX,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
ECT_CLI:
        PUSH    EAX
        CLI
ECT_STI:
        PUSH    EAX
        STI
ECT_GET_FLAGS:
        PUSH    EAX
        PUSHFD
        POP     EAX
ECT_SET_FLAGS:
        PUSH    EAX
        POPFD 
ECT_BT:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BT      DWORD PTR [EBX],EDX
        ADC     EAX,0
ECT_BTS:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BTS     DWORD PTR [EBX],EDX
        ADC     EAX,0
ECT_BTR:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BTR     DWORD PTR [EBX],EDX
        ADC     EAX,0
ECT_INSQUE:
        POP     EDX
        MOV     EBX,[EDX]
        MOV     [EDX],EAX
        MOV     [EAX],EBX
        MOV     4[EAX],EDX
        MOV     4[EBX],EAX
ECT_INSQUE_REV:
        POP     EDX
        MOV     EBX,4[EDX]
        MOV     [EBX],EAX
        MOV     [EAX],EDX
        MOV     4[EAX],EBX
        MOV     4[EDX],EAX
ECT_REMQUE:    
        MOV     EBX,[EAX]
        MOV     EDX,4[EAX]
        MOV     [EDX],EBX
        MOV     4[EBX],EDX
ECT_INP:    
        MOV     EDX,EAX
        XOR     EAX,EAX
        IN      AL,DX
ECT_OUTP:    
        MOV     EDX,EAX
        POP     EAX
        OUT     DX,AL
ECT_REP_INSB:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP INSB
        POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,8
ECT_REP_INSW:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP INSW
        POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,8
ECT_REP_OUTSB:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP OUTSB
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,8
ECT_REP_OUTSW:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP OUTSW
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,8
ECT_MEMCPY:    
        PUSH    EAX
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     EDI,4[EBP]
        MOV     ESI,8[EBP]
        MOV     ECX,12[EBP]
        OR      ECX,ECX
        JZ      @@1
        REP MOVSB
@@1:    POP     EDI
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,12
ECT_MEMSET:    
        PUSH    EAX
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,4[EBP]
        MOV     ECX,12[EBP]
        MOV     EAX,8[EBP]
        OR      ECX,ECX
        JZ      @@2
        REP STOSB
@@2:    POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,12
ECT_INPW:    
        MOV     EDX,EAX
        XOR     EAX,EAX
        IN      AX,DX
ECT_OUTPW:    
        MOV     EDX,EAX
        POP     EAX
        OUT     DX,AX
ECT_PP_DWORD:
        INC     DWORD PTR [EAX]
        MOV     EAX,DWORD PTR [EAX]
ECT_PP_WORD:
        INC     WORD PTR [EAX]
        MOVZX   EAX,WORD PTR [EAX]
ECT_PP_BYTE:
        INC     BYTE PTR [EAX]
        MOVZX   EAX,BYTE PTR [EAX]
ECT_PP_short:
        INC     WORD PTR [EAX]
        MOVSX   EAX,WORD PTR [EAX]
ECT_PP_char:
        INC     BYTE PTR [EAX]
        MOVSX   EAX,BYTE PTR [EAX]
ECT_MM_DWORD:
        DEC     DWORD PTR [EAX]
        MOV     EAX,DWORD PTR [EAX]
ECT_MM_WORD:
        DEC     WORD PTR [EAX]
        MOVZX   EAX,WORD PTR [EAX]
ECT_MM_BYTE:
        DEC     BYTE PTR [EAX]
        MOVZX   EAX,BYTE PTR [EAX]
ECT_MM_short:
        DEC     WORD PTR [EAX]
        MOVSX   EAX,WORD PTR [EAX]
ECT_MM_char:
        DEC     BYTE PTR [EAX]
        MOVSX   EAX,BYTE PTR [EAX]
ECT_INPD:    
        MOV     EDX,EAX
        IN      EAX,DX
ECT_OUTPD:    
        MOV     EAX,EDX
        POP     EAX
        OUT     DX,EAX
ECT_ENTER:    
        PUSH    EBP
        MOV     EBP,ESP
        SUB     ESP,012345678h
ECT_LEAVE PROC NEAR
        ADD     ESP,012345678h
        POP     EBP
        RET
ECT_LEAVE ENDP

ECT_TYPE:
        PUSH    EAX
        MOV     EAX,12345678h
ECT_DS:
        PUSH    EAX
        MOV     EAX,DWORD PTR SYS_MEMORY_BASE
ECT_ADD_ESP8:
        ADD     ESP,12h
ECT_int_DISP32:   
        PUSH    EAX
        MOV     EAX,DWORD PTR 12345678h[EBP]
ECT_short_DISP32:   
        PUSH    EAX
        MOVSX   EAX,WORD PTR 12345678h[EBP]
ECT_char_DISP32:    
        PUSH    EAX
        MOVSX   EAX,BYTE PTR 12345678h[EBP]
ECT_int_DISP8:   
        PUSH    EAX
        MOV     EAX,DWORD PTR 12h[EBP]
ECT_short_DISP8:   
        PUSH    EAX
        MOVSX   EAX,WORD PTR 12h[EBP]
ECT_char_DISP8:    
        PUSH    EAX
        MOVSX   EAX,BYTE PTR 12h[EBP]
ECT_CALL_INDIRECT:
        PUSH    EAX
        MOV     EAX,ESP    
        CALL    DWORD PTR 12345678h[EAX]
ECT_TABLE_END:

UECT_NULL:
UECT_XOR:
        POP     EDX
        XOR     EAX,EDX
UECT_OR_OR:
        POP     EDX
        OR      EAX,EDX
        JZ      UECT_AND_AND
        MOV     EAX,1
UECT_AND_AND:
        POP     EDX
        AND     EAX,EAX
        JZ      UECT_LESS
        XOR     EAX,EAX
        AND     EDX,EDX
        JZ      UECT_LESS
        INC     EAX
UECT_LESS:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JBE     UECT_GREATER
        INC     EAX
UECT_GREATER:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JAE     UECT_EQUAL_EQUAL
        INC     EAX
UECT_EQUAL_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JNE     UECT_NOT_EQUAL_TO
        INC     EAX
UECT_NOT_EQUAL_TO:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JE      UECT_GREATER_OR_EQUAL
        INC     EAX
UECT_GREATER_OR_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JA      UECT_LESS_OR_EQUAL
        INC     EAX
UECT_LESS_OR_EQUAL:
        POP     EDX
        CMP     EAX,EDX
        MOV     EAX,0
        JB      UECT_ADD
        INC     EAX
UECT_ADD:
        POP     EDX
        ADD     EAX,EDX
UECT_SUB:
        POP     EDX
        SUB     EAX,EDX
        NEG     EAX
UECT_MUL:
        POP     EBX
        MUL     EBX
UECT_DIV:
        MOV     EBX,EAX
        POP     EAX
        XOR     EDX,EDX
        DIV     EBX
UECT_MOD:
        MOV     EBX,EAX
        POP     EAX
        XOR     EDX,EDX
        DIV     EBX
        MOV     EAX,EDX
UECT_UNARY_MINUS:
        NEG     EAX
UECT_NOT:
        OR      EAX,EAX
        MOV     EAX,1
        JZ      UECT_INT
        XOR     EAX,EAX
UECT_INT:
        PUSH    EAX
        MOV     EAX,12345678h
UECT_DOUBLE:
UECT_LOCAL_FRAME_PTR:
        PUSH    EAX
        MOV     EAX,EBP
UECT_DEREFERENCE:
        MOV     EAX,DWORD PTR [EAX]
UECT_CALL:
        PUSH    EAX
        CALL    NEAR PTR UECT_CALL
UECT_RET PROC NEAR
        RET
UECT_RET ENDP
UECT_INC:
        INC     EAX
UECT_DEC:
        DEC     EAX
UECT_INT8:
        MOV     EAX,12h
UECT_ADD32:
        ADD     EAX,12345678h
UECT_SUB32:
        SUB     EAX,12345678h
UECT_ADD8:
        ADD     EAX,12h
UECT_SUB8:
        SUB     EAX,12h
UECT_MUL32:
        MOV     EBX,12345678h
        MUL     EBX
UECT_DIV32:
        MOV     EBX,12345678h
        XOR     EDX,EDX
        DIV     EBX
UECT_MUL8:
        MOV     EBX,12h
        MUL     EBX
UECT_DIV8:
        MOV     EBX,12h
        XOR     EDX,EDX
        DIV     EBX
UECT_ASSIGN32:
        POP     EBX
        MOV     DWORD PTR [EBX],EAX
UECT_ASSIGN16:
        POP     EBX
        MOV     WORD PTR [EBX],AX
UECT_ASSIGN8:
        POP     EBX
        MOV     BYTE PTR [EBX],AL
UECT_ADD_ESP:
        ADD     ESP,12345678h
UECT_NOP:
UECT_OR:
        POP     EDX
        OR      EAX,EDX
UECT_AND:
        POP     EDX
        AND     EAX,EDX
UECT_SHL:
        POP     EBX
        PUSH    ECX
        MOV     ECX,EAX
        MOV     EAX,EBX
        SHL     EAX,CL
        POP     ECX
UECT_SHR:
        POP     EBX
        PUSH    ECX
        MOV     ECX,EAX
        MOV     EAX,EBX
        SHR     EAX,CL
        POP     ECX
UECT_COM:
        NOT     EAX
UECT_PRINTF:
        INT     I_PRINTF
UECT_LABEL:
UECT_SKIPZ:
        OR      EAX,EAX
        JNZ     UECT_JMP
UECT_SKIPZ2:
        DB      0E9h,01,02,03,04
UECT_JMP:
        DB      0E9h,01,02,03,04
UECT_ZBD:
        MOVZX   EAX,BYTE PTR [EAX]
UECT_SBD:
        MOVSX   EAX,BYTE PTR [EAX]
UECT_ZWD:
        MOVZX   EAX,WORD PTR [EAX]
UECT_SWD:
        MOVSX   EAX,WORD PTR [EAX]
UECT_DWORD_PP:
        MOV     EBX,DWORD PTR [EAX]
        INC     DWORD PTR [EAX]
        MOV     EAX,EBX
UECT_WORD_PP:
        MOVZX   EBX,WORD PTR [EAX]
        INC     WORD PTR [EAX]
        MOV     EAX,EBX
UECT_BYTE_PP:
        MOVZX   EBX,BYTE PTR [EAX]
        INC     BYTE PTR [EAX]
        MOV     EAX,EBX
UECT_short_PP:
        MOVSX   EBX,WORD PTR [EAX]
        INC     WORD PTR [EAX]
        MOV     EAX,EBX
UECT_char_PP:
        MOVSX   EBX,BYTE PTR [EAX]
        INC     BYTE PTR [EAX]
        MOV     EAX,EBX
UECT_DWORD_MM:
        MOV     EBX,DWORD PTR [EAX]
        DEC     DWORD PTR [EAX]
        MOV     EAX,EBX
UECT_WORD_MM:
        MOVZX   EBX,WORD PTR [EAX]
        DEC     WORD PTR [EAX]
        MOV     EAX,EBX
UECT_BYTE_MM:
        MOVZX   EBX,BYTE PTR [EAX]
        DEC     BYTE PTR [EAX]
        MOV     EAX,EBX
UECT_short_MM:
        MOVSX   EBX,WORD PTR [EAX]
        DEC     WORD PTR [EAX]
        MOV     EAX,EBX
UECT_char_MM:
        MOVSX   EBX,BYTE PTR [EAX]
        DEC     BYTE PTR [EAX]
        MOV     EAX,EBX
UECT_ADDRESS:
UECT_FS:
        PUSH    EAX
        MOV     EAX,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
UECT_CLI:
        PUSH    EAX
        CLI
UECT_STI:
        PUSH    EAX
        STI
UECT_GET_FLAGS:
        PUSH    EAX
        PUSHFD
        POP     EAX
UECT_SET_FLAGS:
        PUSH    EAX
        POPFD 
UECT_BT:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BT      DWORD PTR [EBX],EDX
        ADC     EAX,0
UECT_BTS:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BTS     DWORD PTR [EBX],EDX
        ADC     EAX,0
UECT_BTR:
        MOV     EBX,EAX
        POP     EDX
        XOR     EAX,EAX
        BTR     DWORD PTR [EBX],EDX
        ADC     EAX,0
UECT_INSQUE:
        POP     EDX
        MOV     EBX,[EDX]
        MOV     [EDX],EAX
        MOV     [EAX],EBX
        MOV     4[EAX],EDX
        MOV     4[EBX],EAX
UECT_INSQUE_REV:
        POP     EDX
        MOV     EBX,4[EDX]
        MOV     [EBX],EAX
        MOV     [EAX],EDX
        MOV     4[EAX],EBX
        MOV     4[EDX],EAX
UECT_REMQUE:    
        MOV     EBX,[EAX]
        MOV     EDX,4[EAX]
        MOV     [EDX],EBX
        MOV     4[EBX],EDX
UECT_INP:    
        MOV     EDX,EAX
        XOR     EAX,EAX
        IN      AL,DX
UECT_OUTP:    
        MOV     EDX,EAX
        POP     EAX
        OUT     DX,AL
UECT_REP_INSB:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP INSB
        POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,8
UECT_REP_INSW:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP INSW
        POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,8
UECT_REP_OUTSB:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP OUTSB
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,8
UECT_REP_OUTSW:    
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDX,8[EBP]
        MOV     ECX,4[EBP]
        REP OUTSW
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,8
UECT_MEMCPY:    
        PUSH    EAX
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     EDI,4[EBP]
        MOV     ESI,8[EBP]
        MOV     ECX,12[EBP]
        OR      ECX,ECX
        JZ      @@1
        REP MOVSB
@@1:    POP     EDI
        POP     ESI
        POP     ECX
        POP     EBP
        ADD     ESP,12
UECT_MEMSET:    
        PUSH    EAX
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,4[EBP]
        MOV     ECX,12[EBP]
        MOV     EAX,8[EBP]
        OR      ECX,ECX
        JZ      @@2
        REP STOSB
@@2:    POP     EDI
        POP     ECX
        POP     EBP
        ADD     ESP,12
UECT_INPW:    
        MOV     EDX,EAX
        XOR     EAX,EAX
        IN      AX,DX
UECT_OUTPW:    
        MOV     EDX,EAX
        POP     EAX
        OUT     DX,AX
UECT_PP_DWORD:
        INC     DWORD PTR [EAX]
        MOV     EAX,DWORD PTR [EAX]
UECT_PP_WORD:
        INC     WORD PTR [EAX]
        MOVZX   EAX,WORD PTR [EAX]
UECT_PP_BYTE:
        INC     BYTE PTR [EAX]
        MOVZX   EAX,BYTE PTR [EAX]
UECT_PP_short:
        INC     WORD PTR [EAX]
        MOVSX   EAX,WORD PTR [EAX]
UECT_PP_char:
        INC     BYTE PTR [EAX]
        MOVSX   EAX,BYTE PTR [EAX]
UECT_MM_DWORD:
        DEC     DWORD PTR [EAX]
        MOV     EAX,DWORD PTR [EAX]
UECT_MM_WORD:
        DEC     WORD PTR [EAX]
        MOVZX   EAX,WORD PTR [EAX]
UECT_MM_BYTE:
        DEC     BYTE PTR [EAX]
        MOVZX   EAX,BYTE PTR [EAX]
UECT_MM_short:
        DEC     WORD PTR [EAX]
        MOVSX   EAX,WORD PTR [EAX]
UECT_MM_char:
        DEC     BYTE PTR [EAX]
        MOVSX   EAX,BYTE PTR [EAX]
UECT_INPD:    
        MOV     EDX,EAX
        IN      EAX,DX
UECT_OUTPD:    
        MOV     EDX,EAX
        POP     EAX
        OUT     DX,EAX
UECT_ENTER:    
        PUSH    EBP
        MOV     EBP,ESP
        SUB     ESP,012345678h
UECT_LEAVE PROC NEAR
        ADD     ESP,012345678h
        POP     EBP
        RET
UECT_LEAVE ENDP
UECT_TYPE:
        PUSH    EAX
        MOV     EAX,12345678h
UECT_DS:
        PUSH    EAX
        MOV     EAX,DWORD PTR SYS_MEMORY_BASE
UECT_ADD_ESP8:
        ADD     ESP,12h
UECT_DWORD_DISP32:   
        PUSH    EAX
        MOV     EAX,DWORD PTR 12345678h[EBP]
UECT_WORD_DISP32:    
        PUSH    EAX
        MOVZX   EAX,WORD PTR 12345678h[EBP]
UECT_BYTE_DISP32: 
        PUSH    EAX
        MOVZX   EAX,BYTE PTR 12345678h[EBP]   
UECT_DWORD_DISP8:   
        PUSH    EAX
        MOV     EAX,DWORD PTR 12h[EBP]
UECT_WORD_DISP8:    
        PUSH    EAX
        MOVZX   EAX,WORD PTR 12h[EBP]
UECT_BYTE_DISP8: 
        PUSH    EAX
        MOVZX   EAX,BYTE PTR 12h[EBP]   
UECT_CALL_INDIRECT:    
        PUSH    EAX
        MOV     EAX,ESP    
        CALL    DWORD PTR 12345678h[EAX]
UECT_TABLE_END:

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
; OUT:  EAX = COUNT
        ALIGN   4
COUNT_CODE_BYTES PROC NEAR
        CALL    INIT_CB_IN_PTRS
        XOR     ECX,ECX ;COUNT
@@1:    PUSH    ECX
        CALL    GET_INTERMEDIATE_CODE2
        MOV     EDX,ECX
        POP     ECX     
        AND     EAX,0FFh
        JZ      @@100
        CMP     EAX,EC_LABEL
        JNE     @@10
        MOV     DWORD PTR LB_ADDRESS[EBX],ECX
@@10:   TEST    DWORD PTR HTE_USER0[EDX],1
        JNZ     @@20
        ADD     ECX,DWORD PTR CODE_TABLE+4[EAX*4]
        SUB     ECX,DWORD PTR CODE_TABLE[EAX*4]
        JMP     @@1
@@20:   ADD     ECX,DWORD PTR UNSIGNED_CODE_TABLE+4[EAX*4]
        SUB     ECX,DWORD PTR UNSIGNED_CODE_TABLE[EAX*4]
        JMP     @@1
@@100:  MOV     EAX,ECX
        RET
COUNT_CODE_BYTES ENDP

;;**************************PROCEDURE*************************
; IN:   EDI = LEX NODE
;       ECX = OPTIMIZATION PASSES
;OUT:   ECX=CODE SIZE
        PUBLIC  COMPILE_CODE
COMPILE_CODE PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        OR      ECX,ECX
        JZ      @@1A
@@1B:   CALL    OPTIMIZE_CODE
@@1A:   CALL    COUNT_CODE_BYTES
        PUSH    EAX             ;-**** -4[EBP]
        CALL    MALLOC
        PUSH    ESI             ;-**** -8[EBP]
        CALL    INIT_CB_IN_PTRS
@@1:    CALL    GET_INTERMEDIATE_CODE2
        AND     EAX,0FFh
        TEST    DWORD PTR HTE_USER0[ECX],1
        JNZ     @@1C
        MOV     ECX,DWORD PTR CODE_TABLE+4[EAX*4]
        SUB     ECX,DWORD PTR CODE_TABLE[EAX*4]
        JZ      @@2
        PUSH    EAX     ;-****
        PUSH    EDI     ;-****
        MOV     EDI,ESI     
        MOV     ESI,DWORD PTR CODE_TABLE[EAX*4]
        JMP     @@1D
@@1C:   MOV     ECX,DWORD PTR UNSIGNED_CODE_TABLE+4[EAX*4]
        SUB     ECX,DWORD PTR UNSIGNED_CODE_TABLE[EAX*4]
        JZ      @@2
        PUSH    EAX     ;-****
        PUSH    EDI     ;-****
        MOV     EDI,ESI     
        MOV     ESI,DWORD PTR UNSIGNED_CODE_TABLE[EAX*4]
@@1D:   REP     MOVSB
        MOV     ESI,EDI     
        POP     EDI     ;+****
        POP     EAX     ;+****
        MOVSX   EDX,WORD PTR FIX_UP_32_JMP_TABLE[EAX*2]
        OR      EDX,EDX
        JZ      @@4
        MOV     EBX,LB_ADDRESS[EBX]
        ADD     EBX,DWORD PTR -8[EBP]
        SUB     EBX,ESI
        SUB     EBX,EDX
        SUB     EBX,4
        MOV     DWORD PTR [ESI+EDX],EBX
        JMP     @@2
@@4:    MOVSX   EDX,WORD PTR FIX_UP_32_TABLE[EAX*2]
        OR      EDX,EDX
        JZ      @@3
        CMP     EAX,EC_ENTER
        JE      @@5
        CMP     EAX,EC_LEAVE
        JNE     @@6
@@5:    MOV     EBX,DWORD PTR [EBP]       ;OLD EBP
        MOV     EBX,DWORD PTR C_SIZE[EBX]
        NEG     EBX
        JMP     @@8
@@6:    CMP     EAX,EC_CALL
        JNE     @@8
        SUB     EBX,ESI
        SUB     EBX,EDX
        SUB     EBX,4
@@8:    MOV     DWORD PTR [ESI+EDX],EBX
        JMP     @@2
@@3:    MOVSX   EDX,WORD PTR FIX_UP_8_TABLE[EAX*2]
        OR      EDX,EDX
        JZ      @@2
        MOV     BYTE PTR [ESI+EDX],BL
@@2:    OR      EAX,EAX
        JNZ     @@1
@@100:  POP     ESI
        POP     ECX
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_COMPILE_TRACE
        JNC     @@101
        MOV     EAX,ESI
        CALL    PUT_HEX
        CALL    SPACE
        PUSH    ECX
        PUSH    ESI
@@102:  LODSB
        CALL    PUT_HEX_BYTE
        LOOP    @@102
        POP     ESI
        POP     ECX
;        PUSH    ESI
;        PUSH    ECX
;        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
;        CALL    DWORD PTR EXT_DUMP*4[EAX]
;        ADD     ESP,8
        CALL    GET_KEY
@@101:  POP     EBP
        CLC
        RET
COMPILE_CODE ENDP

        ENDS
        END
