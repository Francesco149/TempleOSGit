        INCLUDE OSINC.INC
        EXTRN   SWAP_IN_NEXT:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

        ALIGN   4
        PUBLIC  SYS_CURRENT_SCAN_CODE
SYS_CURRENT_SCAN_CODE:  DD      0

        ALIGN   4
NORMAL_KEY_SCAN_DECODE_TABLE:
        DB      0,27,'1234567890-=',8,9
        DB      'qwertyuiop[]',13,129,'as'
        DB      'dfghjkl;',39,'`',128,'\zxcv'
        DB      'bnm,./',128,131,130,32,132,133,134,135,136,137
        DB      0,0,0,0,0,0,0,0,240,0,0,241,0,242,0,0
        DB      243
        ALIGN   4
SHIFT_KEY_SCAN_DECODE_TABLE:
        DB      0,27,'!@#$%^&*()_+',8,9
        DB      'QWERTYUIOP{}',13,129,'AS'
        DB      'DFGHJKL:"~',128,'|ZXCV'
        DB      'BNM<>?',128,131,130,32,132,133,134,135,136,137
        DB      0,0,0,0,0,0,0,0,240,0,0,241,0,242,0,0
        DB      243
        ALIGN   4
CTRL_KEY_SCAN_DECODE_TABLE:
        DB      0,27,'1234567890-=',8,9
        DB      17,23,5,18,20,25,21,9,15,16,'[]',13,129,1,19
        DB      4,6,7,8,10,11,12,';',39,'`',128,'\',26,24,3,22
        DB      2,14,13,',./',128,131,130,32,132,133,134,135,136,137
        DB      0,0,0,0,0,0,0,0,240,0,0,241,0,242,0,0
        DB      243
        ALIGN   4
ALT_KEY_SCAN_DECODE_TABLE:
        DB      0,27,'1234567890-=',8,9
        DB      'qwertyuiop[]',13,129,'as'
        DB      'dfghjkl;',39,'`',128,'\zxcv'
        DB      'bnm,./',128,131,130,32,132,133,134,135,136,137
        DB      0,0,0,0,0,0,0,0,240,0,0,241,0,242,0,0
        DB      243

;;**************************PROCEDURE*************************
        PUBLIC  TRANSLATE_SCAN_CODE
        ALIGN   4
TRANSLATE_SCAN_CODE PROC NEAR
        PUSH    EBX
        LEA     EBX,BYTE PTR NORMAL_KEY_SCAN_DECODE_TABLE
        BT      DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_SHIFT
        JNC     @@3
        LEA     EBX,BYTE PTR SHIFT_KEY_SCAN_DECODE_TABLE
        JMP     @@10

@@3:    BT      DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_CTRL
        JNC     @@4
        LEA     EBX,BYTE PTR CTRL_KEY_SCAN_DECODE_TABLE
        JMP     @@10

@@4:    BT      DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_ALT
        JNC     @@10
        LEA     EBX,BYTE PTR ALT_KEY_SCAN_DECODE_TABLE

@@10:   MOVZX   EAX,BYTE PTR SYS_CURRENT_SCAN_CODE
        AND     AL,07FH
        ADD     EBX,EAX
        MOVZX   EAX,BYTE PTR [EBX]
        POP     EBX
        RET
TRANSLATE_SCAN_CODE ENDP
;;**************************PROCEDURE*************************
        PUBLIC  GET_KEY
        ALIGN   4
GET_KEY PROC NEAR
        JMP     @@2
@@1:    CALL    SWAP_IN_NEXT
@@2:    BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY
        JNC     @@1
        CALL    TRANSLATE_SCAN_CODE
        RET
GET_KEY ENDP
        ENDS
        END
