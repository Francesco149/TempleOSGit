        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        INCLUDE COMPILE.INC
        EXTRN   SYS_CURRENT_SCAN_CODE:DWORD
        EXTRN   SYS_FLAGS:DWORD
        EXTRN   RUN_STATEMENT:NEAR
        EXTRN   CMD_REBOOT:NEAR
        EXTRN   SYS_INTERNAL_TYPES:DWORD
        EXTRN   SPAWN_TASK:NEAR
        EXTRN   SWAP_IN_NEXT_PART2:NEAR
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

;;**************************PROCEDURE*************************
        PUBLIC  TSK_POLL_KEYBOARD
        ALIGN   4
TSK_POLL_KEYBOARD PROC NEAR
        CLI
@@1:    MOV     AL,0Ah
        OUT     20h,AL
        OUT     0EBH,AL
        IN      AL,020h
        TEST    AL,2
        JE      @@10
        INT     021h
@@10:   IN      AL,64h
        TEST    AL,1
        JZ      @@20
        INT     021h
@@20:   CALL    SWAP_IN_NEXT
        JMP     @@1
TSK_POLL_KEYBOARD ENDP

        ALIGN   4
        PUBLIC  IRQ_VECTORS
IRQ_VECTORS:
        DD IRQ00,IRQ01,IRQ02,IRQ03
        DD IRQ04,IRQ05,IRQ06,IRQ07
        DD IRQ08,IRQ09,IRQ0A,IRQ0B
        DD IRQ0C,IRQ0D,IRQ0E,IRQ0F
        DD IRQ10,IRQ11,IRQ12,IRQ13
        DD IRQ14,IRQ15,IRQ16,IRQ17
        DD IRQ18,IRQ19,IRQ1A,IRQ1B
        DD IRQ1C,IRQ1D,IRQ1E,IRQ1F
        DD IRQ20,IRQ21,IRQ22,IRQ23
        DD IRQ24,IRQ25,IRQ26,IRQ27
        DD IRQ28,IRQ29,IRQ2A,IRQ2B
        DD IRQ2C,IRQ2D,IRQ2E,IRQ2F
        DD IRQ30,IRQ31,IRQ32,IRQ33
        DD IRQ34,IRQ35,IRQ36,IRQ37
        DD IRQ38,IRQ39,IRQ3A,IRQ3B
        DD IRQ3C,IRQ3D,IRQ3E,IRQ3F
        DD IRQ40,IRQ41,IRQ42,IRQ43
        DD IRQ44,IRQ45,IRQ46,IRQ47
        DD IRQ48,IRQ49,IRQ4A,IRQ4B
        DD IRQ4C,IRQ4D,IRQ4E,IRQ4F
        DD IRQ50,IRQ51,IRQ52,IRQ53
        DD IRQ54,IRQ55,IRQ56,IRQ57
        DD IRQ58,IRQ59,IRQ5A,IRQ5B
        DD IRQ5C,IRQ5D,IRQ5E,IRQ5F

        ALIGN   4
        PUBLIC  JIFFIES
JIFFIES DD      ?

;;***************** RESERVED INTERRUPT ROUTINES ***************
        ALIGN   4
IRQ00:  PUSHAD
        MOV     EDX,0C300C30H
        JMP     IRQJOIN

IRQ01:  PUSHAD
        MOV     EDX,0C310C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ02:  PUSHAD
        MOV     EDX,0C320C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ03:  PUSHAD
        MOV     EDX,0C330C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ04:  PUSHAD
        MOV     EDX,0C340C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ05:  PUSHAD
        MOV     EDX,0C350C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ06:  PUSHAD
        MOV     EDX,0C360C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ07:  PUSHAD
        MOV     EDX,0C370C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ08:  PUSHAD
        MOV     EDX,0C380C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ09:  PUSHAD
        MOV     EDX,0C390C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ0A:  PUSHAD
        MOV     EDX,0C410C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ0B:  PUSHAD
        MOV     EDX,0C420C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ0C:  PUSHAD
        MOV     EDX,0C430C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ0D:  PUSH    EAX
        MOV     EAX,25H                  ;ACKNOWLEDGE INTERRUPT
        OUT     20H,AL
        MOV     EAX,'D'
        INT     I_PUT_CHAR
        POP     EAX
        IRET

        ALIGN   4
IRQ0E:  PUSHAD
        MOV     EDX,0C450C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ0F:  PUSHAD
        MOV     EDX,0C460C30H
        JMP     IRQJOIN

        ALIGN   4
IRQ10:  PUSHAD
        MOV     EDX,0C300C31H
        JMP     IRQJOIN

IRQ11:  PUSHAD
        MOV     EDX,0C310C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ12:  PUSHAD
        MOV     EDX,0C320C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ13:  PUSHAD
        MOV     EDX,0C330C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ14:  PUSHAD
        MOV     EDX,0C340C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ15:  PUSHAD
        MOV     EDX,0C350C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ16:  PUSHAD
        MOV     EDX,0C360C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ17:  PUSHAD
        MOV     EDX,0C370C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ18:  PUSHAD
        MOV     EDX,0C380C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ19:  PUSHAD
        MOV     EDX,0C390C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1A:  PUSHAD
        MOV     EDX,0C410C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1B:  PUSHAD
        MOV     EDX,0C420C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1C:  PUSHAD
        MOV     EDX,0C430C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1D:  PUSHAD
        MOV     EDX,0C440C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1E:  PUSHAD
        MOV     EDX,0C450C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ1F:  PUSHAD
        MOV     EDX,0C460C31H
        JMP     IRQJOIN

        ALIGN   4
IRQ20_COMMON PROC NEAR
        MOV     AL,20H                  ;ACKNOWLEDGE INTERRUPT
        OUT     20H,AL
        INC     DWORD PTR JIFFIES
        MOV     EAX,CRT_SEL
        MOV     ES,EAX
        INC     BYTE PTR ES:[0]
        RET
IRQ20_COMMON ENDP

        ALIGN   4
IRQ20:  BT      DWORD PTR SYS_FLAGS,SYSF_PREEMPTIVE
        JC      @@1

        PUSH    EAX                     ;NONPREEMPTIVE
        PUSH    ES           
        CALL    IRQ20_COMMON
        POP     ES
        POP     EAX
        IRET
        
@@1:    CALL    SAVE_CONTEXT            ;PREEMPTIVE
        CALL    IRQ20_COMMON
        MOV     EBP,ESP
        MOV     EAX,[EBP]
        MOV     DWORD PTR FS:TSS_EIP,EAX
        MOV     EAX,8[EBP]
        MOV     DWORD PTR FS:TSS_EFLAGS,EAX
        ADD     EBP,12
        MOV     DWORD PTR FS:TSS_ESP,EBP
        
        MOV     ESI,FS:TSS_NEXT_TSS
        MOV     AX,WORD PTR TSS_FS[ESI]
        MOV     FS,AX
        JMP     SWAP_IN_NEXT_PART2

        ALIGN   4
IRQ21:  STI  
        PUSH    EAX
        PUSH    ES
        MOV     AX,SYS_DS_SEL
        MOV     ES,AX
        IN      AL,60H

        MOV     AH,AL
        MOV     AL,20H                  ;ACKNOWLEDGE INTERRUPT
        OUT     20H,AL
        MOVZX   EAX,AH

        CMP     EAX,02AH
        JE      @@11
        CMP     EAX,036H
        JNE     @@2
@@11:   BTS     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_SHIFT
        JMP     @@1

@@2:    CMP     EAX,0AAH
        JE      @@12
        CMP     EAX,0B6H
        JNE     @@3
@@12:   BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_SHIFT
        JMP     @@1

@@3:    CMP     EAX,01DH
        JNE     @@4
        BTS     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_CTRL
        JMP     @@1

@@4:    CMP     EAX,09DH
        JNE     @@5
        BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_CTRL
        JMP     @@1

@@5:    CMP     EAX,038H
        JNE     @@6
        BTS     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_ALT
        JMP     @@1

@@6:    CMP     EAX,0B8H
        JNE     @@5A
        BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_ALT
        JMP     @@1

@@5A:   CMP     EAX,05BH
        JNE     @@6A
        BTS     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_WIN
        JMP     @@1

@@6A:   CMP     EAX,0DBH
        JNE     @@7
        BTR     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_WIN
        JMP     @@1

@@7:    BT      DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_CTRL
        JNC     @@10
        BT      DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_ALT
        JNC     @@10

        CMP     EAX,053H         ;CTRL ALT DEL?
        JNE     @@8
        JMP     CMD_REBOOT

@@8:    CMP     EAX,01H          ;CTRL ALT ESC?
        JNE     @@9
        CALL    SPAWN_USER_AND_ACTIVATE
        JMP     @@1

@@9:    CMP     EAX,0FH          ;CTRL ALT TAB?
        JNE     @@13
        CALL    CMD_ACTIVATE_NEXT_USER
        JMP     @@1

@@13:   CMP     EAX,02DH          ;CTRL ALT BREAK
        JNE     @@10
        CALL    KILL_ACTIVE_USER
        JMP     @@1

@@10:   OR      AL,AL
        JS      @@1
        BTS     DWORD PTR SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY
@@1:    MOV     BYTE PTR SYS_CURRENT_SCAN_CODE,AL

        POP     ES
        POP     EAX
        IRET

        ALIGN   4
IRQ22:  PUSHAD
        MOV     EDX,09320932H
        JMP     IRQJOIN


        ALIGN   4
IRQ23:  PUSHAD
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_IRQ03*4[EAX]
        POPAD
        IRET

        ALIGN   4
IRQ24:  PUSHAD
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_IRQ04*4[EAX]
        POPAD
        IRET

        ALIGN   4
IRQ25:  PUSHAD
        MOV     EDX,09350932H
        JMP     IRQJOIN

        ALIGN   4
IRQ26:  PUSHAD
        MOV     EDX,09360932H
        JMP     IRQJOIN

        ALIGN   4
IRQ27:  PUSHAD
        MOV     EDX,09370932H
        JMP     IRQJOIN

        ALIGN   4
IRQ28:  PUSHAD
        MOV     EDX,09380932H
        JMP     IRQJOIN


        ALIGN   4
IRQ29:  PUSHAD
        MOV     EDX,09390932H
        JMP     IRQJOIN

        ALIGN   4
IRQ2A:  PUSHAD
        MOV     EDX,09410932H
        JMP     IRQJOIN


        ALIGN   4
IRQ2B:  PUSHAD
        MOV     EDX,09420932H
        JMP     IRQJOIN


IRQ2C:  ALIGN   4
        PUSHAD
        MOV     EDX,09430932H
        JMP     IRQJOIN

        ALIGN   4
IRQ2D:  PUSHAD
        MOV     EDX,09440932H
        JMP     IRQJOIN


        ALIGN   4
IRQ2E:  PUSHAD
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_IRQ0E*4[EAX]
        POPAD
        IRET

        ALIGN   4
IRQ2F:  PUSHAD
        MOV     EDX,09460932H
        JMP     IRQJOIN

;************************************ SOFTWARE INT'S ******************************

        ALIGN   4
IRQ30:  STI
        CALL    PUT_SYSTEXT
        IRET

        ALIGN   4
IRQ31:  STI
        CALL    PUT_CHAR
        IRET

        ALIGN   4
IRQ32:  STI
        CALL    GET_CHAR
        IRET

        ALIGN   4
IRQ33:  STI                     ;PRINTF
        PUSH    EBP
        MOV     EBP,ESP
        MOV     EBX,EAX         ;FETCH PARAM COUNT
        LEA     EBP,16-8[EBP+EBX*8]
@@20:   MOV     EDX,[EBP]
        MOV     EAX,4[EBP]
        CMP     BYTE PTR C_PTR_COUNT[EDX],1
        JE      @@25
        CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_DWORD*4),EDX
        JE      @@21
        CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_int*4),EDX
        JNE     @@21A
@@21:   CALL    PUT_HEX
        JMP     @@29
@@21A:  CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_WORD*4),EDX
        JE      @@22
        CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_short*4),EDX
        JNE     @@22A
@@22:   CALL    PUT_HEX_WORD
        JMP     @@29
@@22A:  CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_BYTE*4),EDX
        JNE     @@23
        CALL    PUT_HEX_BYTE
        JMP     @@29
@@23:   CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_char*4),EDX
        JNE     @@23A
        CALL    PUT_CHAR
        JMP     @@29
@@23A:  JMP     @@21

@@25:   SUB     EDX,C_STRUCT_SIZE
        CMP     DWORD PTR SYS_INTERNAL_TYPES+(IT_char*4),EDX
        JNE     @@21
        PUSH    ESI
        MOV     ESI,EAX
        CALL    PUT_STRING
        POP     ESI
        JMP     @@29

@@29:   SUB     EBP,8
        DEC     EBX
        JNZ     @@20
        POP     EBP
        IRET

        ALIGN   4
IRQ34:  PUSHAD
        MOV     EDX,09340933H
        JMP     IRQJOIN

        ALIGN   4
IRQ35:  PUSHAD
        MOV     EDX,09350933H
        JMP     IRQJOIN

        ALIGN   4
IRQ36:  PUSHAD
        MOV     EDX,09360933H
        JMP     IRQJOIN

        ALIGN   4
IRQ37:  PUSHAD
        MOV     EDX,09370933H
        JMP     IRQJOIN

        ALIGN   4
IRQ38:  PUSHAD
        MOV     EDX,09380933H
        JMP     IRQJOIN

        ALIGN   4
IRQ39:  PUSHAD
        MOV     EDX,09390933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3A:  PUSHAD
        MOV     EDX,09410933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3B:  PUSHAD
        MOV     EDX,09420933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3C:  PUSHAD
        MOV     EDX,09430933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3D:  PUSHAD
        MOV     EDX,09440933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3E:  PUSHAD
        MOV     EDX,09450933H
        JMP     IRQJOIN

        ALIGN   4
IRQ3F:  PUSHAD
        MOV     EDX,09460933H
        JMP     IRQJOIN


;******************** SOFTWARE IRQS ****************
        ALIGN   4
IRQ40:  PUSHAD
        MOV     EDX,09300934H
        JMP     IRQJOIN

        ALIGN   4
IRQ41:  PUSHAD
        MOV     EDX,09310934H
        JMP     IRQJOIN

        ALIGN   4
IRQ42:  PUSHAD
        MOV     EDX,09320934H
        JMP     IRQJOIN

        ALIGN   4
IRQ43:  PUSHAD
        MOV     EDX,09330934H
        JMP     IRQJOIN

        ALIGN   4
IRQ44:  PUSHAD
        MOV     EDX,09340934H
        JMP     IRQJOIN

        ALIGN   4
IRQ45:  PUSHAD
        MOV     EDX,09350934H
        JMP     IRQJOIN

        ALIGN   4
IRQ46:  PUSHAD
        MOV     EDX,09360934H
        JMP     IRQJOIN

        ALIGN   4
IRQ47:  PUSHAD
        MOV     EDX,09370934H
        JMP     IRQJOIN

        ALIGN   4
IRQ48:  PUSHAD
        MOV     EDX,09380934H
        JMP     IRQJOIN

        ALIGN   4
IRQ49:  PUSHAD
        MOV     EDX,09390934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4A:  PUSHAD
        MOV     EDX,09410934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4B:  PUSHAD
        MOV     EDX,09420934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4C:  PUSHAD
        MOV     EDX,09430934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4D:  PUSHAD
        MOV     EDX,09440934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4E:  PUSHAD
        MOV     EDX,09450934H
        JMP     IRQJOIN

        ALIGN   4
IRQ4F:  PUSHAD
        MOV     EDX,09460934H
        JMP     IRQJOIN

        ALIGN   4
IRQ50:  PUSHAD
        MOV     EDX,09300935H
        JMP     IRQJOIN

        ALIGN   4
IRQ51:  PUSHAD
        MOV     EDX,09310935H
        JMP     IRQJOIN

        ALIGN   4
IRQ52:  PUSHAD
        MOV     EDX,09320935H
        JMP     IRQJOIN

        ALIGN   4
IRQ53:  PUSHAD
        MOV     EDX,09330935H
        JMP     IRQJOIN

        ALIGN   4
IRQ54:  PUSHAD
        MOV     EDX,09340935H
        JMP     IRQJOIN

        ALIGN   4
IRQ55:  PUSHAD
        MOV     EDX,09350935H
        JMP     IRQJOIN

        ALIGN   4
IRQ56:  PUSHAD
        MOV     EDX,09360935H
        JMP     IRQJOIN

        ALIGN   4
IRQ57:  PUSHAD
        MOV     EDX,09370935H
        JMP     IRQJOIN

        ALIGN   4
IRQ58:  PUSHAD
        MOV     EDX,09380935H
        JMP     IRQJOIN

        ALIGN   4
IRQ59:  PUSHAD
        MOV     EDX,09390935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5A:  PUSHAD
        MOV     EDX,09410935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5B:  PUSHAD
        MOV     EDX,09420935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5C:  PUSHAD
        MOV     EDX,09430935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5D:  PUSHAD
        MOV     EDX,09440935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5E:  PUSHAD
        MOV     EDX,09450935H
        JMP     IRQJOIN

        ALIGN   4
IRQ5F:  PUSHAD
        MOV     EDX,09460935H
        JMP     IRQJOIN




        ALIGN   4
IRQJOIN:
        MOV     EAX,ST_INTERRUPT
        INT     I_PUT_SYSTEXT
        MOV     EAX,EDX
        CALL    CRT_OUT
        MOV     CL,16
        SHR     EAX,CL
        CALL    CRT_OUT
        CALL    CRLF
        CALL    DEBUG_SPIN_WAIT
        POPAD
        IRET

        ENDS    

        END
