        INCLUDE OSINC.INC
        INCLUDE OSINC2.INC
        EXTRN   MEMORY_FREE_LIST:DWORD
        EXTRN   SYS_FLAGS:DWORD
        EXTRN   SYS_CURRENT_SCAN_CODE:DWORD
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE,DS:SYSCODE

        ALIGN   4
        PUBLIC  SYS_BPT_LIST
SYS_BPT_LIST    DD      0
        ENDS
        END
