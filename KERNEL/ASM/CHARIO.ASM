        INCLUDE OSINC.INC
        EXTRN   GET_KEY             :NEAR
        EXTRN   CRT_OUT             :NEAR
        EXTRN   BINARY_TO_FIXED_HEX :NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  GET_CHAR
GET_CHAR PROC NEAR
        CALL    GET_KEY
        INT     I_PUT_CHAR
        RET
GET_CHAR ENDP
;;**************************PROCEDURE*************************
; IN:   ES:[EDI]=ADDRESS OF BUFFER
;       ECX     =SIZE OF BUFFER
; OUT:  ES:[EDI]=BYTE AFTER LAST BYTE ENTERED
;       ECX     =NUM CHARS READ
        ALIGN   4
        PUBLIC  GET_STRING
GET_STRING PROC NEAR
        PUSH    EAX
        PUSH    EBX
        XOR     EBX,EBX
        DEC     ECX
@@1:    OR      ECX,ECX         ;ROOM LEFT?
        JE      @@9
        INT     I_GET_CHAR
        CMP     AL,13           ;RETURN?
        JE      @@9
        CMP     AL,8            ;BACK SPACE?
        JE      @@2
        STOSB
        INC     EBX
        DEC     ECX
        JMP     @@1

@@2:    OR      EBX,EBX
        JZ      @@1
        INC     ECX
        DEC     EDI
        DEC     EBX
        JMP     @@1

@@9:    MOV     AL,0
        STOSB
        MOV     ECX,EBX
        POP     EBX
        POP     EAX
        RET
GET_STRING ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  PUT_CHAR
PUT_CHAR PROC NEAR
        JMP     CRT_OUT
PUT_CHAR ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  PUT_STRING,FIXED_PUT_STRING
PUT_STRING PROC NEAR
; IN:   ES:[ESI]=STRING
;       ECX=COUNT
        PUSH    ECX
        MOV     ECX,0FFFFFFFFH
        CALL    NEAR PTR FIXED_PUT_STRING
        POP     ECX
        RET
PUT_STRING ENDP
FIXED_PUT_STRING PROC NEAR
        OR      ESI,ESI
        JZ      @@200
        PUSH    ESI
        PUSH    ECX
        XOR     EAX,EAX
        OR      ECX,ECX
        JZ      @@100
@@1:    LODSB
        OR      AL,AL
        JZ      @@100
        INT     I_PUT_CHAR
        LOOP    @@1
   
@@100:  POP     ECX
        POP     ESI
@@200:  RET
FIXED_PUT_STRING ENDP
;;**************************PROCEDURE*************************
; IN:   EAX=NUMBER TO PRINT
        ALIGN   4
        PUBLIC  FIXED_PUT_HEX,PUT_HEX,PUT_HEX_WORD,PUT_HEX_BYTE,PUT_DEC
PUT_DEC  PROC NEAR
        PUSH    ECX
        MOV     ECX,8
        CALL    NEAR PTR FIXED_PUT_HEX
        POP     ECX
        RET
PUT_DEC ENDP
PUT_HEX  PROC NEAR
        PUSH    ECX
        MOV     ECX,8
        CALL    NEAR PTR FIXED_PUT_HEX
        POP     ECX
        RET
PUT_HEX ENDP
PUT_HEX_WORD  PROC NEAR
        PUSH    ECX
        MOV     ECX,4
        CALL    NEAR PTR FIXED_PUT_HEX
        POP     ECX
        RET
PUT_HEX_WORD ENDP
PUT_HEX_BYTE  PROC NEAR
        PUSH    ECX
        MOV     ECX,2
        CALL    NEAR PTR FIXED_PUT_HEX
        POP     ECX
        RET
PUT_HEX_BYTE ENDP
FIXED_PUT_HEX PROC NEAR
        PUSH    ES                      ;-****
        PUSH    EAX                     ;-****
        PUSH    EBX                     ;-****
        PUSH    EDI                     ;-****
        PUSH    EBP                     ;-****
        SUB     ESP,8                   ;-****

        MOV     BX,SS
        MOV     ES,BX
        MOV     EDI,ESP

        PUSH    ECX                     ;-****
        CALL    BINARY_TO_FIXED_HEX
        POP     ECX                     ;+****

        MOV     EBP,ESP

@@1:    MOV     AL,BYTE PTR [EBP]
        INC     EBP
        INT     I_PUT_CHAR
        LOOP    @@1

        ADD     ESP,8                   ;+****
        POP     EBP                     ;+****
        POP     EDI                     ;+****
        POP     EBX                     ;+****
        POP     EAX                     ;+****
        POP     ES                      ;+****
        RET
FIXED_PUT_HEX  ENDP
        ENDS
        END
