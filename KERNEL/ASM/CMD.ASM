        INCLUDE OSINC.INC
        INCLUDE COMPILE.INC
        EXTRN   SYSTEM_EXTERN_TABLE:DWORD
        EXTRN   ROOT_TSS:DWORD
        EXTRN   SWAP_IN_NEXT:NEAR
        EXTRN   FREE:NEAR
        EXTRN   RFREE:NEAR
        EXTRN   CRLF:NEAR
        EXTRN   DEBUG_SPIN_WAIT:NEAR
        EXTRN   MALLOC:NEAR
        EXTRN   NEW_STRING:NEAR
        EXTRN   RMALLOC:NEAR
        EXTRN   RNEW_STRING:NEAR
        EXTRN   FIND_HASH_BUCKET:NEAR
        EXTRN   STRLEN:NEAR
        EXTRN   LEX:NEAR
        EXTRN   NEW_LEX:NEAR
        EXTRN   DELETE_LEX:NEAR
        EXTRN   PUT_HEX:NEAR
        EXTRN   PUT_STRING:NEAR
        EXTRN   COMPILE_EXPRESSION:NEAR
        EXTRN   EVAL_EXPRESSION:NEAR
        EXTRN   GET_EXPRESSION:NEAR
        EXTRN   PUT_EXPRESSION:NEAR
        EXTRN   SPAWN_TASK:NEAR
        EXTRN   SYS_FLAGS:DWORD
        EXTRN   SYS_HASH_TABLE:DWORD
        EXTRN   FIND_HASH_ENTRY:NEAR
        EXTRN   INIT_CRT:NEAR
        EXTRN   CLEAR_CRT:NEAR
        EXTRN   CURSOR_HOME:NEAR
        EXTRN   GET_STRING:NEAR
        EXTRN   CURSOR_OFF:NEAR
        EXTRN   HEX_DUMP:NEAR
        EXTRN   COMPILE_STATEMENT:NEAR
        EXTRN   RUN_STATEMENT:NEAR
        EXTRN   END_TASK:NEAR
        EXTRN   ZEROS:DWORD
        EXTRN   ACTIVATE_NEXT_USER      :NEAR

ROOT_CMD_QUEUE_NODE STRUC
RQ_NEXT         DD  ?
RQ_CMD          DD  ?
RQ_STRUCT_SIZE  EQU $
        ENDS

;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE
ROOT_COMMAND:
        DD      ?
ROOT_COMMAND_RETURN:
        DD      ?
ROOT_COMMAND_QUEUE:
        DD      ?
ROOT_QUEUE_LAST:
        DD      ?

ROOT_START_CMD:
        DB '#include "FIL_ROOT_CPP"',13,10,';',0
USER_START_CMD:
        DB '#include "/HOPPY/KERNEL/CPP/USER.CPP"',13,10,';',0
SYSTEM_START_CMD:
        DB '#include "/HOPPY/KERNEL/CPP/SYSTEM.CPP"',13,10,';',0

;;**************************PROCEDURE*************************
        PUBLIC  CMD_SPAWN
        ALIGN   4
CMD_SPAWN PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        OR      EAX,EAX
        JNZ     @@10
        MOV     EAX,ST_INVALID_TASK_ADDRESS
        INT     I_PUT_SYSTEXT
        XOR     EAX,EAX
        JMP     @@100
@@10:   PUSHFD
        CLI
        MOV     ESI,DWORD PTR SF_PARAM2[EBP]
        CALL    SPAWN_TASK
        MOV     ECX,DWORD PTR TSS_HASH_TABLE[EDI]
        MOV     EAX,DWORD PTR FS:TSS_HASH_TABLE
        MOV     DWORD PTR HT_NEXT[ECX],EAX
        POPFD
        MOV     EAX,EDI
@@100:  POP     EDI
        POP     ESI
        POP     ECX
        POP     EBP
        RET
CMD_SPAWN ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_QSYSTEM
        ALIGN   4
CMD_QSYSTEM PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,SF_PARAM1[EBP]
        OR      ESI,ESI
        JZ      @@100
        CALL    RNEW_STRING
        CALL    FREE
        PUSHFD
        PUSH    EDI
        CLI
        LEA     EAX,QSYSTEM_COMMAND_LINE
        MOV     ESI,SF_PARAM1[EBP]
@@10:   CALL    SPAWN_TASK
        POP     DWORD PTR TSS_STARTUP_CMD[EDI]
        MOV     ECX,DWORD PTR TSS_HASH_TABLE[EDI]
        MOV     EAX,DWORD PTR FS:TSS_HASH_TABLE
        MOV     DWORD PTR HT_NEXT[ECX],EAX
        MOV     EAX,EDI
        POPFD
@@100:  POP     EDI
        POP     ESI
        POP     ECX
        POP     EBP
        RET
CMD_QSYSTEM ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_SYSTEM
        ALIGN   4
CMD_SYSTEM PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ECX
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,SF_PARAM1[EBP]
        OR      ESI,ESI
        JZ      @@100
        CALL    RNEW_STRING
        CALL    FREE
        PUSHFD
        PUSH    EDI
        CLI
        LEA     EAX,SYSTEM_COMMAND_LINE
        MOV     ESI,SF_PARAM1[EBP]
@@10:   CALL    SPAWN_TASK
        POP     DWORD PTR TSS_STARTUP_CMD[EDI]
        MOV     ECX,DWORD PTR TSS_HASH_TABLE[EDI]
        MOV     EAX,DWORD PTR FS:TSS_HASH_TABLE
        MOV     DWORD PTR HT_NEXT[ECX],EAX
        MOV     EAX,EDI
        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_SUSPENDED
        POPFD
        CALL    SWAP_IN_NEXT
        MOV     EAX,DWORD PTR FS:TSS_SYSTEM_RESULT
@@100:  POP     EDI
        POP     ESI
        POP     ECX
        POP     EBP
        RET
CMD_SYSTEM ENDP
;;**************************PROCEDURE*************************
; IN:   EDI=LEX NODE
        ALIGN   4
        PUBLIC  EXECUTE_COMMAND_LINE
EXECUTE_COMMAND_LINE PROC NEAR
        PUSH    ESI
@@1:    CMP     DWORD PTR LEX_TOKEN[EDI],TK_IDENT
        JNE     @@10
        MOV     ESI,DWORD PTR LEX_HASH_ENTRY[EDI]
        OR      ESI,ESI
        JZ      @@10
        CMP     DWORD PTR HTE_TYPE[ESI],HTT_KEYWORD
        JNE     @@10
        CMP     DWORD PTR HTE_USER0[ESI],KW_EXIT
        JE      @@100
@@10:   PUSH    ESI
        PUSH    EDI
        PUSH    ECX
        MOV     ESI,LEX_IDENT[EDI]
        MOV     ECX,TSS_DESC_LENGTH
        MOV     EDI,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        ADD     EDI,TSS_TASK_DESCRIPTOR
        REP MOVSB
        XOR     EAX,EAX
        STOSB
        POP     ECX
        POP     EDI
        POP     ESI

        XOR     EBP,EBP
        CALL    COMPILE_STATEMENT
        JC      @@20

        PUSH    ESI
        CALL    RUN_STATEMENT
        POP     ESI
        CALL    FREE
@@20:   CMP     DWORD PTR LEX_TOKEN[EDI],TK_EOF
        JNE     @@1
@@100:  POP     ESI
        RET
EXECUTE_COMMAND_LINE ENDP

;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  COMMAND_LINE
COMMAND_LINE PROC NEAR
        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_LOCAL_USER
        CALL    INIT_CRT
        CALL    CURSOR_OFF
        MOV     BYTE PTR FS:TSS_TEXT_ATTRIBUTE,0F1H
        MOV     BYTE PTR FS:TSS_BORDER_ATTRIBUTE,0F9H
        BTS     WORD PTR FS:TSS_CRT_FLAGS,CRTF_WRAP_AROUND
        MOV     DWORD PTR FS:TSS_WINDOW_TOP,13
        CALL    CLEAR_CRT
        CALL    CURSOR_HOME
        MOV     EAX,DWORD PTR SYSTEM_EXTERN_TABLE
        CALL    DWORD PTR EXT_SELECT_DEFAULT_DRIVE*4[EAX]
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_ECHO
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_LEX_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_PARSE_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_OPT_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_COMPILE_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_RUN_TRACE
        LEA     ESI,BYTE PTR USER_START_CMD
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     EDX,LF_PROMPT+LF_DOUBLE_SEMICOLON
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     ESI,DWORD PTR FS:TSS_ABSOLUTE_ADDRESS
        CALL    ACTIVATE_NEXT_USER
        JMP     END_TASK
COMMAND_LINE ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  QSYSTEM_COMMAND_LINE
QSYSTEM_COMMAND_LINE PROC NEAR
        LEA     ESI,BYTE PTR SYSTEM_START_CMD
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     ESI,DWORD PTR FS:TSS_STARTUP_CMD
        MOV     DWORD PTR FS:TSS_STARTUP_CMD,0
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     ESI,DWORD PTR FS:TSS_STARTUP_CMD
        CALL    RFREE
        JMP     END_TASK
QSYSTEM_COMMAND_LINE ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  SYSTEM_COMMAND_LINE
SYSTEM_COMMAND_LINE PROC NEAR
        LEA     ESI,BYTE PTR SYSTEM_START_CMD
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     ESI,DWORD PTR FS:TSS_STARTUP_CMD
        MOV     DWORD PTR FS:TSS_STARTUP_CMD,0
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        MOV     ESI,DWORD PTR FS:TSS_PARENT_TSS
        MOV     DWORD PTR TSS_SYSTEM_RESULT[ESI],EAX
        BTR     WORD PTR TSS_TASK_FLAGS[ESI],TSSF_SUSPENDED
        CALL    DELETE_LEX
        MOV     ESI,DWORD PTR FS:TSS_STARTUP_CMD
        CALL    RFREE
        JMP     END_TASK
SYSTEM_COMMAND_LINE ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_ROOT
CMD_ROOT PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     EAX,SF_PARAM1[EBP]
        JMP     @@2
@@1:    CALL    SWAP_IN_NEXT
@@2:    BTS     DWORD PTR SYS_FLAGS,SYSF_ROOT_BUSY
        JC      @@1
        BTC     DWORD PTR SYS_FLAGS,SYSF_ROOT_DONE
        MOV     DWORD PTR ROOT_COMMAND,EAX
@@3:    CALL    SWAP_IN_NEXT
        BTC     DWORD PTR SYS_FLAGS,SYSF_ROOT_DONE
        JNC     @@3
        MOV     EAX,DWORD PTR ROOT_COMMAND_RETURN
        BTC     DWORD PTR SYS_FLAGS,SYSF_ROOT_BUSY
        MOV     ESI,SF_PARAM1[EBP]
        CALL    FREE
        POP     ESI
        POP     EBP
        RET
CMD_ROOT ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  CMD_QUEUE_ROOT
CMD_QUEUE_ROOT PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,SF_PARAM1[EBP]
        CALL    RNEW_STRING
        CALL    FREE
        MOV     EAX,RQ_STRUCT_SIZE
        CALL    RMALLOC
        MOV     DWORD PTR RQ_CMD[ESI],EDI
        MOV     DWORD PTR RQ_NEXT[ESI],0
@@1:    BTS     DWORD PTR SYS_FLAGS,SYSF_ROOT_QUEUE_IN_USE
        JNC     @@2
        CALL    SWAP_IN_NEXT
        JMP     @@1
@@2:    MOV     EDI,DWORD PTR ROOT_QUEUE_LAST
        MOV     DWORD PTR RQ_NEXT[EDI],ESI
        MOV     DWORD PTR ROOT_QUEUE_LAST,ESI
        BTR     DWORD PTR SYS_FLAGS,SYSF_ROOT_QUEUE_IN_USE
        POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_QUEUE_ROOT ENDP
;;**************************PROCEDURE*************************
        ALIGN   4
        PUBLIC  ROOT_COMMAND_LINE
ROOT_COMMAND_LINE PROC NEAR
        CALL    INIT_CRT
        CALL    CURSOR_OFF
        MOV     BYTE PTR FS:TSS_TEXT_ATTRIBUTE,0F5H
        MOV     BYTE PTR FS:TSS_BORDER_ATTRIBUTE,0FDH
        MOV     BYTE PTR FS:TSS_MENU_ATTRIBUTE,0F2H
        BTS     WORD PTR FS:TSS_CRT_FLAGS,CRTF_WRAP_AROUND
        INC     DWORD PTR FS:TSS_WINDOW_TOP
        CALL    CLEAR_CRT
        CALL    CURSOR_HOME
        MOV     EAX,ST_ROOT_STARTUP
        INT     I_PUT_SYSTEXT
        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_ECHO
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_MALLOC_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_LEX_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_PARSE_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_OPT_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_COMPILE_TRACE
;        BTS     WORD PTR FS:TSS_TASK_FLAGS,TSSF_RUN_TRACE
        LEA     ESI,BYTE PTR ROOT_START_CMD
        MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        MOV     DWORD PTR ROOT_COMMAND,0
        MOV     DWORD PTR ROOT_COMMAND_QUEUE,0
        LEA     EAX,DWORD PTR ROOT_COMMAND_QUEUE
        MOV     DWORD PTR ROOT_QUEUE_LAST,EAX
@@1:    MOV     ESI,DWORD PTR ROOT_COMMAND
        OR      ESI,ESI
        JNZ     @@2
        BTS     DWORD PTR SYS_FLAGS,SYSF_ROOT_QUEUE_IN_USE
        JC      @@3
        MOV     ESI,DWORD PTR ROOT_COMMAND_QUEUE
        OR      ESI,ESI
        JZ      @@4
        CMP     DWORD PTR ROOT_QUEUE_LAST,ESI
        JNE     @@5
        LEA     EAX,DWORD PTR ROOT_COMMAND_QUEUE
        MOV     DWORD PTR ROOT_QUEUE_LAST,EAX
@@5:    MOV     EAX,DWORD PTR RQ_NEXT[ESI]
        MOV     DWORD PTR ROOT_COMMAND_QUEUE,EAX
        BTR     DWORD PTR SYS_FLAGS,SYSF_ROOT_QUEUE_IN_USE
        MOV     EDI,DWORD PTR RQ_CMD[ESI]
        CALL    FREE        
        XOR     EDX,EDX     ;FREE BUFFER
        MOV     ESI,EDI
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        CALL    DELETE_LEX
        JMP     @@1
@@4:    BTR     DWORD PTR SYS_FLAGS,SYSF_ROOT_QUEUE_IN_USE
@@3:    CALL    SWAP_IN_NEXT
        JMP     @@1
@@2:    MOV     EDX,LF_DONT_FREE_BUFFER
        CALL    NEW_LEX
        CALL    LEX
        CALL    EXECUTE_COMMAND_LINE
        MOV     DWORD PTR ROOT_COMMAND_RETURN,EAX
        XOR     EAX,EAX
        MOV     DWORD PTR ROOT_COMMAND,EAX
        BTS     DWORD PTR SYS_FLAGS,SYSF_ROOT_DONE
        CALL    DELETE_LEX
        JMP     @@1
ROOT_COMMAND_LINE ENDP

        ENDS
        END
