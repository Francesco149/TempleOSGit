        INCLUDE OSINC.INC
        EXTRN   DRAW_CHAR:NEAR
        EXTRN   FREE:NEAR
        EXTRN   MALLOC:NEAR
        EXTRN   SYSTEM_EXTERN_TABLE:DWORD
        EXTRN   SYS_FLAGS:DWORD
        EXTRN   BINARY_TO_FIXED_HEX:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CURSOR_HOME
CURSOR_HOME     PROC NEAR
        CALL    NEAR PTR CURSOR_OFF
        PUSH    EAX                     ;-****
        PUSH    EDX
        MOV     EAX,DWORD PTR FS:TSS_WINDOW_LEFT
        MOV     DWORD PTR FS:TSS_CURSOR_HOR,EAX
        MOV     DWORD PTR FS:TSS_CURSOR_OFFSET,EAX

        MOV     EAX,DWORD PTR FS:TSS_WINDOW_TOP
        MOV     DWORD PTR FS:TSS_CURSOR_VERT,EAX
        IMUL    DWORD PTR FS:TSS_CRT_WIDTH
        ADD     DWORD PTR FS:TSS_CURSOR_OFFSET,EAX
        POP     EDX
        POP     EAX                     ;+****
        CALL    NEAR PTR CURSOR_ON
        RET
CURSOR_HOME     ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CURSOR_OFF
CURSOR_OFF      PROC NEAR
        PUSH    ES                      ;-****
        PUSH    EAX                     ;-****
        PUSH    EDI                     ;-****
        BTR     WORD PTR FS:TSS_CRT_FLAGS,CRTF_CURSOR_ON
        JNC     @@1
        MOV     AX,WORD PTR FS:TSS_CRT_SELECT
        MOV     ES,AX
        MOV     EDI,DWORD PTR FS:TSS_CURSOR_OFFSET
        INC     EDI
        MOV     AL,0FFH
        CMP     EDI,DWORD PTR FS:TSS_CRT_SIZE
        JGE     @@1
        CMP     EDI,0
        JL      @@1
        XOR     BYTE PTR ES:[EDI],AL
@@1:    POP     EDI                     ;+****
        POP     EAX                     ;+****
        POP     ES                      ;+****
        RET
CURSOR_OFF      ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CURSOR_ON
CURSOR_ON      PROC NEAR
        PUSH    ES                      ;-****
        PUSH    EAX                     ;-****
        PUSH    EDI                     ;-****
        BTS     WORD PTR FS:TSS_CRT_FLAGS,CRTF_CURSOR_ON
        JC     @@1
        MOV     AX,WORD PTR FS:TSS_CRT_SELECT
        MOV     ES,AX
        MOV     EDI,DWORD PTR FS:TSS_CURSOR_OFFSET
        INC     EDI
        MOV     AL,0FFH
        CMP     EDI,DWORD PTR FS:TSS_CRT_SIZE
        JGE     @@1
        CMP     EDI,0
        JL      @@1
        XOR     BYTE PTR ES:[EDI],AL
@@1:    POP     EDI                     ;+****
        POP     EAX                     ;+****
        POP     ES                      ;+****
        RET
CURSOR_ON      ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
EDI_HOME        PROC NEAR
        PUSH    EAX                     ;-****
        PUSH    EDX
        MOV     EDI,DWORD PTR FS:TSS_WINDOW_LEFT
        MOV     EAX,DWORD PTR FS:TSS_WINDOW_TOP
        IMUL    DWORD PTR FS:TSS_CRT_WIDTH
        ADD     EDI,EAX
        POP     EDX
        POP     EAX                     ;+****
        RET
EDI_HOME        ENDP
;;**************************PROCEDURE*************************
; IN:   AX = FILL PATTERN

        ALIGN   4
        PUBLIC  FILL_CRT
FILL_CRT        PROC NEAR
        CALL    CURSOR_OFF
        PUSH    ES                      ;-****
        PUSHAD                          ;-****
        PUSH    EAX

        MOV     DX,WORD PTR FS:TSS_CRT_SELECT
        MOV     ES,DX

        CALL    EDI_HOME
        MOV     EDX,DWORD PTR FS:TSS_WINDOW_BOTTOM
        CMP     EDX,DWORD PTR FS:TSS_CRT_HEIGHT
        JL      @@5
        MOV     EDX,DWORD PTR FS:TSS_CRT_HEIGHT
        DEC     EDX
@@5:    MOV     EBX,DWORD PTR FS:TSS_WINDOW_TOP
        CMP     EBX,0
        JGE     @@6
        PUSH    EDX
        NEG     EBX
        MOV     EAX,DWORD PTR FS:TSS_CRT_WIDTH
        MUL     EBX
        ADD     EDI,EAX
        XOR     EBX,EBX
        POP     EDX
@@6:    SUB     EDX,EBX
        INC     EDX

        MOV     EBX,DWORD PTR FS:TSS_WINDOW_RIGHT
        CMP     EBX,DWORD PTR FS:TSS_CRT_WIDTH
        JL      @@10
        MOV     EBX,DWORD PTR FS:TSS_CRT_WIDTH
        SUB     EBX,2
@@10:   MOV     ECX,DWORD PTR FS:TSS_WINDOW_LEFT
        CMP     ECX,0
        JGE     @@11
        SUB     EDI,ECX
        XOR     ECX,ECX
@@11:   SUB     EBX,ECX
        SHR     EBX,1
        INC     EBX

        POP     EAX
@@1:    MOV     ECX,EBX
        PUSH    EDI                     ;-****
        REP     STOSW
        POP     EDI                     ;+****  
        ADD     EDI,DWORD PTR FS:TSS_CRT_WIDTH
        DEC     EDX
        JNZ     @@1

        POPAD                           ;+****
        POP     ES                      ;+****
        CALL    CURSOR_ON
        RET
FILL_CRT        ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CLEAR_CRT
CLEAR_CRT       PROC NEAR
        PUSH    EAX                     ;-****
        MOV     AL,020H
        MOV     AH,BYTE PTR FS:TSS_TEXT_ATTRIBUTE
        CALL    FILL_CRT
        POP     EAX                     ;+****
        RET
CLEAR_CRT       ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
SCROLL_UP       PROC NEAR
        PUSH    ES                      ;-****
        PUSHAD                          ;-****

        MOV     DX,WORD PTR FS:TSS_CRT_SELECT
        MOV     ES,DX

        CALL    EDI_HOME
        
        MOV     EDX,DWORD PTR FS:TSS_WINDOW_BOTTOM
        SUB     EDX,DWORD PTR FS:TSS_WINDOW_TOP

        MOV     EBX,DWORD PTR FS:TSS_WINDOW_RIGHT
        SUB     EBX,DWORD PTR FS:TSS_WINDOW_LEFT
        SHR     EBX,1
        INC     EBX

@@1:    MOV     ECX,EBX
        MOV     ESI,EDI
        ADD     ESI,DWORD PTR FS:TSS_CRT_WIDTH
        PUSH    EDI                     ;-****
        SEGES
        REP     MOVSW
        POP     EDI                     ;+****  
        ADD     EDI,DWORD PTR FS:TSS_CRT_WIDTH
        DEC     EDX
        JNZ     @@1

        MOV     ECX,EBX
        MOV     EAX,020H
@@2:    STOSB
        INC     EDI
        LOOP    @@2

        POPAD                           ;+****
        POP     ES                      ;+****
        RET
SCROLL_UP       ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
CARRIAGE_RETURN PROC NEAR
        PUSH    EAX                     ;-****
        PUSH    EBX
        MOV     EAX,DWORD PTR FS:TSS_CURSOR_HOR
        MOV     EBX,DWORD PTR FS:TSS_WINDOW_LEFT
        MOV     DWORD PTR FS:TSS_CURSOR_HOR,EBX
        SUB     EAX,EBX
        SUB     DWORD PTR FS:TSS_CURSOR_OFFSET,EAX
        POP     EBX
        POP     EAX                     ;+****
        RET
CARRIAGE_RETURN ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
BACK_SPACE      PROC NEAR
        PUSH    ES                      ;-****
        PUSH    EAX                     ;-****
        PUSH    EDI                     ;-****

        MOV     AX,WORD PTR FS:TSS_CRT_SELECT
        MOV     ES,AX

        MOV     EAX,DWORD PTR FS:TSS_CURSOR_HOR
        SUB     EAX,2
        CMP     EAX,DWORD PTR FS:TSS_WINDOW_LEFT
        JL      @@1

        MOV     DWORD PTR FS:TSS_CURSOR_HOR,EAX
        MOV     EDI,DWORD PTR FS:TSS_CURSOR_OFFSET
        SUB     EDI,2
        MOV     DWORD PTR FS:TSS_CURSOR_OFFSET,EDI
        MOV     AH,BYTE PTR FS:TSS_TEXT_ATTRIBUTE
        MOV     AL,020H
        STOSW

@@1:    POP     EDI                     ;+****
        POP     EAX                     ;+****
        POP     ES                      ;+****
        RET
BACK_SPACE      ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
LINE_FEED       PROC NEAR
        PUSH    EAX                     ;-****
        MOV     EAX,DWORD PTR FS:TSS_CURSOR_VERT
        INC     EAX
        CMP     EAX,DWORD PTR FS:TSS_WINDOW_BOTTOM
        JLE     @@1
        CALL    SCROLL_UP
        JMP     @@2

        ALIGN   4
@@1:    MOV     DWORD PTR FS:TSS_CURSOR_VERT,EAX
        MOV     EAX,DWORD PTR FS:TSS_CRT_WIDTH
        ADD     DWORD PTR FS:TSS_CURSOR_OFFSET,EAX
@@2:    POP     EAX                     ;+****
        RET
LINE_FEED       ENDP
;;**************************PROCEDURE*************************
; IN:   AL=ASCII CODE
        PUBLIC  CRT_OUT
        ALIGN   4
CRT_OUT PROC NEAR
        BT      DWORD PTR SYS_FLAGS,SYSF_RAW_CRT_OUT
        JNC     @@1A
        CALL    DRAW_CHAR
        RET
@@1A:   PUSH    EAX                     ;-****
        PUSH    EBX                     ;-****
        PUSH    EDX                     ;-****
        MOV     EBX,DWORD PTR FS:TSS_CURRENT_LTF
        OR      EBX,EBX
        JZ      @@10
        PUSH    0                       ;Menu Selection
        PUSH    0                       ;Scan Code
        AND     EAX,0FFh
        PUSH    EAX                     ;CH
        PUSH    EBX                     ;LTF
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_LTF_PUTCHAR*4[EAX]
        ADD     ESP,16
        JMP     @@100

@@10:   CALL    CURSOR_OFF
        PUSH    ES                      ;-****
        PUSH    EDI                     ;-****

        MOV     DI,CRT_SEL
        MOV     ES,DI
        CMP     AL,31
        JA      @@2
        CMP     AL,8                    ;BACK SPACE?
        JNE     @@4
        CALL    BACK_SPACE
        JMP     @@3

@@4:    CMP     AL,10                   ;LINE FEED?
        JNE     @@1
        CALL    LINE_FEED
        JMP     @@3

@@1:    CALL    CARRIAGE_RETURN
        JMP     @@3

@@2:    MOV     EDI,DWORD PTR FS:TSS_CURSOR_OFFSET
        MOV     AH,BYTE PTR FS:TSS_TEXT_ATTRIBUTE
        STOSW   
        MOV     EAX,DWORD PTR FS:TSS_CURSOR_HOR
        ADD     EAX,2
        CMP     EAX,DWORD PTR FS:TSS_WINDOW_RIGHT
        JLE     @@5
        BT      WORD PTR FS:TSS_CRT_FLAGS,CRTF_WRAP_AROUND
        JNC     @@3
        CALL    CARRIAGE_RETURN
        CALL    LINE_FEED
        JMP     @@3

@@5:    MOV     DWORD PTR FS:TSS_CURSOR_HOR,EAX
        MOV     DWORD PTR FS:TSS_CURSOR_OFFSET,EDI

@@3:    POP     EDI                     ;+****
        POP     ES                      ;+****
        CALL    CURSOR_ON
@@100:  POP     EDX                     ;+****
        POP     EBX                     ;+****
        POP     EAX                     ;+****  
        RET
CRT_OUT ENDP
;;**************************PROCEDURE*************************
; IN:   ESI=LINE STRING
        PUBLIC  CRT_LINE_OUT
        ALIGN   4
CRT_LINE_OUT PROC NEAR
        PUSH    ES                      ;-****
        PUSH    EAX                     ;-****
        PUSH    EBX                     ;-****
        PUSH    ECX                     ;-****
        PUSH    EDX                     ;-****
        PUSH    ESI                     ;-****
        PUSH    EDI                     ;-****

        MOV     DI,CRT_SEL
        MOV     ES,DI

        MOV     EDI,DWORD PTR FS:TSS_CURSOR_OFFSET
        MOV     EDX,DWORD PTR FS:TSS_WINDOW_RIGHT
        CMP     EDX,DWORD PTR FS:TSS_CRT_WIDTH
        JL      @@10
        MOV     EDX,DWORD PTR FS:TSS_CRT_WIDTH
        SUB     EDX,2
@@10:   MOV     EBX,DWORD PTR FS:TSS_WINDOW_LEFT
        CMP     EBX,0
        JGE     @@11
        NEG     EBX
        ADD     EDI,EBX
        SHR     EBX,1
@@5:    LODSB
        OR      AL,AL
        JZ      @@6
        DEC     EBX
        JNZ     @@5
@@6:    XOR     EBX,EBX
@@11:   CMP     EDI,0
        JL      @@100
        SUB     EDX,EBX
        SHR     EDX,1
        INC     EDX
        ADD     EDX,ESI
        MOV     CL,BYTE PTR [EDX]
        MOV     BYTE PTR [EDX],0 ;terminate (dangerous)
        PUSH    EDX                     ;-****

        MOV     AH,BYTE PTR FS:TSS_TEXT_ATTRIBUTE
        JMP     @@1
@@2:    STOSW
@@1:    LODSB
        OR      AL,AL
        JNZ     @@2     
        POP     EDX                     ;+****
        MOV     BYTE PTR [EDX],CL ;unterminate (dangerous)

@@100:  CALL    LINE_FEED
        POP     EDI                     ;+****
        POP     ESI                     ;+****
        POP     EDX                     ;+****
        POP     ECX                     ;+****
        POP     EBX                     ;+****
        POP     EAX                     ;+****  
        POP     ES                      ;+****
        RET
CRT_LINE_OUT ENDP
;;**************************PROCEDURE*************************
        PUBLIC  INIT_CRT
        ALIGN   4
INIT_CRT        PROC NEAR
        MOV     WORD PTR FS:TSS_CRT_SELECT,CRT_SEL
        MOV     WORD PTR FS:TSS_CRT_FLAGS,0
        BTS     WORD PTR FS:TSS_CRT_FLAGS,CRTF_SHOW
        MOV     BYTE PTR FS:TSS_TEXT_ATTRIBUTE,0F1H
        MOV     BYTE PTR FS:TSS_BORDER_ATTRIBUTE,0F9H
        MOV     BYTE PTR FS:TSS_MENU_ATTRIBUTE,0F2H
        MOV     DWORD PTR FS:TSS_CRT_WIDTH,SCREEN_COLS*2
        MOV     DWORD PTR FS:TSS_CRT_HEIGHT,SCREEN_LINES
        MOV     DWORD PTR FS:TSS_CRT_SIZE,SCREEN_COLS*SCREEN_LINES*2
        MOV     DWORD PTR FS:TSS_WINDOW_RIGHT,SCREEN_COLS*2-4
        MOV     DWORD PTR FS:TSS_WINDOW_LEFT,2
        MOV     DWORD PTR FS:TSS_WINDOW_TOP,1
        MOV     DWORD PTR FS:TSS_WINDOW_BOTTOM,SCREEN_LINES-2
        CALL    CURSOR_HOME
        RET
INIT_CRT        ENDP
        ENDS
        END
