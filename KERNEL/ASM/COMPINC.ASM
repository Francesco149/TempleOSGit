#define CF_INTERNAL 0

	STRUC 0
C_HASH_NEXT:	 DD	?;
C_HASH_STRING:	 DD	?;
C_HASH_TYPE:	 DD	?;
C_MEM_COUNT:	 DW	?;
C_PARAM_COUNT:	 DW	?;	//FOR FUNCTIONS
C_SIZE: 	 DD	?;	//Must be in same position as HTE_USER1
C_DEREFERENCE:	 DW	?;     //Must be in same position as HTE_USER2
C_ASSIGN:	 DW	?;	  //Must be in same position as HTE_USER2
C_MEM_LIST:	 DD	?;	 //MEMBER LIST
C_MEM_LIST_LAST: DD	?;
C_STATEMENTS:	 DD	?;	 //For functions
C_RETURN_CLASS:  DD	?;	  //For functions
C_PTR_COUNT:	 DB	?;
C_FLAGS:	 DB	?,?,?;
C_STRUCT_SIZE:
	ENDS

	STRUC	0
IT_CHAR:	 DB	 ?;
IT_BYTE:	 DB	 ?;
IT_SHORT:	 DB	 ?;
IT_WORD:	 DB	 ?;
IT_INT: 	 DB	 ?;
IT_DWORD:	 DB	 ?;
IT_DOUBLE:	 DB	 ?;
IT_UDOUBLE:	 DB	 ?; //ficticious place holder
IT_STRUCT_SIZE:
	ENDS

#define MLF_DEFAULT_AVAILABLE	    1
#define MLF_FUNCTION		    2
#define MLF_DOT_DOT_DOT 	    4

	STRUC 0
ML_NEXT:	 DD	 ?;
ML_STRING:	 DD	 ?;
ML_CLASS:	 DD	 ?;
ML_COUNT:	 DD	 ?;	 //FOR ARRAYS
ML_OFFSET:	 DD	 ?;
ML_PARAM_COUNT:  DW	 ?;	 //FOR FUNCTIONS
ML_FLAGS:	 DW	 ?;
ML_DEFAULT:	 DD	 ?;
ML_FORMAT_STR:	 DD	 ?;
ML_FORMAT_DATA:  DD	 ?;
ML_OUTPUT_FUN:	 DD	 ?;
ML_INPUT_FUN:	 DD	 ?;
ML_STRUCT_SIZE:
	ENDS


#define GVAF_FUNCTION	    1

	STRUC 0
GVA_HASH_NEXT:	  DD	  ?;
GVA_HASH_STRING:  DD	  ?;
GVA_HASH_TYPE:	  DD	  ?;
GVA_CLASS:	  DD	  ?;
GVA_COUNT:	  DD	  ?;	   //FOR ARRAYS
GVA_FLAGS:	  DW	  ?;
GVA_PARAM_COUNT:  DW	  ?;
GVA_STRUCT_SIZE:
#define GVA_DATA  GVA_STRUCT_SIZE
		ENDS


	STRUC 0   //ENUMERATION
KW_INCLUDE:	 DB	 ?;
KW_DEFINE:	 DB	 ?;
KW_CLASS:	 DB	 ?;
KW_FUN: 	 DB	 ?;
KW_IF:		 DB	 ?;
KW_ELSE:	 DB	 ?;
KW_FOR: 	 DB	 ?;
KW_EXIT:	 DB	 ?;
KW_WHILE:	 DB	 ?;
KW_SYSFUN:	 DB	 ?;
KW_RETURN:	 DB	 ?;
KW_SIZEOF:	 DB	 ?;
KW_INTERNAL:	 DB	 ?;
KW_TYPEOF:	 DB	 ?;
KW_FORMAT_STR:	 DB	 ?;
KW_FORMAT_DATA:  DB	 ?;
KW_OUTPUT_FUN:	 DB	 ?;
KW_INPUT_FUN:	 DB	 ?;
	ENDS

	STRUC 0
IC_CODE:	 DD	 ?;
IC_DATA:	 DD	 ?;
IC_CLASS:	 DD	 ?;
IC_STRUCT_SIZE:
	ENDS


	STRUC 0
CB_NEXT:	 DD	 ?;
CB_DATA:	 DB	 IC_STRUCT_SIZE*63 DUP(?);
CB_STRUCT_SIZE:
	ENDS

	STRUC 0
LB_NEXT:	 DD	 ?;
LB_STRING:	 DD	 ?;
LB_ADDRESS:	 DD	 ?;
LB_STRUCT_SIZE:
	ENDS

//Adding new expressions codes:
// 1)Create EC_ define here
// 2)Add code to ECT_,UECT_,FCT_,OCT_ tables
// 3)Add to CODE_TEXT_TABLE,FIX_UP_8,FIX_UP_32,FIX_UP_32_JMP
// 4)BINARY_OPS,UNARY_OPS,DEREF_TABLE and pre/post/inc/dec



	STRUC 0   //NOTE these are hard-coded in tables
EC_NULL:		 DB ?; //     0
EC_XOR: 		 DB ?; //     1
EC_OR_OR:		 DB ?; //     2
EC_AND_AND:		 DB ?; //     3
EC_LESS_THAN:		 DB ?; //     4
EC_GREATER_THAN:	 DB ?; //     5
EC_EQUAL_EQUAL: 	 DB ?; //     6
EC_NOT_EQUAL_TO:	 DB ?; //     7

EC_GREATER_OR_EQUAL:	 DB ?; //     8
EC_LESS_OR_EQUAL:	 DB ?; //     9
EC_ADDITION:		 DB ?; //     0Ah
EC_SUBTRACTION: 	 DB ?; //     0Bh
EC_MULTIPLICATION:	 DB ?; //     0Ch
EC_DIVISION:		 DB ?; //     0Dh
EC_REMAINDER:		 DB ?; //     0Eh
EC_UNARY_MINUS: 	 DB ?; //     0Fh

EC_NOT: 		 DB ?; //     010h
EC_INT32:		 DB ?; //     011h
EC_DOUBLE:		 DB ?; //     012h
EC_LOCAL_FRAME_PTR:	 DB ?; //     013h
EC_DEREFERENCE: 	 DB ?; //     014h
EC_CALL:		 DB ?; //     015h
EC_RET: 		 DB ?; //     016h
EC_INC: 		 DB ?; //     017h

EC_DEC: 		 DB ?; //     018h
EC_INT8:		 DB ?; //     019h //unused
EC_ADD32:		 DB ?; //     01Ah
EC_SUB32:		 DB ?; //     01Bh
EC_ADD8:		 DB ?; //     01Ch
EC_SUB8:		 DB ?; //     01Dh
EC_MUL32:		 DB ?; //     01Eh
EC_DIV32:		 DB ?; //     01Fh

EC_MUL8:		 DB ?; //     020h
EC_DIV8:		 DB ?; //     021h
EC_ASSIGN32:		 DB ?; //     022h
EC_ASSIGN16:		 DB ?; //     023h
EC_ASSIGN8:		 DB ?; //     024h
EC_ADD_ESP:		 DB ?; //     025h
EC_NOP: 		 DB ?; //     026h
EC_OR:			 DB ?; //     027h

EC_AND: 		 DB ?; //     028h
EC_SHL: 		 DB ?; //     029h
EC_SHR: 		 DB ?; //     02Ah
EC_COM: 		 DB ?; //     02Bh
EC_PRINTF:		 DB ?; //     02Ch
EC_LABEL:		 DB ?; //     02Dh
EC_SKIPZ:		 DB ?; //     02Eh
EC_JMP: 		 DB ?; //     02Fh

EC_ZBD: 		 DB ?; //     030h
EC_SBD: 		 DB ?; //     031h
EC_ZWD: 		 DB ?; //     032h
EC_SWD: 		 DB ?; //     033h
EC_DWORD_PP:		 DB ?; //     034h
EC_WORD_PP:		 DB ?; //     035h
EC_BYTE_PP:		 DB ?; //     036h
EC_short_PP:		 DB ?; //     037h

EC_char_PP:		 DB ?; //     038h
EC_DWORD_MM:		 DB ?; //     039h
EC_WORD_MM:		 DB ?; //     03Ah
EC_BYTE_MM:		 DB ?; //     03Bh
EC_short_MM:		 DB ?; //     03Ch
EC_char_MM:		 DB ?; //     03Dh
EC_ADDRESS:		 DB ?; //     03Eh
EC_FS:			 DB ?; //     03Fh

EC_CLI: 		 DB ?; //     040h
EC_STI: 		 DB ?; //     041h
EC_GET_FLAGS:		 DB ?; //     042h
EC_SET_FLAGS:		 DB ?; //     043h
EC_BT:			 DB ?; //     044h
EC_BTS: 		 DB ?; //     045h
EC_BTR: 		 DB ?; //     046h
EC_INSQUE:		 DB ?; //     047h

EC_INSQUE_REV:		 DB ?; //     048h
EC_REMQUE:		 DB ?; //     049h
EC_INP: 		 DB ?; //     04Ah
EC_OUTP:		 DB ?; //     04Bh
EC_REP_INSB:		 DB ?; //     04Ch
EC_REP_INSW:		 DB ?; //     04Dh
EC_REP_OUTSB:		 DB ?; //     04Eh
EC_REP_OUTSW:		 DB ?; //     04Fh

EC_MEMCPY:		 DB ?; //     050h
EC_MEMSET:		 DB ?; //     051h
EC_INPW:		 DB ?; //     052h
EC_OUTPW:		 DB ?; //     053h
EC_PP_DWORD:		 DB ?; //     054h
EC_PP_WORD:		 DB ?; //     055h
EC_PP_BYTE:		 DB ?; //     056h
EC_PP_short:		 DB ?; //     057h

EC_PP_char:		 DB ?; //     058h
EC_MM_DWORD:		 DB ?; //     059h
EC_MM_WORD:		 DB ?; //     05Ah
EC_MM_BYTE:		 DB ?; //     05Bh
EC_MM_short:		 DB ?; //     05Ch
EC_MM_char:		 DB ?; //     05Dh
EC_INPD:		 DB ?; //     05Eh
EC_OUTPD:		 DB ?; //     05Fh

EC_ENTER:		 DB ?; //     060h
EC_LEAVE:		 DB ?; //     061h
EC_TYPE:		 DB ?; //     062h
EC_DS:			 DB ?; //     063h
EC_ADD_ESP8:		 DB ?; //     064h
EC_DWORD_DISP32:	 DB ?; //     065h
EC_WORD_DISP32: 	 DB ?; //     066h
EC_BYTE_DISP32: 	 DB ?; //     067h

EC_DWORD_DISP8: 	 DB ?; //     068h
EC_WORD_DISP8:		 DB ?; //     069h
EC_BYTE_DISP8:		 DB ?; //     06Ah
EC_CALL_INDIRECT:	 DB ?; //     06Bh
	ENDS
	END
