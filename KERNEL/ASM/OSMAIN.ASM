
        EXTRN   IRQ_VECTORS             :NEAR
        EXTRN   INIT_MEMORY             :NEAR
        EXTRN   SYS_CURRENT_SCAN_CODE   :DWORD
        EXTRN   SYS_BPT_LIST            :DWORD
        EXTRN   SYS_CS_SIZE             :NEAR
        EXTRN   SYS_DS_SIZE             :DWORD
        EXTRN   SYSTEM_STRING_TABLE     :DWORD
        EXTRN   ADD_MAP_SYMBOLS         :NEAR
        EXTRN   NEW_CLASS               :NEAR
        EXTRN   ROOT_COMMAND_LINE       :NEAR
        EXTRN   INIT_SYSTEXT_AND_EXTERNS:NEAR
        EXTRN   BOOT_CODE               :FAR
        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        INCLUDE COMPILE.INC
;;***************************SEGMENT**************************
SYSCODE SEGMENT PARA USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

        PUBLIC  GDTTAB
        ALIGN   4
GDTTAB: DB      MAXGDT*8 DUP(?)

        PUBLIC  ROOT_TSS
        ALIGN   4
ROOT_TSS: DB      TSS_STRUCT_SIZE DUP(?)

        PUBLIC  IDTTAB
        ALIGN   4
IDTTAB: DB      MAXIDT*8 DUP(?)

        PUBLIC  SYS_TEMP_PTR,SYS_MEMORY_BASE,SYS_FLAGS,SYS_HASH_TABLE,SYS_INTERNAL_TYPES
        PUBLIC  SYS_BOOT_IMAGE_START,SYS_BOOT_IMAGE_END,SYS_BOOT_CODE,SYS_TEXT_BASE
        PUBLIC  SYS_GRAPHICS_BASE,SYS_GRAPHICS_HIDDEN,SYS_CLIPBOARD_LTF
        ALIGN   4
SYS_BOOT_IMAGE_START    DD      ?
SYS_BOOT_CODE           DD      ?
SYS_BOOT_IMAGE_END      DD      ?   
SYS_TEMP_PTR:           DD      ?,?   
SYS_FLAGS:              DD      128   ;RAW_CRT_OUT
TOPGDT:                 DD      0
SYS_BOOT_BASE:          DD      ?
SYS_MEMORY_BASE:        DD      ?
SYS_HASH_TABLE:         DD      ?
SYS_INTERNAL_TYPES:     DD      IT_STRUCT_SIZE DUP(0)
SYS_TEXT_BASE           DD      ?
SYS_GRAPHICS_BASE       DD      ?
SYS_GRAPHICS_HIDDEN     DD      ?
SYS_CLIPBOARD_LTF       DD      ?
SYMBOL_TABLE:
        DD      HTT_KEYWORD,KW_CLASS,0,0
        DB      'class',0
        DD      HTT_KEYWORD,KW_CLASS,0,0
        DB      'struct',0
        DD      HTT_KEYWORD,KW_FUN,0,0
        DB      'fun',0
        DD      HTT_KEYWORD,KW_SYSFUN,0,0
        DB      'sysfun',0
        DD      HTT_KEYWORD,KW_IF,0,0
        DB      'if',0
        DD      HTT_KEYWORD,KW_ELSE,0,0
        DB      'else',0
        DD      HTT_KEYWORD,KW_FOR,0,0
        DB      'for',0
        DD      HTT_KEYWORD,KW_WHILE,0,0
        DB      'while',0
        DD      HTT_KEYWORD,KW_EXIT,0,0
        DB      'exit',0
        DD      HTT_KEYWORD,KW_INCLUDE,0,0
        DB      'include',0
        DD      HTT_KEYWORD,KW_DEFINE,0,0
        DB      'define',0
        DD      HTT_KEYWORD,KW_RETURN,0,0
        DB      'return',0
        DD      HTT_KEYWORD,KW_SIZEOF,0,0
        DB      'sizeof',0
        DD      HTT_KEYWORD,KW_INTERNAL,0,0
        DB      'internal',0

        DD      HTT_INTERNAL_TYPE,IT_BYTE,1,EC_ZBD+100h+65536*(EC_ASSIGN8+700h)
        DB      'BYTE',0
        DD      HTT_INTERNAL_TYPE,IT_CHAR,1,EC_SBD+100h+65536*(EC_ASSIGN8+700h)
        DB      'char',0
        DD      HTT_INTERNAL_TYPE,IT_WORD,2,EC_ZWD+100h+65536*(EC_ASSIGN16+700h)
        DB      'WORD',0
        DD      HTT_INTERNAL_TYPE,IT_SHORT,2,EC_SWD+100h+65536*(EC_ASSIGN16+700h)
        DB      'short',0
        DD      HTT_INTERNAL_TYPE,IT_DWORD,4,EC_DEREFERENCE+100h+65536*(EC_ASSIGN32+700h)
        DB      'DWORD',0
        DD      HTT_INTERNAL_TYPE,IT_INT,4,EC_DEREFERENCE+100h+65536*(EC_ASSIGN32+700h)
        DB      'void',0
        DD      HTT_INTERNAL_TYPE,IT_INT,4,EC_DEREFERENCE+100h+65536*(EC_ASSIGN32+700h)
        DB      'int',0
        DD      HTT_INTERNAL_TYPE,IT_DOUBLE,8,EC_DEREFERENCE+100h+65536*(EC_ASSIGN32+700h)
        DB      'double',0

        DD      HTT_INVALID

;;**************************PROCEDURE*************************
INIT_OS PROC NEAR
        MOV     AX,SYS_BOOT_DS_SEL      ;LOAD DS SELECTOR
        MOV     DS,AX
        MOV     ES,AX

        MOV     DWORD PTR TOPGDT,FIRST_FREE_SEL
                                        ; CRT DESCRIPTOR
        MOV     DWORD PTR GDTTAB+CRT_SEL,8000FFFFH
        MOV     DWORD PTR GDTTAB+CRT_SEL+4,00CF920BH
        MOV     DWORD PTR GDTTAB+ZERO_DS_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+4+ZERO_DS_SEL,00CF9200H


        MOV     DWORD PTR GDTTAB+SYS_DS_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+4+SYS_DS_SEL,00CF9200H
        MOV     EBX,DWORD PTR SYSTEM_MEMORY_LOW
        MOV     DWORD PTR SYS_MEMORY_BASE,EBX
        MOV     ECX,EBX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2+SYS_DS_SEL,ECX
        AND     EBX,0FF000000H
        ADD     DWORD PTR GDTTAB+4+SYS_DS_SEL,EBX

        MOV     DWORD PTR GDTTAB+SYS_CS_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+4+SYS_CS_SEL,00CF9A00H
        MOV     EBX,DWORD PTR SYS_MEMORY_BASE
        MOV     ECX,EBX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2+SYS_CS_SEL,ECX
        MOV     ECX,EBX
        AND     ECX,0FF000000H
        ADD     DWORD PTR GDTTAB+4+SYS_CS_SEL,ECX

        MOV     DWORD PTR GDTTAB+ROOT_TSS_SEL,OFFSET(TSS_STRUCT_SIZE)-1
        MOV     DWORD PTR GDTTAB+ROOT_TSS_SEL+4,00008900H
        MOV     EBX,DWORD PTR SYS_MEMORY_BASE
        ADD     EBX,OFFSET ROOT_TSS
        MOV     ECX,EBX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2+ROOT_TSS_SEL,ECX
        MOV     ECX,EBX
        AND     ECX,0FF000000H
        ADD     DWORD PTR GDTTAB+4+ROOT_TSS_SEL,ECX

;COPY HIGH
        MOV     EDI,DWORD PTR SYSTEM_MEMORY_LOW     ;TRANSFER CS
        SUB     EDI,DWORD PTR SYS_BOOT_BASE
        LEA     EAX,SYS_CS_SIZE
        ADD     EAX,PAGE_SIZE-1
        AND     EAX,0-PAGE_SIZE
        ADD     DWORD PTR SYSTEM_MEMORY_LOW,EAX     ;START HEAP HERE         
        XOR     ESI,ESI
        LEA     ECX,SYS_CS_SIZE
        SHR     ECX,1
        SHR     ECX,1
        CLD
        REP MOVSD


                                        ; SETUP POINTER TO LIMIT AND BASE
                                        ;OF GDT.  USE ENTRY (0) BECAUSE IT IS
                                        ;INVALID.
        MOV     WORD PTR SYS_TEMP_PTR,MAXGDT*8-1
        MOV     EAX,DWORD PTR SYS_MEMORY_BASE
        ADD     EAX,OFFSET GDTTAB+NIL_SEL
        MOV     DWORD PTR SYS_TEMP_PTR+2,EAX
        XOR     EAX,EAX
        MOV     WORD PTR SYS_TEMP_PTR+6,AX
        LGDT    FWORD PTR SYS_TEMP_PTR

        MOV     AX,SYS_DS_SEL 
        MOV     DS,AX
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX
        MOV     ESP,0200000H            ;TEMP

        DB      0EAH                 ;JMP SYS_CS_SEL:CONT_INIT_OS
        DD      OFFSET(CONT_INIT_OS)
        DW      SYS_CS_SEL

CONT_INIT_OS:
        CALL    INIT_MEMORY

;INIT INTERRUPT TABLE
        MOV     ECX,MAXIDT
        LEA     EDI,DWORD PTR IDTTAB
        LEA     ESI,DWORD PTR IRQ_VECTORS
@@1:    LODSD
        ADD     EAX,65536*SYS_CS_SEL
        STOSD
        MOV     EAX,00008E00H ;E=32-bit irq gate
        STOSD 
        LOOP    @@1

        CALL    NEAR PTR INIT_8259_AND_LIDT

;MISC INIT
        XOR     EAX,EAX
        MOV     DWORD PTR SYS_CURRENT_SCAN_CODE,EAX
        MOV     DWORD PTR SYS_BPT_LIST,EAX
;INIT SYSTEM TSS
        LEA     EDI,DWORD PTR ROOT_TSS
        MOV     EDX,ROOT_TSS_SEL
        LEA     EAX,COMMAND_LINE
        CALL    NEAR PTR INIT_TSS
        MOV     FS,DX
        MOV     ESP,DWORD PTR FS:TSS_ESP

;        MOV     EAX,ROOT_TSS_SEL          ;SELECT CURRENT TSS
;        LTR     AX

        CALL    INIT_SYSTEXT_AND_EXTERNS
        MOV     DWORD PTR FS:TSS_STRING_TABLE,ESI
;INIT SYSTEM HASH TABLE
        MOV     EAX,04000h
        CALL    NEW_HASH_TABLE
        MOV     DWORD PTR SYS_HASH_TABLE,EDI
        MOV     ESI,DWORD PTR FS:TSS_HASH_TABLE
        MOV     DWORD PTR HT_NEXT[ESI],EDI

;INIT CRT
        MOV     EAX,SCREEN_COLS*SCREEN_LINES*2
        CALL    MALLOC
        MOV     DWORD PTR SYS_TEXT_BASE,ESI
        ADD     ESI,DWORD PTR SYS_MEMORY_BASE
        MOV     DWORD PTR GDTTAB+CRT_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+4+CRT_SEL,00CF9200H
        MOV     ECX,ESI
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2+CRT_SEL,ECX
        AND     ESI,0FF000000H
        ADD     DWORD PTR GDTTAB+4+CRT_SEL,ESI

        MOV     EAX,SCREEN_WIDTH*SCREEN_HEIGHT/8*4
        CALL    MALLOC
        MOV     DWORD PTR SYS_GRAPHICS_BASE,ESI
        CALL    CMD_GR_CLEAR
        MOV     EAX,SCREEN_WIDTH*SCREEN_HEIGHT/8*4
        CALL    MALLOC
        MOV     DWORD PTR SYS_GRAPHICS_HIDDEN,ESI

        CALL    INIT_CRT
        MOV     BYTE PTR FS:TSS_TEXT_ATTRIBUTE,017H
        MOV     BYTE PTR FS:TSS_BORDER_ATTRIBUTE,01FH
        BTS     WORD PTR FS:TSS_CRT_FLAGS,CRTF_WRAP_AROUND
        CALL    CLEAR_CRT
        CALL    CURSOR_OFF
        INC     DWORD PTR FS:TSS_WINDOW_TOP
        CALL    CURSOR_HOME
        
        MOV     EAX,ST_ROOT_STARTUP
        INT     I_PUT_SYSTEXT

;        CALL    DEBUG_SPIN_WAIT

;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_ECHO
;        BTS     DWORD PTR FS:TSS_TASK_FLAGS,TSSF_MALLOC_TRACE
        CALL    NEAR PTR ADD_OS_HASH_ENTRIES
        CALL    ADD_MAP_SYMBOLS

        PUSH    SYS_DEFAULT_FLAGS   ;ENABLE IRQ'S
        POPFD

        JMP     ROOT_COMMAND_LINE

INIT_OS ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  ADD_USER_HASH_ENTRIES
ADD_OS_HASH_ENTRIES PROC NEAR
        LEA     ESI,DWORD PTR SYMBOL_TABLE
        MOV     EDI,DWORD PTR SYS_HASH_TABLE
ADD_USER_HASH_ENTRIES:
@@1:    MOV     EDX,DWORD PTR [ESI]
        CMP     EDX,HTT_INVALID
        JE      @@100
@@2:    MOV     ECX,DWORD PTR 4[ESI]
        PUSH    ECX               ;-**** USER0
        PUSH    DWORD PTR 8[ESI]  ;-**** USER1
        PUSH    DWORD PTR 12[ESI] ;-**** USER2
        ADD     ESI,16
        PUSH    EDI     ;-****
        PUSH    ESI     ;-****
        CMP     EDX,HTT_INTERNAL_TYPE
        JNE     @@3
        CALL    NEW_CLASS
        MOV     EBX,EBP
        MOV     DWORD PTR SYS_INTERNAL_TYPES[ECX*4],EBX
        JMP     @@4

@@3:    MOV     EAX,HTE_STRUCT_SIZE
        CALL    MALLOC
        MOV     EBX,ESI
@@4:    MOV     DWORD PTR HTE_TYPE[EBX],EDX
        POP     ESI     ;+****
        CALL    NEW_STRING
        MOV     DWORD PTR HTE_STRING[EBX],EDI
        CALL    STRLEN
        INC     EAX
        ADD     ESI,EAX
        POP     EDI     ;+****
        POP     DWORD PTR HTE_USER2[EBX]        ;+****
        POP     DWORD PTR HTE_USER1[EBX]        ;+****
        POP     DWORD PTR HTE_USER0[EBX]        ;+****
        PUSH    ESI     ;-****
        MOV     ESI,DWORD PTR HTE_STRING[EBX]
        CALL    FIND_HASH_BUCKET
        MOV     EAX,DWORD PTR [ESI]
        MOV     HTE_NEXT[EBX],EAX
        MOV     DWORD PTR [ESI],EBX
        POP     ESI     ;+****
        JMP     @@1
@@100:  RET
ADD_OS_HASH_ENTRIES ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=CODE ADDRESS
;       ESI=DESC
; OUT:  EDI=TASK ADDRESS
        ALIGN   4
        PUBLIC  SPAWN_TASK
SPAWN_TASK      PROC NEAR
        PUSHFD
        PUSH    ESI
        PUSH    EDX
        CLI
        PUSH    EAX     ;-****
        MOV     EAX,TSS_STRUCT_SIZE
        CALL    RMALLOC
        POP     EAX     ;+****
        MOV     EDI,ESI
        XOR     EDX,EDX
        CALL    NEAR PTR INIT_TSS
        CALL    ADD_TASK_TO_QUEUE
        MOV     EDX,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        MOV     DWORD PTR TSS_PARENT_TSS[EDI],EDX
        POP     EDX
        POP     ESI

        PUSH    ESI
        PUSH    EDI
        PUSH    ECX
        MOV     ECX,TSS_DESC_LENGTH
        ADD     EDI,TSS_TASK_DESCRIPTOR
        REP MOVSB
        XOR     EAX,EAX
        STOSB
        POP     ECX
        POP     EDI
        POP     ESI

        POPFD
        RET
SPAWN_TASK      ENDP

INITIAL_STACK_BLOCKS    EQU     16
;;**************************PROCEDURE*************************
; IN:   [EDI] POINTS TO TSS
;       DX  = THE SELECTOR OF THE TSS (0 MEANS CREATE A NEW SELECTOR)
;       EAX = STARTING EIP
; OUT:  DX  = TSS MEMORY DESCRIPTOR       
INIT_TSS        PROC NEAR
        PUSH    ESI
        PUSH    EDI
        PUSH    EBX
        PUSH    ES
        PUSHFD
        CLI                     ;FOR MEM ALLOC FS CHANGE

        PUSH    EAX
        PUSH    EDI
        MOV     ECX,TSS_STRUCT_SIZE/4
        XOR     EAX,EAX
        REP     STOSD
        POP     EDI
        POP     TSS_EIP[EDI]

        MOV     EAX,SYS_CS_SEL
        MOV     DWORD PTR TSS_CS[EDI],EAX
        MOV     EAX,SYS_DS_SEL
        MOV     DWORD PTR TSS_DS[EDI],EAX
        MOV     DWORD PTR TSS_ES[EDI],EAX
        MOV     DWORD PTR TSS_GS[EDI],EAX

        MOV     DWORD PTR TSS_ABSOLUTE_ADDRESS[EDI],EDI
        MOV     WORD PTR TSS_IO_BITMAP_PTR [EDI],TSS_IO_BITMAP
        MOV     TSS_NEXT_TSS[EDI],EDI
        MOV     TSS_LAST_TSS[EDI],EDI
        MOV     DWORD PTR TSS_EFLAGS[EDI],SYS_DEFAULT_FLAGS
        MOV     DWORD PTR TSS_CR3[EDI],SYS_DEFAULT_CR3
        MOV     EAX,DWORD PTR SYSTEM_STRING_TABLE
        MOV     DWORD PTR TSS_STRING_TABLE[EDI],EAX
        MOV     EAX,07Fh
        MOV     DWORD PTR TSS_OPTIMIZATION[EDI],EAX

;        SLDT    WORD PTR TSS_LDTR[EDI]

        OR      DX,DX
        JNZ     @@1
        CALL    NEAR PTR CREATE_TSS_SELECTOR
@@1:    MOV     WORD PTR TSS_SELECTOR[EDI],DX
        MOV     EAX,EDI
        ADD     EAX,DWORD PTR SYS_MEMORY_BASE
        CALL    NEAR PTR CREATE_MEMORY_SELECTOR          ;TODO:Keep track
        MOV     WORD PTR TSS_FS[EDI],DX

;BLOCKS FOR STACK
        PUSH    FS                      ;-****
        MOV     FS,DX                   ;ALLOC TO THIS PROC NEARESS
        MOV     EAX,INITIAL_STACK_BLOCKS     ;NUMBER OF PAGES
        CALL    ALLOCATE_MEMORY_BLOCKS
        PUSH    ESI                     ;-****
        PUSH    EDI
        MOV     EAX,01000h
        CALL    NEW_HASH_TABLE
        MOV     ESI,EDI
        MOV     EDI,DWORD PTR SYS_HASH_TABLE
        MOV     HT_NEXT[ESI],EDI
        POP     EDI
        MOV     TSS_HASH_TABLE[EDI],ESI
        POP     ESI                     ;+****
        POP     FS                      ;+****
        LEA     EAX,PAGE_SIZE*INITIAL_STACK_BLOCKS[ESI]
        MOV     EBX,SYS_DS_SEL
        MOV     DWORD PTR TSS_SS0[EDI],EBX
        MOV     DWORD PTR TSS_SS1[EDI],EBX
        MOV     DWORD PTR TSS_SS2[EDI],EBX
        MOV     DWORD PTR TSS_SS[EDI],EBX
        MOV     DWORD PTR TSS_ESP0[EDI],EAX
        MOV     DWORD PTR TSS_ESP1[EDI],EAX
        MOV     DWORD PTR TSS_ESP2[EDI],EAX
        MOV     DWORD PTR TSS_ESP[EDI],EAX

        MOV     ECX,(MAXIO/32)
        MOV     EAX,0FFFFFFFFH
        LEA     EDI,TSS_IO_BITMAP[EDI]
        REP     STOSD

        POPFD
        POP     ES
        POP     EBX
        POP     EDI
        POP     ESI
        RET
INIT_TSS        ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=BASE (ABSOLUTE NOT REL TO DS)
; OUT:  EBX=@NEW SELECTOR (IN GDT TABLE)
;        DX=SELECTOR

        PUBLIC  CREATE_MEMORY_SELECTOR
        ALIGN   4
CREATE_MEMORY_SELECTOR  PROC NEAR
        PUSH    DS              ;-****
        PUSH    ECX
        MOV     BX,SYS_DS_SEL
        MOV     DS,BX

        MOV     EDX,DWORD PTR TOPGDT
        MOV     EBX,EDX
        ADD     DWORD PTR TOPGDT,8
        
        MOV     DWORD PTR GDTTAB[EBX],0000FFFFH
        MOV     DWORD PTR GDTTAB+4[EBX],00CF9200H
        MOV     ECX,EAX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2[EBX],ECX
        AND     EAX,0FF000000H
        ADD     DWORD PTR GDTTAB+4[EBX],EAX

        POP     ECX             
        POP     DS              ;+****
        RET
CREATE_MEMORY_SELECTOR  ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=BASE (ABSOLUTE NOT REL TO DS)
; OUT:  EBX=@NEW SELECTOR (IN GDT TABLE)
;        DX=SELECTOR

        PUBLIC  CREATE_CODE_SELECTOR
        ALIGN   4
CREATE_CODE_SELECTOR  PROC NEAR
        PUSH    DS              ;-****
        PUSH    ECX
        MOV     BX,SYS_DS_SEL
        MOV     DS,BX

        MOV     EDX,DWORD PTR TOPGDT
        MOV     EBX,EDX
        ADD     DWORD PTR TOPGDT,8
        
        MOV     DWORD PTR GDTTAB[EBX],0000FFFFH
        MOV     DWORD PTR GDTTAB+4[EBX],00CF9A00H
        MOV     ECX,EAX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2[EBX],ECX
        AND     EAX,0FF000000H
        ADD     DWORD PTR GDTTAB+4[EBX],EAX

        POP     ECX
        POP     DS              ;+****
        RET
CREATE_CODE_SELECTOR  ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=BASE OF TSS REL TO DS
; OUT:  EBX=@NEW SELECTOR (IN GDT TABLE)
;        DX=SELECTOR

        PUBLIC  CREATE_TSS_SELECTOR
        ALIGN   4
CREATE_TSS_SELECTOR     PROC NEAR
        PUSH    EDI             ;-****
        PUSH    ECX

        MOV     EDX,DWORD PTR TOPGDT
        MOV     EBX,EDX
        ADD     DWORD PTR TOPGDT,8
        
        MOV     DWORD PTR GDTTAB[EBX],OFFSET(TSS_STRUCT_SIZE)-1
        MOV     DWORD PTR GDTTAB+4[EBX],00008900H
        ADD     EDI,DWORD PTR SYS_MEMORY_BASE       
        MOV     ECX,EDI
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2[EBX],ECX
        AND     EDI,0FF000000H
        ADD     DWORD PTR GDTTAB+4[EBX],EDI

        POP     ECX
        POP     EDI             ;+****
        RET
CREATE_TSS_SELECTOR     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  INIT_8259_AND_LIDT
INIT_8259_AND_LIDT PROC NEAR

        MOV     AL,15H      ;IW1
        OUT     020H,AL
        OUT     0EBH,AL
        OUT     0A0H,AL
        OUT     0EBH,AL

        MOV     AL,20H      ;IW2
        OUT     021H,AL
        OUT     0EBH,AL
        MOV     AL,28H
        OUT     0A1H,AL
        OUT     0EBH,AL

        MOV     AL,4H       ;IW3
        OUT     021H,AL
        OUT     0EBH,AL
        MOV     AL,2H
        OUT     0A1H,AL
        OUT     0EBH,AL

        MOV     AL,0DH      ;IW4
        OUT     021H,AL
        OUT     0EBH,AL
        MOV     AL,09H 
        OUT     0A1H,AL
        OUT     0EBH,AL

        XOR     AL,AL
        OUT     021H,AL
        OUT     0EBH,AL
        OUT     0A1H,AL
        OUT     0EBH,AL

                                        ; SETUP POINTER TO LIMIT AND BASE
                                        ;OF IDT.
        MOV     WORD PTR SYS_TEMP_PTR,MAXIDT*8-1
        MOV     EAX,DWORD PTR SYS_MEMORY_BASE
        ADD     EAX,OFFSET IDTTAB
        MOV     DWORD PTR SYS_TEMP_PTR+2,EAX
        XOR     EAX,EAX
        MOV     WORD PTR SYS_TEMP_PTR+6,AX
        LIDT    FWORD PTR SYS_TEMP_PTR

        RET
INIT_8259_AND_LIDT ENDP

;;**************************PROCEDURE*************************
        PUBLIC  CMD_REBOOT
CMD_REBOOT        PROC NEAR
        MOV     EAX,ZERO_DS_SEL
        MOV     ES,AX
        MOV     WORD PTR ES:[0472H],0

        MOV     AL,08FH
        OUT     070H,AL
        OUT     0EBH,AL
        MOV     AL,00H
        OUT     071H,AL
        OUT     0EBH,AL
        MOV     AL,0H
        OUT     070H,AL
        OUT     0EBH,AL

        IN      AL,092H
        OR      AL,01H
        OUT     092H,AL
        OUT     0EBH,AL
@@100:  JMP     @@100
CMD_REBOOT  ENDP
        ENDS
;;***************************SEGMENT**************************
SYSFILES SEGMENT PARA USE32 PUBLIC
        ENDS
;;***************************SEGMENT**************************
DOSSTK  SEGMENT STACK PARA USE16 PUBLIC
        ASSUME  CS:INITCODE, DS:SYSCODE, SS:DOSSTK
        ALIGN   4
        DD  256 DUP(?)
TOPSTK:
        ENDS
;;***************************SEGMENT**************************
BOOTCODE        SEGMENT PARA USE16 PUBLIC
        ASSUME  CS:BOOTCODE,DS:BOOTCODE
        ENDS
;;***************************SEGMENT**************************
INITCODE        SEGMENT PARA USE16 PUBLIC
        ASSUME  CS:INITCODE, DS:SYSCODE, SS:DOSSTK
        PUBLIC  START
START:
        MOV     AX,DOSSTK
        MOV     SS,AX
        MOV     AX,OFFSET(TOPSTK)
        MOV     SP,AX
        CLI

        IN      AL,092H
        OR      AL,02H          ;A20
        OUT     092H,AL

        CALL    NEAR PTR ENTER_GRAPHICS_MODE

        PUBLIC  ENTER_PMODE
ENTER_PMODE:
        CLD
        MOV     AX,SYSCODE
        MOV     DS,AX
        MOV     ES,AX
        XOR     EAX,EAX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX
        XOR     ESP,ESP
        XOR     EBX,EBX
        XOR     ECX,ECX
        XOR     EDX,EDX
        XOR     ESI,ESI
        XOR     EDI,EDI
        XOR     EBP,EBP

        MOV     EAX,SYSCODE
        MOV     CL,4
        SHL     EAX,CL
        MOV     DWORD PTR SYS_BOOT_IMAGE_START,EAX
        MOV     EAX,INITCODE
        MOV     CL,4
        SHL     EAX,CL
        ADD     EAX,OFFSET(END_INITCODE)
        MOV     DWORD PTR SYS_BOOT_IMAGE_END,EAX
        MOV     EAX,BOOTCODE
        MOV     CL,4
        SHL     EAX,CL
        ADD     EAX,OFFSET(BOOT_CODE)
        MOV     DWORD PTR SYS_BOOT_CODE,EAX

                                        ; DS DESCRIPTOR
        MOV     DWORD PTR GDTTAB+SYS_BOOT_DS_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+SYS_BOOT_DS_SEL+4,00CF9200H
        MOV     EAX,SYSCODE
        MOV     CL,4
        SHL     EAX,CL
        MOV     DWORD PTR SYS_BOOT_BASE,EAX
        ADD     DWORD PTR GDTTAB+SYS_BOOT_DS_SEL+2,EAX

                                        ; SYS_CS DESCRIPTOR
        MOV     DWORD PTR GDTTAB+SYS_BOOT_CS_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+SYS_BOOT_CS_SEL+4,00CF9A00H
        MOV     EAX,SYSCODE
        MOV     CL,4
        SHL     EAX,CL
        ADD     DWORD PTR GDTTAB+SYS_BOOT_CS_SEL+2,EAX

        MOV     DWORD PTR GDTTAB+SYS_FILES_SEL,0000FFFFH
        MOV     DWORD PTR GDTTAB+4+SYS_FILES_SEL,00CF9200H
        MOV     EBX,SYSFILES
        MOV     CL,4
        SHL     EBX,CL
        MOV     ECX,EBX
        AND     ECX,0FFFFFFH
        ADD     DWORD PTR GDTTAB+2+SYS_FILES_SEL,ECX
        MOV     ECX,EBX
        AND     ECX,0FF000000H
        ADD     DWORD PTR GDTTAB+4+SYS_FILES_SEL,ECX

                                        ; SETUP POINTER TO LIMIT AND BASE
                                        ;OF GDT.  USE ENTRY (0) BECAUSE IT IS
                                        ;INVALID.
        MOV     WORD PTR SYS_TEMP_PTR,MAXGDT*8-1
        MOV     EAX,SYSCODE
        MOV     CL,4
        SHL     EAX,CL
        ADD     EAX,OFFSET GDTTAB+NIL_SEL
        MOV     DWORD PTR SYS_TEMP_PTR+2,EAX
        XOR     EAX,EAX
        MOV     WORD PTR SYS_TEMP_PTR+6,AX
        LGDT    FWORD PTR SYS_TEMP_PTR

        MOV     EAX,CR0                 ;ENTER PROTECTED MODE!!
        BTS     EAX,0
        MOV     CR0,EAX

        DB      066H,0EAH               ;JMP SYS_CS_SEL:INIT_OS
        DD      OFFSET(INIT_OS)
        DW      SYS_BOOT_CS_SEL

;;**************************PROCEDURE*************************
        PUBLIC  ENTER_GRAPHICS_MODE
ENTER_GRAPHICS_MODE PROC NEAR
        XOR     EAX,EAX
        XOR     EBX,EBX
        XOR     ECX,ECX
        XOR     EDX,EDX
        XOR     ESI,ESI
        XOR     EDI,EDI
        XOR     EBP,EBP
        MOV     AX,6Ah          ;800x600 Mono
        INT     10h
        RET
ENTER_GRAPHICS_MODE ENDP
END_INITCODE:
        ENDS
        END     START
