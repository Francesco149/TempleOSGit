        INCLUDE OSINC.INC
        EXTRN   FIXED_PUT_HEX:NEAR
        EXTRN   PUT_HEX:NEAR
        EXTRN   PUT_STRING:NEAR
        EXTRN   PUT_HEX_BYTE:NEAR
        EXTRN   DEBUG_SPIN_WAIT:NEAR
        EXTRN   SPACE:NEAR
        EXTRN   CRLF:NEAR
        EXTRN   MALLOC:NEAR
        EXTRN   GET_KEY:NEAR
        EXTRN   GET_STRING:NEAR
        EXTRN   FREE:NEAR
        EXTRN   NEW_STRING:NEAR
        EXTRN   STRLEN:NEAR
        EXTRN   INSQUE:NEAR
        EXTRN   INSQUE_REV:NEAR
        EXTRN   REMQUE:NEAR

;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE
;;**************************PROCEDURE*************************
; IN:   ESI=STRING TO CONVERT
;       EDI=LINKED TEXT FILE
        PUBLIC  TO_LINKED_TEXT_FILE
TO_LINKED_TEXT_FILE PROC
        PUSHAD
        XOR     EAX,EAX
        MOV     ECX,EDI

@@1:    PUSH    ESI
        MOV     EAX,LTFL_STRUCT_SIZE
        CALL    MALLOC
        MOV     DWORD PTR LTFL_FLAGS[ESI],0
        MOV     EDX,ESI                 ;EDX=NEW NODE
        POP     ESI

        MOV     EBX,ESI
@@2:    LODSB
        OR      AL,AL
        JNZ     @@10
        DEC     ESI
        CMP     ESI,EBX
        JNE     @@5
        MOV     ESI,EDX                 ;DONE (NO NEW LINE)
        CALL    FREE
        JMP     @@100

@@5:    XCHG    ESI,EBX                 ;DONE BUT ONE LINE REMAINS
        CALL    NEW_STRING
        XCHG    EBX,ESI
        MOV     DWORD PTR LTFL_LINE_DATA[EDX],EDI
        PUSH    ESI
        MOV     ESI,EDX
        MOV     EDI,DWORD PTR LTF_INSERT_POINT[ECX]
        CALL    INSQUE_REV
        INC     LTF_NUM_LINES[ECX]
        POP     ESI
        JMP     @@100
           
@@10:   CMP     AL,13
        JE      @@11
        CMP     AL,10
        JNE     @@2     
@@11:   DEC     ESI                     ;INSERT NEW LINE AND CONTINUE
        MOV     BYTE PTR [ESI],0
        XCHG    ESI,EBX
        CALL    NEW_STRING
        XCHG    EBX,ESI
        MOV     BYTE PTR [ESI],AL
        INC     ESI
        CMP     AL,10
        JE      @@14
@@12:   LODSB           ;FIRST WAS CR
        CMP     AL,13
        JE      @@12
        CMP     AL,10
        JNE     @@15
@@14:   LODSB           ;FIRST WAS LF
        CMP     AL,13
        JE      @@14
@@15:   DEC     ESI

@@16:   MOV     DWORD PTR LTFL_LINE_DATA[EDX],EDI
        PUSH    ESI
        MOV     ESI,EDX
        MOV     EDI,DWORD PTR LTF_INSERT_POINT[ECX]
        CALL    INSQUE_REV
        INC     LTF_NUM_LINES[ECX]
        POP     ESI
        JMP     @@1
            
@@100:  MOV     EDI,ECX
        MOV     EAX,DWORD PTR LTF_NEXT[EDI]
        MOV     DWORD PTR LTF_CUR_LINE[EDI],EAX
        MOV     DWORD PTR LTF_CUR_TOP_LINE_NUM[EDI],1
        MOV     DWORD PTR LTF_CUR_TOP_LINE[EDI],EAX
        POPAD
        RET
TO_LINKED_TEXT_FILE ENDP
        ENDS
        END
