        INCLUDE OSINC.INC
        EXTRN  START:FAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT USE32 PUBLIC
        ENDS
;;***************************SEGMENT**************************
INITCODE SEGMENT USE16 PUBLIC
        ENDS
;;***************************SEGMENT**************************
BOOTCODE SEGMENT PARA USE16 PUBLIC
        ASSUME  CS:BOOTCODE,DS:BOOTCODE
;;**************************PROCEDURE*************************
BLOCK_SIZE      EQU 0200h
BLOCK_COUNT     EQU 020h
BOOT_HIGH_LOC   EQU 09000h

        PUBLIC  BOOT_CODE,BOOT_IMG_BLOCK_LO
        PUBLIC  START_BOOT_CODE,END_BOOT_CODE
START_BOOT_CODE:
        DB      05Ah DUP(?)
BOOT_CODE PROC NEAR
        CLI
        CLD
        MOV     AX,0B800h
        MOV     FS,AX
        MOV     AX,BOOT_HIGH_LOC
        MOV     SS,AX
        MOV     ES,AX
        MOV     SP,01000h
        STI
        XOR     EBX,EBX
        CALL    GET_IP
GET_IP:
        POP     BX
        LEA     EAX,GET_IP
        SUB     EBX,EAX
        MOV     CL,4
        SHR     EBX,CL
        MOV     AX,CS           ;COPY HIGH
        ADD     AX,BX
        MOV     DS,AX
        MOV     ECX,BLOCK_SIZE
        LEA     SI,START_BOOT_CODE
        XOR     DI,DI
        REP MOVSB

        DB      0EAH
        DW      BOOT_RESTART
        DW      BOOT_HIGH_LOC

BOOT_RESTART:
        MOV     ECX,8    ;128K
@@2:    PUSH    ECX
        MOV     AX,CS
        MOV     DS,AX
        LEA     SI,LBA_PACKET
        MOV     DX,080h
        MOV     AX,04200h
        INT     013h
        MOV     AX,CS
        MOV     DS,AX
        LEA     SI,LBA_PACKET
        MOV     AX,BLOCK_SIZE*BLOCK_COUNT/16
        ADD     WORD PTR 6[SI],AX
        MOV     EAX,BLOCK_COUNT
        ADD     DWORD PTR 8[SI],EAX
        POP     ECX
        LOOP    @@2

        JMP     START
BOOT_CODE ENDP

LBA_PACKET:
        DW      16      ;packet size
        DW      BLOCK_COUNT
        DW      0,SYSCODE ;buf addr
BOOT_IMG_BLOCK_LO: DD      0,0     ;Block lo,block hi

END_BOOT_CODE:
        ENDS
        END 
