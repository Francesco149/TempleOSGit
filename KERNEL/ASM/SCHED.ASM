        INCLUDE OSINC.INC
        EXTRN   ROOT_TSS:DWORD
        EXTRN   SPAWN_TASK:NEAR
        EXTRN   COMMAND_LINE:NEAR
        EXTRN   INSQUE:NEAR,            REMQUE:NEAR
        EXTRN   DEBUG_SPIN_WAIT:NEAR
        EXTRN   FREE_BLOCK_LIST:NEAR
        EXTRN   FREE:NEAR
        EXTRN   RFREE:NEAR
        EXTRN   GDTTAB:DWORD
        EXTRN   SYSTEM_EXTERN_TABLE:DWORD
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

;;**************************PROCEDURE*************************
; IN:   EDI = @TSS TO ADD TO QUEUE
;
        PUBLIC  ADD_TASK_TO_QUEUE
        ALIGN   4
ADD_TASK_TO_QUEUE       PROC NEAR
        PUSH    EBX
        PUSH    ESI
        PUSHFD
        CLI
        MOV     ESI,FS:TSS_LAST_TSS     ;INSQUE
        MOV     EBX,FS:TSS_ABSOLUTE_ADDRESS
        MOV     TSS_NEXT_TSS[ESI],EDI
        MOV     TSS_LAST_TSS[EBX],EDI
        MOV     TSS_LAST_TSS[EDI],ESI
        MOV     TSS_NEXT_TSS[EDI],EBX
        POPFD        
        POP     ESI
        POP     EBX
        RET
ADD_TASK_TO_QUEUE       ENDP

;;**************************PROCEDURE*************************
        PUBLIC  SAVE_CONTEXT    ;less EIP and ESP
        ALIGN   4
SAVE_CONTEXT    PROC NEAR
        PUSHFD
        POP     DWORD PTR FS:TSS_EFLAGS
        MOV     DWORD PTR FS:TSS_EAX,EAX
        MOV     DWORD PTR FS:TSS_EBX,EBX
        MOV     DWORD PTR FS:TSS_ECX,ECX
        MOV     DWORD PTR FS:TSS_EDX,EDX
        MOV     DWORD PTR FS:TSS_ESI,ESI
        MOV     DWORD PTR FS:TSS_EDI,EDI
        MOV     DWORD PTR FS:TSS_EBP,EBP

        PUSH    EAX

;        XOR     EAX,EAX
;        MOV     AX,FS:TSS_SELECTOR
;        ADD     EAX,GDTTAB
;        BTC     DWORD PTR [EAX],41

        MOV     AX,SS
        MOV     WORD PTR FS:TSS_SS0,AX
        MOV     WORD PTR FS:TSS_SS,AX
        MOV     AX,CS
        MOV     WORD PTR FS:TSS_CS,AX
        MOV     AX,DS
        MOV     WORD PTR FS:TSS_DS,AX
        MOV     AX,ES
        MOV     WORD PTR FS:TSS_ES,AX
        MOV     AX,FS
        MOV     WORD PTR FS:TSS_FS,AX
        MOV     AX,GS
        MOV     WORD PTR FS:TSS_GS,AX
;        SLDT    WORD PTR FS:TSS_LDTR
        POP     EAX
        RET
SAVE_CONTEXT    ENDP

;;**************************PROCEDURE*************************
        PUBLIC  RESTORE_CONTEXT    ;From FS
        ALIGN   4
RESTORE_CONTEXT    PROC NEAR
        MOV     EBX,DWORD PTR FS:TSS_EBX
        MOV     ECX,DWORD PTR FS:TSS_ECX
        MOV     EDX,DWORD PTR FS:TSS_EDX
        MOV     ESI,DWORD PTR FS:TSS_ESI
        MOV     EDI,DWORD PTR FS:TSS_EDI
        MOV     EBP,DWORD PTR FS:TSS_EBP

        MOV    AX,WORD PTR FS:TSS_SS
        MOV    SS,AX
        MOV    AX,WORD PTR FS:TSS_DS
        MOV    DS,AX
        MOV    AX,WORD PTR FS:TSS_ES
        MOV    ES,AX
;        MOV    AX,WORD PTR FS:TSS_FS
;        MOV    FS,AX
        MOV     AX,WORD PTR FS:TSS_GS
        MOV     GS,AX

;        LLDT    WORD PTR FS:TSS_LDTR
;        LTR     WORD PTR FS:TSS_SELECTOR
        MOV     ESP,DWORD PTR FS:TSS_ESP
        MOV     EAX,DWORD PTR FS:TSS_EAX
        PUSH    DWORD PTR FS:TSS_EFLAGS
        PUSH    DWORD PTR FS:TSS_CS
        PUSH    DWORD PTR FS:TSS_EIP
        IRET
RESTORE_CONTEXT    ENDP

;;**************************PROCEDURE*************************
        PUBLIC  END_TASK
        ALIGN   4
END_TASK PROC NEAR
        CLI
        MOV     ESI,FS:TSS_MEMORY_BLOCK_LIST
        CALL    FREE_BLOCK_LIST

        MOV     ESI,FS:TSS_NEXT_TSS
        MOV     EDI,FS:TSS_LAST_TSS
        MOV     TSS_NEXT_TSS[EDI],ESI
        MOV     TSS_LAST_TSS[ESI],EDI

        PUSH    ESI
        MOV     ESI,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        CALL    RFREE
        POP     ESI

        MOV     AX,WORD PTR TSS_FS[ESI]
        MOV     FS,AX
        JMP     SWAP_IN_NEXT_PART2
END_TASK ENDP
;;**************************PROCEDURE*************************
        PUBLIC  KILL_TASK
;IN     EDI=TASK
        ALIGN   4
KILL_TASK PROC NEAR
        PUSH    ESI
        PUSHFD
        CLI
        MOV     ESI,TSS_MEMORY_BLOCK_LIST[EDI]
        CALL    FREE_BLOCK_LIST

        PUSH    EDI
        MOV     ESI,TSS_NEXT_TSS[EDI]
        MOV     EDI,TSS_LAST_TSS[EDI]
        MOV     TSS_NEXT_TSS[EDI],ESI
        MOV     TSS_LAST_TSS[ESI],EDI
        POP     EDI

        MOV     ESI,EDI
        CALL    RFREE
        POPFD
        POP     ESI
        RET
KILL_TASK ENDP
;;**************************PROCEDURE*************************
        PUBLIC  REMQUE_TASK
;IN     EDI=TASK
        ALIGN   4
REMQUE_TASK PROC NEAR
        PUSH    ESI
        PUSHFD
        CLI

        PUSH    EDI
        MOV     ESI,TSS_NEXT_TSS[EDI]
        MOV     EDI,TSS_LAST_TSS[EDI]
        MOV     TSS_NEXT_TSS[EDI],ESI
        MOV     TSS_LAST_TSS[ESI],EDI
        POP     EDI

        POPFD
        POP     ESI
        RET
REMQUE_TASK ENDP
;;**************************PROCEDURE*************************
        PUBLIC  SWAP_IN_NEXT,SWAP_IN_NEXT_PART2
        ALIGN   4
SWAP_IN_NEXT    PROC NEAR
        PUSHFD
        CLI
        CALL    SAVE_CONTEXT
        LEA     EAX,@@1
        MOV     DWORD PTR FS:TSS_EIP,EAX
        POP     DWORD PTR FS:TSS_EFLAGS
        MOV     DWORD PTR FS:TSS_ESP,ESP
@@10:   MOV     ESI,FS:TSS_NEXT_TSS
        MOV     AX,WORD PTR TSS_FS[ESI]
        MOV     FS,AX
SWAP_IN_NEXT_PART2:
        BT      WORD PTR FS:TSS_TASK_FLAGS,TSSF_SUSPENDED
        JC      @@10
        JMP     RESTORE_CONTEXT
@@1:    RET
SWAP_IN_NEXT    ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
CMD_DESC DB     'USER CMD PROMPT',0
        PUBLIC  SPAWN_USER_AND_ACTIVATE
SPAWN_USER_AND_ACTIVATE PROC NEAR
        PUSHFD
        CLI
        PUSHAD
        CALL    NEAR PTR DEACTIVATE_USER
@@18:   LEA     EAX,COMMAND_LINE
        LEA     ESI,BYTE PTR CMD_DESC
        CALL    SPAWN_TASK      ;This could hang on the RMALLOC
        LEA     ESI,DWORD PTR ROOT_TSS
        MOV     ESI,DWORD PTR TSS_HASH_TABLE[ESI]
        MOV     EDI,DWORD PTR TSS_HASH_TABLE[EDI]
        MOV     DWORD PTR HT_NEXT[EDI],ESI
        POPAD
        POPFD
        RET
SPAWN_USER_AND_ACTIVATE ENDP
;;**************************PROCEDURE*************************
;IN:    ESI=CURRENT USER TSS
        ALIGN   4
        PUBLIC  ACTIVATE_NEXT_USER
ACTIVATE_NEXT_USER PROC NEAR
        PUSHFD
        CLI
        PUSH    EDI
        MOV     ESI,DWORD PTR TSS_NEXT_TSS[ESI]
        MOV     EDI,ESI
@@14:   BT      DWORD PTR TSS_TASK_FLAGS[ESI],TSSF_LOCAL_USER
        JNC     @@19
        BTR     DWORD PTR TSS_TASK_FLAGS[ESI],TSSF_SUSPENDED
        JC      @@19B
@@19:   MOV     ESI,DWORD PTR TSS_NEXT_TSS[ESI]
        CMP     ESI,EDI
        JNE     @@14
@@19B:  POP     EDI
        POPFD
        RET
ACTIVATE_NEXT_USER ENDP
;;**************************PROCEDURE*************************
;OUT:   ESI=TSS OF USER
        ALIGN   4
        PUBLIC  DEACTIVATE_USER
DEACTIVATE_USER PROC NEAR
        PUSHFD
        CLI
        PUSH    EDI
        MOV     ESI,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        MOV     EDI,ESI
@@11:   BT      WORD PTR TSS_TASK_FLAGS[ESI],TSSF_SUSPENDED
        JC      @@12
        BT      WORD PTR TSS_TASK_FLAGS[ESI],TSSF_LOCAL_USER
        JC      @@13
@@12:   MOV     ESI,DWORD PTR TSS_NEXT_TSS[ESI]
        CMP     ESI,EDI
        JNE     @@11
        JMP     @@13A
@@13:   BTS     WORD PTR TSS_TASK_FLAGS[ESI],TSSF_SUSPENDED
@@13A:  POP     EDI
        POPFD
        RET
DEACTIVATE_USER ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  KILL_ACTIVE_USER
KILL_ACTIVE_USER PROC NEAR
        PUSHFD
        CLI
        CALL    SAVE_CONTEXT
        LEA     EAX,@@1
        MOV     DWORD PTR FS:TSS_EIP,EAX
        POP     DWORD PTR FS:TSS_EFLAGS
        MOV     DWORD PTR FS:TSS_ESP,ESP
        CALL    DEACTIVATE_USER
        PUSH    ESI
        CALL    ACTIVATE_NEXT_USER
        POP     ESI
        BT      WORD PTR TSS_TASK_FLAGS[ESI],TSSF_LOCAL_USER
        JNC     @@2
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_BLANK_CRT_TEXT*4[EAX]
        MOV     EAX,TSS_FS[ESI]
        MOV     FS,AX
        JMP     END_TASK        ;No Return
@@1:    RET
@@2:    JMP     RESTORE_CONTEXT
KILL_ACTIVE_USER ENDP
;;**************************PROCEDURE*************************
;IN:    ESI=CURRENT USER TSS
        ALIGN   4
        PUBLIC  CMD_ACTIVATE_NEXT_USER
CMD_ACTIVATE_NEXT_USER PROC NEAR
        PUSH    ESI
        CALL    DEACTIVATE_USER
        CALL    ACTIVATE_NEXT_USER
        POP     ESI
        RET
CMD_ACTIVATE_NEXT_USER ENDP
        ENDS
        END
