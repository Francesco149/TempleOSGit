        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        EXTRN   JIFFIES:DWORD
        EXTRN   SYS_HASH_TABLE:DWORD
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE
;;**************************PROCEDURE*************************
        PUBLIC  CMD_RMALLOC
        ALIGN   4
CMD_RMALLOC PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     EAX,SF_PARAM1[EBP]
        CALL    RMALLOC
        MOV     EAX,ESI
        POP     ESI
        POP     EBP
        RET
CMD_RMALLOC ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_RFREE
        ALIGN   4
CMD_RFREE PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,SF_PARAM1[EBP]
        CALL    RFREE
        POP     ESI
        POP     EBP
        RET
CMD_RFREE ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_MALLOC
        ALIGN   4
CMD_MALLOC PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     EAX,SF_PARAM1[EBP]
        CALL    MALLOC
        MOV     EAX,ESI
        POP     ESI
        POP     EBP
        RET
CMD_MALLOC ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_FREE
        ALIGN   4
CMD_FREE PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,SF_PARAM1[EBP]
        CALL    FREE
        POP     ESI
        POP     EBP
        RET
CMD_FREE ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_FIND_HASH_ENTRY
        ALIGN   4
CMD_FIND_HASH_ENTRY PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDI
        PUSH    ESI
        MOV     ESI,SF_PARAM1[EBP]
        MOV     EDI,SF_PARAM2[EBP]
        MOV     EBX,SF_PARAM3[EBP]
        CALL    FIND_HASH_ENTRY
        MOV     EAX,ESI
        POP     ESI
        POP     EDI
        POP     EBP
        RET
CMD_FIND_HASH_ENTRY ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_KILL
        ALIGN   4
CMD_KILL PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDI
        MOV     EDI,DWORD PTR SF_PARAM1[EBP]
        CALL    KILL_TASK
        POP     EDI
        POP     EBP
        RET
CMD_KILL ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_TO_LINKED_TEXT_FILE
        ALIGN   4
CMD_TO_LINKED_TEXT_FILE PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDI
        PUSH    ESI
        MOV     EDI,DWORD PTR SF_PARAM2[EBP]
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    TO_LINKED_TEXT_FILE
        POP     ESI
        POP     EDI
        POP     EBP
        RET
CMD_TO_LINKED_TEXT_FILE ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_CRT_LINE_OUT
        ALIGN   4
CMD_CRT_LINE_OUT PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    CRT_LINE_OUT
        POP     ESI
        POP     EBP
        RET
CMD_CRT_LINE_OUT ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_DELETE_LINKED_LIST
        ALIGN   4
CMD_DELETE_LINKED_LIST PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    DELETE_LINKED_LIST
        POP     ESI
        POP     EBP
        RET
CMD_DELETE_LINKED_LIST ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_ADD_SYS_HASH
        ALIGN   4
CMD_ADD_SYS_HASH PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDI
        MOV     EBP,SF_PARAM1[EBP]
        MOV     ESI,HTE_STRING[EBP]
        MOV     EDI,DWORD PTR SYS_HASH_TABLE
        CALL    FIND_HASH_BUCKET
        MOV     EAX,DWORD PTR [ESI]
        MOV     DWORD PTR HTE_NEXT[EBP],EAX
        MOV     DWORD PTR [ESI],EBP
        POP     EDI
        POP     EBP
        RET
CMD_ADD_SYS_HASH ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_SLEEP
        ALIGN   4
CMD_SLEEP PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDX
        PUSH    EBX
        MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        MOV     EBX,184
        MUL     EBX
        MOV     EBX,10000
        DIV     EBX
        ADD     EAX,DWORD PTR JIFFIES
@@1:    CALL    SWAP_IN_NEXT
        CMP     EAX,DWORD PTR JIFFIES
        JA      @@1
        POP     EBX
        POP     EDX
        POP     EBP
        RET
CMD_SLEEP ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_LOAD_TEXT_FILE
        ALIGN   4
CMD_LOAD_TEXT_FILE PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    LOAD_TEXT_FILE
        MOV     EAX,EDI
        POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_LOAD_TEXT_FILE ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_NEW_STRING
        ALIGN   4
CMD_NEW_STRING PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    NEW_STRING
        MOV     EAX,EDI
        POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_NEW_STRING ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_RNEW_STRING
        ALIGN   4
CMD_RNEW_STRING PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    RNEW_STRING
        MOV     EAX,EDI
        POP     EDI
        POP     ESI
        POP     EBP
        RET
CMD_RNEW_STRING ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_STRLEN
        ALIGN   4
CMD_STRLEN PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    STRLEN
        POP     ESI
        POP     EBP
        RET
CMD_STRLEN ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_PUT_CHAR
        ALIGN   4
CMD_PUT_CHAR PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        MOV     EAX,DWORD PTR SF_PARAM1[EBP]
        INT     I_PUT_CHAR
        POP     EBP
        RET
CMD_PUT_CHAR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_GET_CHAR
        ALIGN   4
CMD_GET_CHAR PROC NEAR
        INT     I_GET_CHAR
        RET
CMD_GET_CHAR ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_PUT_STRING
        ALIGN   4
CMD_PUT_STRING PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    ESI
        MOV     ESI,DWORD PTR SF_PARAM1[EBP]
        CALL    PUT_STRING
        POP     ESI
        POP     EBP
        RET
CMD_PUT_STRING ENDP
;;**************************PROCEDURE*************************
        PUBLIC  CMD_GET_STRING
        ALIGN   4
CMD_GET_STRING PROC NEAR
        PUSH    EBP
        MOV     EBP,ESP
        PUSH    EDI
        PUSH    ECX
        MOV     EDI,DWORD PTR SF_PARAM1[EBP]
        MOV     ECX,DWORD PTR SF_PARAM2[EBP]
        CALL    GET_STRING
        POP     ECX
        POP     EDI
        POP     EBP
        RET
CMD_GET_STRING ENDP
        ENDS
        END


