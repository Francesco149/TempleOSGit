        INCLUDE OSINC.INC
        EXTRN   SYS_MEMORY_BASE:DWORD
        EXTRN   SYS_FLAGS:DWORD
        EXTRN   SWAP_IN_NEXT:NEAR
        EXTRN   PUT_HEX:NEAR
        EXTRN   SPACE:NEAR
        EXTRN   BINARY_TO_HEX:NEAR
        EXTRN   NEW_STRING:NEAR
        EXTRN   DEBUG_SPIN_WAIT:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

        PUBLIC  SYSTEM_MEMORY_LOW,SYSTEM_MEMORY_HIGH
        ALIGN   4
SYSTEM_MEMORY_LOW:
        DD      100000H
SYSTEM_MEMORY_HIGH:
        DD      1FFFFFFFH

        PUBLIC  MEMORY_FREE_LIST
        ALIGN   4
MEMORY_FREE_LIST:       DD      ?

        ALIGN   4
        PUBLIC  INIT_MEMORY
INIT_MEMORY     PROC NEAR
        MOV     EDX,DWORD PTR SYS_MEMORY_BASE
        MOV     DWORD PTR MEMORY_FREE_LIST,0
        MOV     EBX,DWORD PTR SYSTEM_MEMORY_LOW
        MOV     EAX,DWORD PTR SYSTEM_MEMORY_HIGH
        INC     EAX
        SUB     EAX,EBX
        MOV     ECX,PAGE_BITS
        SHR     EAX,CL
        SUB     EBX,EDX
        MOV     EDI,DWORD PTR MEMORY_FREE_LIST
        MOV     DWORD PTR MB_NEXT[EBX],EDI
        MOV     DWORD PTR MEMORY_FREE_LIST,EBX
        MOV     DWORD PTR MB_SIZE[EBX],EAX
        RET
INIT_MEMORY     ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=NUM PAGES
;
; OUT:  ESI=BASE ADDRESS REL TO DS
;
        ALIGN   4
        PUBLIC  ALLOCATE_MEMORY_BLOCKS
ALLOCATE_MEMORY_BLOCKS PROC NEAR
        PUSH    EAX
        PUSH    EBX
        PUSH    ECX
        PUSH    EDI
        JMP     @@2
@@2A:   CALL    SWAP_IN_NEXT   
@@2:    BTS     SYS_FLAGS,SYSF_MEMORY_IN_USE
        JC      @@2A

        LEA     ESI,DWORD PTR MEMORY_FREE_LIST

@@1:    LEA     EBX,DWORD PTR MB_NEXT[ESI]
        MOV     ESI,DWORD PTR [EBX]
        OR      ESI,ESI
        JZ      @@9                     ;TODO: Out of memory
        CMP     DWORD PTR MB_SIZE[ESI],EAX
        JB      @@1
        JNE     @@3

        MOV     EDI,DWORD PTR MB_NEXT[ESI]
        MOV     DWORD PTR MB_NEXT[EBX],EDI
        JMP     @@5

@@3:    SUB     DWORD PTR MB_SIZE[ESI],EAX    ;UPDATE FREE ENTRY 
        MOV     CL,PAGE_BITS
        MOV     EDI,DWORD PTR MB_SIZE[ESI]
        SHL     EDI,CL
        ADD     ESI,EDI                 ;SET ESI TO @ALLOCATED PAGES

        MOV     MB_SIZE[ESI],EAX
@@5:    MOV     EDI,DWORD PTR FS:TSS_MEMORY_BLOCK_LIST
        MOV     DWORD PTR MB_NEXT[ESI],EDI
        MOV     DWORD PTR FS:TSS_MEMORY_BLOCK_LIST,ESI        

@@9:    BTC     SYS_FLAGS,SYSF_MEMORY_IN_USE
        POP     EDI
        POP     ECX
        POP     EBX
        POP     EAX
        RET
ALLOCATE_MEMORY_BLOCKS ENDP
;;**************************PROCEDURE*************************
; IN:   ESI=BASE ADDRESS
        ALIGN   4
        PUBLIC  FREE_BLOCK_LIST
FREE_BLOCK_LIST PROC NEAR
        PUSH    EAX
        PUSH    EDI

        JMP     @@3
@@3A:   CALL    SWAP_IN_NEXT   
@@3:    BTS     SYS_FLAGS,SYSF_MEMORY_IN_USE
        JC      @@3A

        MOV     EDI,DWORD PTR MEMORY_FREE_LIST
        MOV     DWORD PTR MEMORY_FREE_LIST,ESI
        JMP     @@1
@@2:    MOV     ESI,EAX
@@1:    MOV     EAX,MB_NEXT[ESI]
        OR      EAX,EAX
        JNZ     @@2
        MOV     MB_NEXT[ESI],EDI
        BTC     SYS_FLAGS,SYSF_MEMORY_IN_USE
        POP     EDI
        POP     EAX
        RET
FREE_BLOCK_LIST ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=NUM BYTES
; OUT:  ESI=BASE ADDRESS
;
        ALIGN   4
        PUBLIC  MALLOC
MALLOC PROC NEAR
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_MALLOC_TRACE
        JNC     @@1a
        PUSH    EAX
        MOV     EAX,'A'
        INT     I_PUT_CHAR
        POP     EAX
        CALL    PUT_HEX
        CALL    SPACE

@@1a:   PUSH    EBX
        PUSH    ECX
        PUSH    EDI

        JMP     @@9
@@9A:   CALL    SWAP_IN_NEXT   
@@9:    BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_HEAP_IN_USE
        JC      @@9A

        ADD     EAX,7
        AND     EAX,0FFFFFFFCH
        CMP     EAX,UUM_STRUCT_SIZE
        JAE     @@4
        MOV     EAX,UUM_STRUCT_SIZE       
@@4:    CMP     EAX,HEAP_CACHE_SIZE
        JAE     @@10
        MOV     ESI,DWORD PTR FS:TSS_HEAP_CACHE[EAX]
        OR      ESI,ESI
        JZ      @@10
        MOV     ECX,DWORD PTR UUM_NEXT[ESI]
        MOV     DWORD PTR FS:TSS_HEAP_CACHE[EAX],ECX
        JMP     @@5     
@@10:   MOV     ESI,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        ADD     ESI,TSS_MALLOC_FREE_LIST

@@1:    LEA     EBX,DWORD PTR UUM_NEXT[ESI]
        MOV     ESI,DWORD PTR [EBX]
        OR      ESI,ESI
        JNZ     @@2
        PUSH    EAX             ;-****
        MOV     CL,PAGE_BITS
        ADD     EAX,8*PAGE_SIZE-1
        SHR     EAX,CL
        CALL    ALLOCATE_MEMORY_BLOCKS
        LEA     ESI,MB_STRUCT_SIZE[ESI]
        SHL     EAX,CL
        SUB     EAX,MB_STRUCT_SIZE
        MOV     EDI,DWORD PTR FS:TSS_MALLOC_FREE_LIST
        MOV     DWORD PTR UUM_NEXT[ESI],EDI
        MOV     DWORD PTR UUM_SIZE[ESI],EAX
        MOV     DWORD PTR FS:TSS_MALLOC_FREE_LIST,ESI
        MOV     EBX,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        ADD     EBX,TSS_MALLOC_FREE_LIST
        POP     EAX             ;+****
        JMP     @@3
@@2:    CMP     DWORD PTR UUM_SIZE[ESI],EAX
        JB      @@1
        JNE     @@3

@@8:    MOV     EDI,DWORD PTR UUM_NEXT[ESI]
        MOV     DWORD PTR UUM_NEXT[EBX],EDI
        JMP     @@5

@@3:    SUB     DWORD PTR UUM_SIZE[ESI],EAX    ;UPDATE FREE ENTRY 
        CMP     DWORD PTR UUM_SIZE[ESI],UUM_STRUCT_SIZE
        JAE     @@7
        ADD     DWORD PTR UUM_SIZE[ESI],EAX
        MOV     EAX,DWORD PTR UUM_SIZE[ESI]
        JMP     @@8

@@7:    MOV     EDI,DWORD PTR UUM_SIZE[ESI]
        ADD     ESI,EDI                 ;SET ESI TO @ALLOCATED PAGES

@@5:    MOV     DWORD PTR [ESI],EAX
        ADD     ESI,4
@@100:  BTC     WORD PTR FS:TSS_TASK_FLAGS,TSSF_HEAP_IN_USE
        POP     EDI
        POP     ECX
        POP     EBX
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_MALLOC_TRACE
        JNC     @@101
        PUSH    EAX
        MOV     EAX,ESI
        CALL    PUT_HEX
        CALL    SPACE
        POP     EAX
@@101:  RET
MALLOC ENDP

;;**************************PROCEDURE*************************
; IN:   ESI=BASE ADDRESS
        ALIGN   4
        PUBLIC  FREE
FREE PROC NEAR
        OR      ESI,ESI
        JZ      @@100

        JMP     @@9
@@9A:   CALL    SWAP_IN_NEXT   
@@9:    BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_HEAP_IN_USE
        JC      @@9A

        PUSH    EDI
        SUB     ESI,4
        MOV     EDI,DWORD PTR [ESI] ;USED_USER_MEMORY_SIZE
        MOV     UUM_SIZE[ESI],EDI
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_MALLOC_TRACE
        JNC     @@1
        PUSH    EAX
        MOV     EAX,'F'
        INT     I_PUT_CHAR
        MOV     EAX,EDI
        CALL    PUT_HEX
        CALL    SPACE
        LEA     EAX,4[ESI]
        CALL    PUT_HEX
        CALL    SPACE
        POP     EAX
        
@@1:    CMP     EDI,HEAP_CACHE_SIZE
        JAE     @@10
        PUSH    EBX
        MOV     EBX,DWORD PTR FS:TSS_HEAP_CACHE[EDI]
        MOV     DWORD PTR UUM_NEXT[ESI],EBX
        MOV     DWORD PTR FS:TSS_HEAP_CACHE[EDI],ESI
        POP     EBX
        POP     EDI
        BTC     WORD PTR FS:TSS_TASK_FLAGS,TSSF_HEAP_IN_USE
        RET

@@10:   MOV     EDI,DWORD PTR FS:TSS_MALLOC_FREE_LIST
        MOV     UUM_NEXT[ESI],EDI
        MOV     DWORD PTR FS:TSS_MALLOC_FREE_LIST,ESI
        POP     EDI
        BTC     WORD PTR FS:TSS_TASK_FLAGS,TSSF_HEAP_IN_USE
@@100:  RET
FREE ENDP

;;**************************PROCEDURE*************************
        PUBLIC  RMALLOC
        ALIGN   4
RMALLOC PROC NEAR
        PUSH    FS
        PUSH    EAX
        MOV     EAX,ROOT_TSS_MEM_SEL
        MOV     FS,AX
        POP     EAX
        CALL    MALLOC
        POP     FS
        RET
RMALLOC ENDP

;;**************************PROCEDURE*************************
        PUBLIC  RNEW_STRING
        ALIGN   4
RNEW_STRING PROC NEAR
        PUSH    FS
        PUSH    EAX
        MOV     EAX,ROOT_TSS_MEM_SEL
        MOV     FS,AX
        POP     EAX
        CALL    NEW_STRING
        POP     FS
        RET
RNEW_STRING ENDP

;;**************************PROCEDURE*************************
        PUBLIC  RFREE
        ALIGN   4
RFREE PROC NEAR
        PUSH    FS
        PUSH    EAX
        MOV     EAX,ROOT_TSS_MEM_SEL
        MOV     FS,AX
        POP     EAX
        CALL    FREE
        POP     FS
        RET
RFREE ENDP

;;**************************PROCEDURE*************************
        PUBLIC  TSK_FREEMEM     ;DEAD CODE
        ALIGN   4
TSK_FREEMEM PROC NEAR             ;THIS TASK DISPLAYS FREE MEMORY
        JMP     @@3
@@3A:   CALL    SWAP_IN_NEXT   
@@3:    BTS     SYS_FLAGS,SYSF_MEMORY_IN_USE
        JC      @@3A
        XOR     EAX,EAX
        MOV     ESI,DWORD PTR MEMORY_FREE_LIST
        JMP     @@4
@@2:    MOV     ESI,EBX
@@4:    ADD     EAX,MB_SIZE[ESI]
        MOV     EBX,MB_NEXT[ESI]
        OR      EBX,EBX
        JNZ     @@2
        BTC     SYS_FLAGS,SYSF_MEMORY_IN_USE
        MOV     CL,PAGE_BITS
        SHL     EAX,CL

        MOV     ESI,10
;        CALL    CRT_PUT_HEX
        CALL    SWAP_IN_NEXT
        JMP     @@3A
TSK_FREEMEM ENDP

        ENDS
        END
