        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSCODE SEGMENT DWORD USE32 PUBLIC
        ASSUME  CS:SYSCODE, DS:SYSCODE

        PUBLIC  SYS_CS_SIZE
        ALIGN   4
SYS_CS_SIZE:
        ENDS
        END
