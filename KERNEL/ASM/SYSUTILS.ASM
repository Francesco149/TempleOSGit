        INCLUDE OSINC.INC
        EXTRN   FIXED_PUT_HEX:NEAR
        EXTRN   PUT_HEX:NEAR
        EXTRN   SPACE:NEAR
        EXTRN   CRLF:NEAR
        EXTRN   MALLOC:NEAR
        EXTRN   FREE:NEAR
        EXTRN   NEW_STRING:NEAR
        EXTRN   STRLEN:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

;;**************************PROCEDURE*************************
        PUBLIC  DEBUG_SPIN_WAIT
DEBUG_SPIN_WAIT PROC NEAR
        PUSH    ECX
        MOV     ECX,8FFFFFFFH
@@1:    LOOP    @@1
        POP     ECX
        RET
DEBUG_SPIN_WAIT ENDP

;;**************************PROCEDURE*************************
; IN:   ESI
        PUBLIC  DELETE_LINKED_LIST
        ALIGN   4
DELETE_LINKED_LIST PROC NEAR
        PUSH    EDI
        JMP     @@2
@@1:    MOV     EDI,DWORD PTR [ESI]
        CALL    FREE
        MOV     ESI,EDI
@@2:    OR      ESI,ESI
        JNZ     @@1
        POP     EDI
        RET
DELETE_LINKED_LIST ENDP
;;**************************PROCEDURE*************************
; IN:   ESI=NODE TO INSERT
;       EDI=PREDECESSOR
        PUBLIC  INSQUE
        ALIGN   4
INSQUE  PROC NEAR
        PUSH    EBX
        MOV     EBX,[EDI]
        MOV     [EDI],ESI
        MOV     [ESI],EBX
        MOV     4[ESI],EDI
        MOV     4[EBX],ESI
        POP     EBX
        RET
INSQUE  ENDP
;;**************************PROCEDURE*************************
; IN:   ESI=NODE TO INSERT
;       EDI=SUCCESSOR
        PUBLIC  INSQUE_REV
        ALIGN   4
INSQUE_REV  PROC NEAR
        PUSH    EBX
        MOV     EBX,4[EDI]
        MOV     [EBX],ESI
        MOV     [ESI],EDI
        MOV     4[ESI],EBX
        MOV     4[EDI],ESI
        POP     EBX
        RET
INSQUE_REV  ENDP
;;**************************PROCEDURE*************************
; IN:   ESI=NODE TO REMOVE
        PUBLIC  REMQUE
        ALIGN   4
REMQUE  PROC NEAR
        PUSH    EBX
        PUSH    EDI
        MOV     EBX,[ESI]
        MOV     EDI,4[ESI]
        MOV     [EDI],EBX
        MOV     4[EBX],EDI
        POP     EDI
        POP     EBX
        RET
REMQUE  ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=BINARY
;       ES:[EDI]=@FIELD
;       ECX=LENGTH (IF THE HEX_FIELD ENTRY IS USED)
;
; OUT:  EAX PRESERVED
;       ECX=0
;       [EDI] POINTS PAST DESTINATION

        ALIGN   4
        PUBLIC  BINARY_TO_HEX,BINARY_TO_FIXED_HEX
BINARY_TO_HEX   PROC NEAR
        MOV     ECX,8
BINARY_TO_FIXED_HEX:
        PUSHFD                          ;-****
        CLI                             ;because of STD
        PUSH    EAX                     ;-****
        PUSH    EDX                     ;-****
        ADD     EDI,ECX
        PUSH    EDI                     ;-**** EDI WE WANT TO RETURN
        DEC     EDI
        MOV     EDX,EAX
        STD

@@1:    MOV     AL,DL
        AND     AL,0FH
        ADD     AL,90H
        DAA
        ADC     AL,40H
        DAA
        STOSB   
        SHR     EDX,1
        SHR     EDX,1
        SHR     EDX,1
        SHR     EDX,1
        LOOP    @@1

        POP     EDI                     ;+****
        POP     EDX                     ;+****
        POP     EAX                     ;+****
        POPFD                           ;+****
        RET
BINARY_TO_HEX   ENDP
;;**************************PROCEDURE*************************
;IN:    [ESI] = @FIELD
;       ECX = LENGTH
;OUT:   EAX = VALUE
;       ECX = 0
        PUBLIC  HEX_TO_BINARY
        ALIGN   4
HEX_TO_BINARY   PROC NEAR
        PUSH    EDX
        XOR     EDX,EDX
        OR      ECX,ECX
        JZ      @@9
@@1:    SHL     EDX,1
        SHL     EDX,1
        SHL     EDX,1
        SHL     EDX,1   
        LODSB
        SUB     AL,030H
        CMP     AL,09H
        JA      @@2
        SUB     AL,07H
@@2:    MOVZX   EAX,AL
        ADD     EDX,EAX 
        LOOP    @@1

@@9:    MOV     EAX,EDX
        POP     EDX
        RET
HEX_TO_BINARY   ENDP
;;************************************************************
TRAP_NODE       STRUC
TR_NEXT         DD      ?
TR_ESP          DD      ?
TR_ROUTINE      DD      ?
TR_STRUCT_SIZE  DD      ?
                ENDS
;;**************************PROCEDURE*************************
        PUBLIC  REGISTER_TRAP
        ALIGN   4
REGISTER_TRAP PROC NEAR
;IN:    EBX=ADDRESS OF TRAP
;       ECX=STACK
        PUSH    ESI
        PUSH    EAX
        MOV     EAX,TR_STRUCT_SIZE
        CALL    MALLOC
        MOV     EAX,DWORD PTR FS:TSS_TRAP_LIST
        MOV     DWORD PTR TR_NEXT[ESI],EAX
        MOV     DWORD PTR FS:TSS_TRAP_LIST,ESI
        MOV     DWORD PTR TR_ESP[ESI],ECX
        MOV     DWORD PTR TR_ROUTINE[ESI],EBX
        POP     EAX
        POP     ESI
        RET
REGISTER_TRAP ENDP
;;**************************PROCEDURE*************************
        PUBLIC  END_TRAP
        ALIGN   4
END_TRAP PROC NEAR
        PUSH    EAX
        PUSH    ESI
        MOV     ESI,DWORD PTR FS:TSS_TRAP_LIST
        MOV     EAX,DWORD PTR TR_NEXT[ESI]
        MOV     DWORD PTR FS:TSS_TRAP_LIST,EAX
        CALL    FREE
        POP     ESI
        POP     EAX
        RET
END_TRAP ENDP
;;**************************PROCEDURE*************************
;OUT:   NUKES EBX
        PUBLIC  TRAP
        ALIGN   4
TRAP PROC NEAR
        MOV     EBX,DWORD PTR FS:TSS_TRAP_LIST
        MOV     ESP,DWORD PTR TR_ESP[EBX]
        CALL    END_TRAP
        JMP     DWORD PTR TR_ROUTINE[EBX]
TRAP ENDP
;;**************************PROCEDURE*************************
SEED_LO:        DW      0
SEED_HI:        DW      0
        PUBLIC  CMD_SRAND
        ALIGN   4
CMD_SRAND PROC NEAR 
        PUSH    EBP
        MOV     EBP,ESP
        MOV     AX,WORD PTR SF_PARAM1[EBP]
        MOV     WORD PTR SEED_LO,AX
        XOR     AX,AX
        MOV     WORD PTR SEED_HI,AX
        POP     EBP
        RET
CMD_SRAND ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_RAND
        ALIGN   4
CMD_RAND PROC NEAR 
        PUSH    ESI
        PUSH    ECX
        MOVZX   ECX,WORD PTR SEED_HI
        MOVZX   EBX,WORD PTR SEED_LO
        MOV     EDX,015Ah
        MOV     EAX,04E35h
        XCHG    SI,AX
        XCHG    DX,AX
        TEST    AX,AX
        JE      @@1
        MUL     BX
@@1:    JCXZ    @@2
        XCHG    CX,AX
        MUL     SI
        ADD     AX,CX
@@2:    XCHG    SI,AX
        MUL     BX
        ADD     DX,SI
        ADD     AX,1
        ADC     DX,0
        MOV     WORD PTR SEED_HI,DX
        MOV     WORD PTR SEED_LO,AX
        MOV     AX,DX
        AND     EAX,07FFFh
        POP     ECX
        POP     ESI
        RET
CMD_RAND ENDP
        ENDS
        END
