        INCLUDE OSINC.INC
        EXTRN   SWAP_IN_NEXT:NEAR
        EXTRN   SYS_TEXT_BASE:DWORD
        EXTRN   SYS_GRAPHICS_BASE:DWORD
        EXTRN   SYS_GRAPHICS_HIDDEN:DWORD
        EXTRN   SYSTEM_EXTERN_TABLE:DWORD
        EXTRN   PUT_HEX:NEAR
        EXTRN   CRLF:NEAR
        EXTRN   SPACE:NEAR
        EXTRN   GET_KEY:NEAR
        EXTRN   JIFFIES:DWORD
;CRT.ASM
        EXTRN   CURSOR_ON               :NEAR
        EXTRN   CURSOR_OFF              :NEAR
        EXTRN   INIT_CRT                :NEAR
        EXTRN   CURSOR_HOME             :NEAR
        EXTRN   CRT_OUT                 :NEAR
        EXTRN   CRT_LINE_OUT            :NEAR
        EXTRN   CLEAR_CRT               :NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE
        PUBLIC  SYS_FONT
SYS_FONT:
        db 000h,000h,000h,000h,000h,000h,000h,000h ;/* ^@ */
        db 07eh,081h,0a5h,081h,0bdh,099h,081h,07eh ;/* ^A */
        db 07eh,0ffh,0bdh,0ffh,0c3h,0e7h,0ffh,07eh ;/* ^B */
        db 06ch,0feh,0feh,0feh,07ch,038h,010h,000h ;/* ^C */
        db 010h,038h,07ch,0feh,07ch,038h,010h,000h ;/* ^D */
        db 000h,018h,03ch,0e7h,0e7h,03ch,018h,000h ;/* ^E */
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,060h,078h,07eh,07eh,078h,060h,000h ;/* |> */
        db 000h,006h,01eh,07eh,07eh,01eh,006h,000h ;/* <| */
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,018h,018h,03ch,03ch,07eh,07eh,000h ;/* /\ */
        db 000h,07eh,07eh,03ch,03ch,018h,018h,000h ;/* \/ */
        db 000h,000h,000h,000h,000h,000h,000h,000h ;/*	 */
        db 018h,03ch,03ch,018h,018h,000h,018h,000h ;/* ! */
        db 06Ch,06Ch,06Ch,000h,000h,000h,000h,000h ;/* " */
        db 036h,036h,07Fh,036h,07Fh,036h,036h,000h ;/* # */
        db 00Ch,03Fh,068h,03Eh,00Bh,07Eh,018h,000h ;/* $ */
        db 060h,066h,00Ch,018h,030h,066h,006h,000h ;/* % */
        db 038h,06Ch,06Ch,038h,06Dh,066h,03Bh,000h ;/* & */
        db 018h,018h,018h,000h,000h,000h,000h,000h ;/* ' */
        db 00Ch,018h,030h,030h,030h,018h,00Ch,000h ;/* ( */
        db 030h,018h,00Ch,00Ch,00Ch,018h,030h,000h ;/* ) */
        db 000h,018h,07Eh,03Ch,07Eh,018h,000h,000h ;/* * */
        db 000h,018h,018h,07Eh,018h,018h,000h,000h ;/* + */
        db 000h,000h,000h,000h,000h,018h,018h,030h ;/* , */
        db 000h,000h,000h,07Eh,000h,000h,000h,000h ;/* - */
        db 000h,000h,000h,000h,000h,018h,018h,000h ;/* . */
        db 000h,006h,00Ch,018h,030h,060h,000h,000h ;/* / */
        db 03Ch,066h,06Eh,07Eh,076h,066h,03Ch,000h ;/* 0 */
        db 018h,038h,018h,018h,018h,018h,07Eh,000h ;/* 1 */
        db 03Ch,066h,006h,00Ch,018h,030h,07Eh,000h ;/* 2 */
        db 03Ch,066h,006h,01Ch,006h,066h,03Ch,000h ;/* 3 */
        db 00Ch,01Ch,03Ch,06Ch,07Eh,00Ch,00Ch,000h ;/* 4 */
        db 07Eh,060h,07Ch,006h,006h,066h,03Ch,000h ;/* 5 */
        db 01Ch,030h,060h,07Ch,066h,066h,03Ch,000h ;/* 6 */
        db 07Eh,006h,00Ch,018h,030h,030h,030h,000h ;/* 7 */
        db 03Ch,066h,066h,03Ch,066h,066h,03Ch,000h ;/* 8 */
        db 03Ch,066h,066h,03Eh,006h,00Ch,038h,000h ;/* 9 */
        db 000h,000h,018h,018h,000h,018h,018h,000h ;/* : */
        db 000h,000h,018h,018h,000h,018h,018h,030h ;/* ; */
        db 00Ch,018h,030h,060h,030h,018h,00Ch,000h ;/* < */
        db 000h,000h,07Eh,000h,07Eh,000h,000h,000h ;/* = */ 
        db 030h,018h,00Ch,006h,00Ch,018h,030h,000h ;/* > */
        db 03Ch,066h,00Ch,018h,018h,000h,018h,000h ;/* ? */
        db 03Ch,066h,06Eh,06Ah,06Eh,060h,03Ch,000h ;/* @ */
        db 03Ch,066h,066h,07Eh,066h,066h,066h,000h ;/* A */
        db 07Ch,066h,066h,07Ch,066h,066h,07Ch,000h ;/* B */
        db 03Ch,066h,060h,060h,060h,066h,03Ch,000h ;/* C */
        db 078h,06Ch,066h,066h,066h,06Ch,078h,000h ;/* D */
        db 07Eh,060h,060h,07Ch,060h,060h,07Eh,000h ;/* E */
        db 07Eh,060h,060h,07Ch,060h,060h,060h,000h ;/* F */
        db 03Ch,066h,060h,06Eh,066h,066h,03Ch,000h ;/* G */
        db 066h,066h,066h,07Eh,066h,066h,066h,000h ;/* H */
        db 07Eh,018h,018h,018h,018h,018h,07Eh,000h ;/* I */
        db 03Eh,00Ch,00Ch,00Ch,00Ch,06Ch,038h,000h ;/* J */
        db 066h,06Ch,078h,070h,078h,06Ch,066h,000h ;/* K */
        db 060h,060h,060h,060h,060h,060h,07Eh,000h ;/* L */
        db 063h,077h,07Fh,06Bh,06Bh,063h,063h,000h ;/* M */
        db 066h,066h,076h,07Eh,06Eh,066h,066h,000h ;/* N */
        db 03Ch,066h,066h,066h,066h,066h,03Ch,000h ;/* O */
        db 07Ch,066h,066h,07Ch,060h,060h,060h,000h ;/* P */
        db 03Ch,066h,066h,066h,06Ah,06Ch,036h,000h ;/* Q */
        db 07Ch,066h,066h,07Ch,06Ch,066h,066h,000h ;/* R */
        db 03Ch,066h,060h,03Ch,006h,066h,03Ch,000h ;/* S */
        db 07Eh,018h,018h,018h,018h,018h,018h,000h ;/* T */
        db 066h,066h,066h,066h,066h,066h,03Ch,000h ;/* U */
        db 066h,066h,066h,066h,066h,03Ch,018h,000h ;/* V */
        db 063h,063h,06Bh,06Bh,07Fh,077h,063h,000h ;/* W */
        db 066h,066h,03Ch,018h,03Ch,066h,066h,000h ;/* X */
        db 066h,066h,066h,03Ch,018h,018h,018h,000h ;/* Y */
        db 07Eh,006h,00Ch,018h,030h,060h,07Eh,000h ;/* Z */
        db 07Ch,060h,060h,060h,060h,060h,07Ch,000h ;/* [ */
        db 000h,060h,030h,018h,00Ch,006h,000h,000h ;/* \ */
        db 03Eh,006h,006h,006h,006h,006h,03Eh,000h ;/* ] */
        db 03Ch,066h,000h,000h,000h,000h,000h,000h ;/* ^ */
        db 000h,000h,000h,000h,000h,000h,000h,0FFh ;/* _ */
        db 030h,018h,000h,000h,000h,000h,000h,000h ;/* ` */
        db 000h,000h,03Ch,006h,03Eh,066h,03Eh,000h ;/* a */
        db 060h,060h,07Ch,066h,066h,066h,07Ch,000h ;/* b */
        db 000h,000h,03Ch,066h,060h,066h,03Ch,000h ;/* c */
        db 006h,006h,03Eh,066h,066h,066h,03Eh,000h ;/* d */
        db 000h,000h,03Ch,066h,07Eh,060h,03Ch,000h ;/* e */
        db 01Ch,030h,030h,07Ch,030h,030h,030h,000h ;/* f */
        db 000h,000h,03Eh,066h,066h,03Eh,006h,03Ch ;/* g */
        db 060h,060h,07Ch,066h,066h,066h,066h,000h ;/* h */
        db 018h,000h,038h,018h,018h,018h,03Ch,000h ;/* i */
        db 018h,000h,038h,018h,018h,018h,018h,070h ;/* j */
        db 060h,060h,066h,06Ch,078h,06Ch,066h,000h ;/* k */
        db 038h,018h,018h,018h,018h,018h,03Ch,000h ;/* l */
        db 000h,000h,036h,07Fh,06Bh,06Bh,063h,000h ;/* m */
        db 000h,000h,07Ch,066h,066h,066h,066h,000h ;/* n */
        db 000h,000h,03Ch,066h,066h,066h,03Ch,000h ;/* o */
        db 000h,000h,07Ch,066h,066h,07Ch,060h,060h ;/* p */
        db 000h,000h,03Eh,066h,066h,03Eh,006h,007h ;/* q */
        db 000h,000h,06Ch,076h,060h,060h,060h,000h ;/* r */
        db 000h,000h,03Eh,060h,03Ch,006h,07Ch,000h ;/* s */
        db 030h,030h,07Ch,030h,030h,030h,01Ch,000h ;/* t */
        db 000h,000h,066h,066h,066h,066h,03Eh,000h ;/* u */
        db 000h,000h,066h,066h,066h,03Ch,018h,000h ;/* v */
        db 000h,000h,063h,06Bh,06Bh,07Fh,036h,000h ;/* w */
        db 000h,000h,066h,03Ch,018h,03Ch,066h,000h ;/* x */
        db 000h,000h,066h,066h,066h,03Eh,006h,03Ch ;/* y */
        db 000h,000h,07Eh,00Ch,018h,030h,07Eh,000h ;/* z */
        db 00Ch,018h,018h,070h,018h,018h,00Ch,000h ;/* { */
        db 018h,018h,018h,018h,018h,018h,018h,000h ;/* | */
        db 030h,018h,018h,00Eh,018h,018h,030h,000h ;/* } */
        db 031h,06Bh,046h,000h,000h,000h,000h,000h ;/* ~ */126
        db 0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh ; /*  */127
        db 024h,024h,024h,024h,024h,024h,024h,024h ;128
        db 03Ch,03Ch,03Ch,03Ch,03Ch,03Ch,03Ch,03Ch ;129
        db 000h,000h,0FFh,000h,000h,0FFh,000h,000h ;130
        db 000h,000h,0FFh,0FFh,0FFh,0FFh,000h,000h ;131
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 022h,088h,022h,088h,022h,088h,022h,088h
        db 055h,0aah,055h,0aah,055h,0aah,055h,0aah
        db 0ddh,077h,0ddh,077h,0ddh,077h,0ddh,077h
        db 018h,018h,018h,018h,018h,018h,018h,018h
        db 018h,018h,018h,0f8h,018h,018h,018h,018h
        db 018h,018h,0f8h,018h,0f8h,018h,018h,018h
        db 066h,066h,066h,0e6h,066h,066h,066h,066h
        db 000h,000h,000h,0feh,066h,066h,066h,066h
        db 000h,000h,0f8h,018h,0f8h,018h,018h,018h
        db 066h,066h,0e6h,006h,0e6h,066h,066h,066h
        db 066h,066h,066h,066h,066h,066h,066h,066h
        db 000h,000h,0feh,006h,0e6h,066h,066h,066h
        db 066h,066h,0e6h,006h,0feh,000h,000h,000h
        db 066h,066h,066h,0feh,000h,000h,000h,000h
        db 018h,018h,0f8h,018h,0f8h,000h,000h,000h
        db 000h,000h,000h,0f8h,018h,018h,018h,018h
        db 018h,018h,018h,01fh,000h,000h,000h,000h
        db 018h,018h,018h,0ffh,000h,000h,000h,000h
        db 000h,000h,000h,0ffh,018h,018h,018h,018h
        db 018h,018h,018h,01fh,018h,018h,018h,018h
        db 000h,000h,000h,0ffh,000h,000h,000h,000h
        db 018h,018h,018h,0ffh,018h,018h,018h,018h
        db 018h,018h,01fh,018h,01fh,018h,018h,018h
        db 066h,066h,066h,067h,066h,066h,066h,066h
        db 066h,066h,067h,060h,07fh,000h,000h,000h
        db 000h,000h,07fh,060h,067h,066h,066h,066h
        db 066h,066h,0e7h,000h,0ffh,000h,000h,000h
        db 000h,000h,0ffh,000h,0e7h,066h,066h,066h
        db 066h,066h,067h,060h,067h,066h,066h,066h
        db 000h,000h,0ffh,000h,0ffh,000h,000h,000h
        db 066h,066h,0e7h,000h,0e7h,066h,066h,066h
        db 018h,018h,0ffh,000h,0ffh,000h,000h,000h
        db 066h,066h,066h,0ffh,000h,000h,000h,000h
        db 000h,000h,0ffh,000h,0ffh,018h,018h,018h
        db 000h,000h,000h,0ffh,066h,066h,066h,066h
        db 066h,066h,066h,07fh,000h,000h,000h,000h
        db 018h,018h,01fh,018h,01fh,000h,000h,000h
        db 000h,000h,01fh,018h,01fh,018h,018h,018h
        db 000h,000h,000h,07fh,066h,066h,066h,066h
        db 066h,066h,066h,0ffh,066h,066h,066h,066h
        db 018h,018h,0ffh,018h,0ffh,018h,018h,018h
        db 018h,018h,018h,0f8h,000h,000h,000h,000h
        db 000h,000h,000h,01fh,018h,018h,018h,018h
        db 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
        db 000h,000h,000h,000h,0ffh,0ffh,0ffh,0ffh
        db 0f0h,0f0h,0f0h,0f0h,0f0h,0f0h,0f0h,0f0h
        db 00fh,00fh,00fh,00fh,00fh,00fh,00fh,00fh
        db 0ffh,0ffh,0ffh,0ffh,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
        db 000h,000h,000h,000h,000h,000h,000h,000h
;;**************************PROCEDURE*************************
        PUBLIC  DRAW_CHAR
        ALIGN   4   ;AL=CHAR, EBX=POSITION
DC_COL  DD  0
DRAW_CHAR PROC NEAR     
        PUSHAD
        PUSH    ES
        MOVZX   EAX,AL
        MOV     DX,ZERO_DS_SEL
        MOV     ES,DX
        MOV     EDI,0A0000h
        MOV     EBX,DWORD PTR DC_COL
        INC     EBX
        MOV     DWORD PTR DC_COL,EBX
        AND     EBX,03Fh
        ADD     EDI,EBX
        MOV     EBX,DWORD PTR DC_COL
        MOV     CL,6
        SHR     EBX,CL
        AND     EBX,03Fh
        PUSH    EAX
        MOV     EAX,EBX
        MOV     EBX,SCREEN_WIDTH
        MUL     EBX
        ADD     EDI,EAX
        POP     EAX

@@10:   LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,8
@@2:    MOV     AL,BYTE PTR [EDX]
        INC     EDX
        MOV     BYTE PTR ES:[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@2

        POP     ES
        POPAD
        RET
DRAW_CHAR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  PLOT_CHAR
        ALIGN   4   ;AX=CHAR, EBX=X*2 EDX=Y
PLOT_CHAR PROC NEAR     
        OR      EBX,EBX
        JS      @@100
        OR      EDX,EDX
        JS      @@100
        CMP     EBX,DWORD PTR FS:TSS_CRT_WIDTH
        JGE     @@100
        CMP     EDX,DWORD PTR FS:TSS_CRT_HEIGHT
        JGE     @@100
        PUSH    EDX
        PUSH    EBX
        PUSH    EAX
        MOV     EAX,EDX
        IMUL    DWORD PTR FS:TSS_CRT_WIDTH
        ADD     EAX,DWORD PTR SYS_TEXT_BASE
        ADD     EBX,EAX
        POP     EAX
        MOV     WORD PTR [EBX],AX
        POP     EBX
        POP     EDX
@@100:  RET
PLOT_CHAR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  DRAW_BORDER
        ALIGN   4
DRAW_BORDER PROC NEAR     
        PUSH    EAX
        PUSH    EBX
        PUSH    ECX
        PUSH    EDX

        XOR     EBX,EBX
        INC     EBX

@@1:    MOV     EBX,DWORD PTR FS:TSS_WINDOW_LEFT
        MOV     ECX,EBX
        NEG     ECX
        ADD     ECX,DWORD PTR FS:TSS_WINDOW_RIGHT
        SHR     ECX,1
        INC     ECX
        MOV     EAX,130
        MOV     AH,BYTE PTR FS:TSS_BORDER_ATTRIBUTE
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_SUSPENDED
        JC      @@5
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_LOCAL_USER
        JNC     @@5
        INC     EAX
@@5:    MOV     EDX,DWORD PTR FS:TSS_WINDOW_TOP
        DEC     EDX
        CALL    PLOT_CHAR
        MOV     EDX,DWORD PTR FS:TSS_WINDOW_BOTTOM
        INC     EDX
        CALL    PLOT_CHAR
        ADD     EBX,2
        LOOP    @@5
        
        MOV     EDX,DWORD PTR FS:TSS_WINDOW_TOP
        MOV     ECX,EDX
        NEG     ECX
        ADD     ECX,DWORD PTR FS:TSS_WINDOW_BOTTOM
        INC     ECX
        MOV     EAX,128
        MOV     AH,BYTE PTR FS:TSS_BORDER_ATTRIBUTE
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_SUSPENDED
        JC      @@10
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_LOCAL_USER
        JNC     @@10
        INC     EAX
@@10:   MOV     EBX,DWORD PTR FS:TSS_WINDOW_LEFT
        SUB     EBX,2
        CALL    PLOT_CHAR
        MOV     EBX,DWORD PTR FS:TSS_WINDOW_RIGHT
        ADD     EBX,2
        CALL    PLOT_CHAR
        INC     EDX
        LOOP    @@10

        POP     EDX
        POP     ECX
        POP     EBX
        POP     EAX
        RET
DRAW_BORDER ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  GR_UPDATE_SCREEN
GR_UPDATE_SCREEN   PROC NEAR
        PUSH    ES                      ;-****
        PUSHAD                          ;-****

        MOV     ESI,DWORD PTR SYS_GRAPHICS_BASE
        MOV     EDI,DWORD PTR SYS_GRAPHICS_HIDDEN
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32*4
        REP     MOVSD

        PUSHFD
        CLI
        MOV     ESI,FS:TSS_ABSOLUTE_ADDRESS
@@20:   MOV     EDI,FS:TSS_NEXT_TSS
        CMP     ESI,EDI
        JE      @@30
        MOV     EAX,DWORD PTR TSS_FS[EDI]
        MOV     FS,AX
        BT      WORD PTR FS:TSS_CRT_FLAGS,CRTF_SHOW
        JNC     @@20
        PUSH    ESI             ;-****
        MOV     EAX,DWORD PTR FS:TSS_UPDATE_WINDOW
        OR      EAX,EAX
        JZ      @@25
        CALL    EAX
@@25:   CALL    DRAW_BORDER
        POP     ESI             ;+****
        JMP     @@20
@@30:   MOV     EAX,DWORD PTR TSS_FS[ESI]
        MOV     FS,AX
        POPFD

        MOV     ESI,DWORD PTR SYS_TEXT_BASE
        MOV     EDI,DWORD PTR SYS_GRAPHICS_HIDDEN
        MOV     BH,SCREEN_LINES
@@1A:   MOV     BL,SCREEN_COLS
@@1:    XOR     EAX,EAX
        LODSW
@@11:
        PUSH    EAX
        TEST    AH,010h
        JZ      @@13B
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12A:  MOV     AL,BYTE PTR [EDX]
        NOT     AL
        INC     EDX
        XOR     BYTE PTR [EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12A
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8
                
@@13B:  POP     EAX
        PUSH    EAX
        TEST    AH,020h
        JZ      @@13C
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12B:  MOV     AL,BYTE PTR [EDX]
        NOT     AL
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*1[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12B
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@13C:  POP     EAX
        PUSH    EAX
        TEST    AH,040h
        JZ      @@13D
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12C:  MOV     AL,BYTE PTR [EDX]
        NOT     AL
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*2[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12C
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@13D:  POP     EAX
        PUSH    EAX
        TEST    AH,080h
        JZ      @@13E
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12D:  MOV     AL,BYTE PTR [EDX]
        NOT     AL
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*3[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12D
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@13E:  POP     EAX
        PUSH    EAX
        TEST    AH,01h
        JZ      @@13F
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12E:  MOV     AL,BYTE PTR [EDX]
        INC     EDX
        XOR     BYTE PTR [EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12E
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8
                
@@13F:  POP     EAX
        PUSH    EAX
        TEST    AH,02h
        JZ      @@13G
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12F:  MOV     AL,BYTE PTR [EDX]
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*1[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12F
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@13G:  POP     EAX
        PUSH    EAX
        TEST    AH,04h
        JZ      @@13H
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12G:  MOV     AL,BYTE PTR [EDX]
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*2[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12G
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@13H:  POP     EAX
        TEST    AH,08h
        JZ      @@10
        XOR     AH,AH
        LEA     EDX,BYTE PTR SYS_FONT[EAX*8]
        MOV     ECX,FONT_HEIGHT
@@12H:  MOV     AL,BYTE PTR [EDX]
        INC     EDX
        XOR     BYTE PTR SCREEN_WIDTH*SCREEN_HEIGHT/8*3[EDI],AL
        ADD     EDI,SCREEN_WIDTH/8
        LOOP    @@12H
        SUB     EDI,FONT_HEIGHT*SCREEN_WIDTH/8

@@10:   INC     EDI
        DEC     BL
        JNZ     @@1
        ADD     EDI,SCREEN_WIDTH-SCREEN_COLS     
        DEC     BH
        JNZ     @@1A

        MOV     AX,ZERO_DS_SEL
        MOV     ES,AX
        MOV     DX,VGA_SC_INDEX
        MOV     AL,VGA_MAP_MASK
        OUT     DX,AL
        MOV     DX,VGA_SC_DATA
        MOV     AL,1
        OUT     DX,AL
        MOV     ESI,DWORD PTR SYS_GRAPHICS_HIDDEN
        MOV     EDI,0A0000h
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32
        REP     MOVSD

        MOV     DX,VGA_SC_INDEX
        MOV     AL,VGA_MAP_MASK
        OUT     DX,AL
        MOV     DX,VGA_SC_DATA
        MOV     AL,2
        OUT     DX,AL
        MOV     ESI,DWORD PTR SYS_GRAPHICS_HIDDEN
        ADD     ESI,SCREEN_WIDTH*SCREEN_HEIGHT/8*1
        MOV     EDI,0A0000h
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32
        REP     MOVSD

        MOV     DX,VGA_SC_INDEX
        MOV     AL,VGA_MAP_MASK
        OUT     DX,AL
        MOV     DX,VGA_SC_DATA
        MOV     AL,4
        OUT     DX,AL
        MOV     ESI,DWORD PTR SYS_GRAPHICS_HIDDEN
        ADD     ESI,SCREEN_WIDTH*SCREEN_HEIGHT/8*2
        MOV     EDI,0A0000h
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32
        REP     MOVSD

        MOV     DX,VGA_SC_INDEX
        MOV     AL,VGA_MAP_MASK
        OUT     DX,AL
        MOV     DX,VGA_SC_DATA
        MOV     AL,8
        OUT     DX,AL
        MOV     ESI,DWORD PTR SYS_GRAPHICS_HIDDEN
        ADD     ESI,SCREEN_WIDTH*SCREEN_HEIGHT/8*3
        MOV     EDI,0A0000h
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32
        REP     MOVSD

        POPAD                           ;+****
        POP     ES                      ;+****
        RET
GR_UPDATE_SCREEN ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_GR_CLEAR
        ALIGN   4
CMD_GR_CLEAR PROC NEAR  
        PUSH    EAX   
        PUSH    ECX
        PUSH    EDI
        MOV     EDI,DWORD PTR SYS_GRAPHICS_BASE
        MOV     ECX,SCREEN_WIDTH*SCREEN_HEIGHT/32*4
        XOR     EAX,EAX
        REP STOSD
        POP     EDI
        POP     ECX
        POP     EAX
        RET
CMD_GR_CLEAR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_GR_PLOT
        ALIGN   4
CMD_GR_PLOT PROC NEAR  
        PUSH    EBP
        MOV     EBP,ESP
        PUSHAD
        MOV     EDI,DWORD PTR SYS_GRAPHICS_BASE
        MOV     EBX,DWORD PTR SF_PARAM1[EBP]    ;X
        OR      EBX,EBX
        JS      @@100
        ADD     EBX,DWORD PTR FS:TSS_WINDOW_PIXEL_LEFT
        CMP     EBX,DWORD PTR FS:TSS_WINDOW_PIXEL_RIGHT
        JG      @@100
        MOV     EAX,DWORD PTR SF_PARAM2[EBP]    ;Y
        OR      EAX,EAX
        JS      @@100
        ADD     EAX,DWORD PTR FS:TSS_WINDOW_PIXEL_TOP
        CMP     EAX,DWORD PTR FS:TSS_WINDOW_PIXEL_BOTTOM
        JG      @@100
        MOV     ECX,SCREEN_WIDTH
        MUL     ECX
        ADD     EAX,EBX
        SHR     EAX,1
        SHR     EAX,1
        SHR     EAX,1
        ADD     EDI,EAX
        MOV     ECX,7
        AND     EBX,7
        SUB     ECX,EBX
        MOV     EAX,1
        SHL     EAX,CL
        MOV     EBX,DWORD PTR SF_PARAM3[EBP] ;COLOR
        OR      BH,BH
        JZ      @@20
        JS      @@10    
         
        TEST    BL,1
        JZ      @@1A
        OR      BYTE PTR[EDI],AL
@@1A:   TEST    BL,2
        JZ      @@1B
        OR      BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*1[EDI],AL
@@1B:   TEST    BL,4
        JZ      @@1C
        OR      BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*2[EDI],AL
@@1C:   TEST    BL,8
        JZ      @@1D
        OR      BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*3[EDI],AL
@@1D:   JMP     @@100

@@10:   TEST    BL,1
        JZ      @@11A
        XOR     BYTE PTR[EDI],AL
@@11A:  TEST    BL,2
        JZ      @@11B
        XOR     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*1[EDI],AL
@@11B:  TEST    BL,4
        JZ      @@11C
        XOR     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*2[EDI],AL
@@11C:  TEST    BL,8
        JZ      @@11D
        XOR     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*3[EDI],AL
@@11D:  JMP     @@100

@@20:   NOT     EAX
        TEST    BL,1
        JZ      @@21A
        AND     BYTE PTR[EDI],AL
@@21A:  TEST    BL,2
        JZ      @@21B
        AND     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*1[EDI],AL
@@21B:  TEST    BL,4
        JZ      @@21C
        AND     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*2[EDI],AL
@@21C:  TEST    BL,8
        JZ      @@100
        AND     BYTE PTR SCREEN_HEIGHT*SCREEN_WIDTH/8*3[EDI],AL

@@100:  POPAD
        POP     EBP
        RET
CMD_GR_PLOT ENDP

;;**************************PROCEDURE*************************
        PUBLIC  CMD_GR_LINE
        ALIGN   4
CMD_GR_LINE PROC NEAR  
        PUSH    EBP
        MOV     EBP,ESP
        PUSHAD

        MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]
        MOV     ECX,DWORD PTR SF_PARAM3[EBP]
        MOV     EDX,DWORD PTR SF_PARAM4[EBP]

        SUB     EBX,EDX
        JNC     @@1
        NEG     EBX
@@1:    SUB     EAX,ECX
        JNC     @@2
        NEG     EAX
@@2:    CMP     EAX,EBX
        JA      @@50
        JNE     @@20
;dx=dy
        MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]
        MOV     ECX,DWORD PTR SF_PARAM3[EBP]
        MOV     EDX,DWORD PTR SF_PARAM4[EBP]
        
        MOV     ESI,1
        CMP     EAX,ECX
        JAE     @@10
        NEG     ESI

@@10:   CMP     EBX,EDX
        JAE     @@11
        NEG     ESI
        XCHG    EAX,ECX
        XCHG    EBX,EDX
@@11:   SUB     EBX,EDX         ;EBX=dy and (ECX,EDX)=start
        INC     EBX
@@12:   PUSH    DWORD PTR SF_PARAM5[EBP]
        PUSH    EDX
        PUSH    ECX
        CALL    CMD_GR_PLOT
        ADD     ESP,12
        INC     EDX
        ADD     ECX,ESI
        DEC     EBX
        JNZ     @@12
        JMP     @@100

;Y LONGEST, EBX=RISE EAX=RUN
@@20:   MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]

        CMP     EBX,EDX
        JAE     @@20A
        XCHG    EAX,ECX
        XCHG    EBX,EDX
        MOV     DWORD PTR SF_PARAM1[EBP],EAX
        MOV     DWORD PTR SF_PARAM2[EBP],EBX
        MOV     DWORD PTR SF_PARAM3[EBP],ECX
        MOV     DWORD PTR SF_PARAM4[EBP],EDX

@@20A:  SUB     EBX,EDX
        XOR     EDI,EDI
        SUB     EAX,ECX
        JNC     @@20B
        NEG     EAX
        NOT     EDI        
@@20B:  MOV     EDX,EAX
        XOR     EAX,EAX
        DIV     EBX      ;dx*100000000/dy
        MOV     ESI,EAX

        OR      EDI,EDI
        JNS     @@22
        NEG     ESI

@@22:   MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]
        MOV     ECX,DWORD PTR SF_PARAM3[EBP]
        MOV     EDX,DWORD PTR SF_PARAM4[EBP]

        SUB     EBX,EDX         ;EBX=dy and (ECX,EDX)=start
        INC     EBX
        XOR     EAX,EAX
@@30:   PUSH    DWORD PTR SF_PARAM5[EBP]
        PUSH    EDX
        PUSH    ECX
        CALL    CMD_GR_PLOT
        ADD     ESP,12
        INC     EDX
        ADD     EAX,ESI
        ADC     ECX,EDI
        DEC     EBX
        JNZ     @@30
        JMP     @@100

;X LONGEST
@@50:   MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]

        CMP     EAX,ECX
        JAE     @@70A
        XCHG    EAX,ECX
        XCHG    EBX,EDX
        MOV     DWORD PTR SF_PARAM1[EBP],EAX
        MOV     DWORD PTR SF_PARAM2[EBP],EBX
        MOV     DWORD PTR SF_PARAM3[EBP],ECX
        MOV     DWORD PTR SF_PARAM4[EBP],EDX

@@70A:  SUB     EAX,ECX
        XOR     EDI,EDI
        SUB     EBX,EDX
        JNC     @@70B
        NEG     EBX
        NOT     EDI
@@70B:  MOV     EDX,EBX
        MOV     EBX,EAX
        XOR     EAX,EAX
        DIV     EBX     ;dy*100000000/dx
        MOV     ESI,EAX

        OR      EDI,EDI
        JNS     @@72
        NEG     ESI

@@72:   MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,DWORD PTR SF_PARAM2[EBP]
        MOV     ECX,DWORD PTR SF_PARAM3[EBP]
        MOV     EDX,DWORD PTR SF_PARAM4[EBP]

        SUB     EAX,ECX         ;EAX=dx and (ECX,EDX)=start
        XOR     EBX,EBX
        INC     EAX
@@80:   PUSH    DWORD PTR SF_PARAM5[EBP]
        PUSH    EDX
        PUSH    ECX
        CALL    CMD_GR_PLOT
        ADD     ESP,12
        INC     ECX
        ADD     EBX,ESI
        ADC     EDX,EDI
        DEC     EAX
        JNZ     @@80

@@100:  POPAD
        POP     EBP
        RET
CMD_GR_LINE ENDP
        ENDS
        END
