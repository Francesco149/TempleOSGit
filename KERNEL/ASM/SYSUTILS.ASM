	ALIGN	4,0x90
DEBUG_SPIN_WAIT::
	PUSH	ECX
	MOV	ECX,0x8FFFFFFF
@@1:	LOOP	@@1
	POP	ECX
	RET
////**************************PROCEDURE*************************
// IN:	 ESI
	ALIGN	4,0x90
DELETE_LINKED_LIST::
	PUSH	EDI
	JMP	@@2
@@1:	MOV	EDI,DWORD [ESI]
	CALL	FREE
	MOV	ESI,EDI
@@2:	OR	ESI,ESI
	JNZ	@@1
	POP	EDI
	RET
////**************************PROCEDURE*************************
// IN:	 ESI=NODE TO INSERT
//	 EDI=PREDECESSOR
	ALIGN	4,0x90
INSQUE::
	PUSH	EBX
	MOV	EBX,[EDI]
	MOV	[EDI],ESI
	MOV	[ESI],EBX
	MOV	4[ESI],EDI
	MOV	4[EBX],ESI
	POP	EBX
	RET
////**************************PROCEDURE*************************
// IN:	 ESI=NODE TO INSERT
//	 EDI=SUCCESSOR
	ALIGN	4,0x90
INSQUE_REV::
	PUSH	EBX
	MOV	EBX,4[EDI]
	MOV	[EBX],ESI
	MOV	[ESI],EDI
	MOV	4[ESI],EBX
	MOV	4[EDI],ESI
	POP	EBX
	RET
////**************************PROCEDURE*************************
// IN:	 ESI=NODE TO REMOVE
	ALIGN	4,0x90
REMQUE::
	PUSH	EBX
	PUSH	EDI
	MOV	EBX,[ESI]
	MOV	EDI,4[ESI]
	MOV	[EDI],EBX
	MOV	4[EBX],EDI
	POP	EDI
	POP	EBX
	RET
////**************************PROCEDURE*************************
// IN:	 EAX=BINARY
//	 ES:[EDI]=@FIELD
//	 ECX=LENGTH (IF THE HEX_FIELD ENTRY IS USED)
//
// OUT:  EAX PRESERVED
//	 ECX=0
//	 [EDI] POINTS PAST DESTINATION

	ALIGN	4,0x90
BINARY_TO_HEX::
	MOV	ECX,8
BINARY_TO_FIXED_HEX:
	PUSHFD				//-****
	CLI				//because of STD
	PUSH	EAX			//-****
	PUSH	EDX			//-****
	ADD	EDI,ECX
	PUSH	EDI			//-**** EDI WE WANT TO RETURN
	DEC	EDI
	MOV	EDX,EAX
	STD

@@1:	MOV	AL,DL
	AND	AL,0xF
	ADD	AL,0x90
	DAA
	ADC	AL,0x40
	DAA
	STOSB
	SHR	EDX,4
	LOOP	@@1

	POP	EDI			//+****
	POP	EDX			//+****
	POP	EAX			//+****
	POPFD				//+****
	RET
////**************************PROCEDURE*************************
//IN:	 [ESI] = @FIELD
//	 ECX = LENGTH
//OUT:	 EAX = VALUE
//	 ECX = 0
	ALIGN	4,0x90
HEX_TO_BINARY::
	PUSH	EDX
	XOR	EDX,EDX
	OR	ECX,ECX
	JZ	@@9
@@1:	SHL	EDX,4
	LODSB
	SUB	AL,0x30
	CMP	AL,9
	JA	@@2
	SUB	AL,7
@@2:	MOVZX	EAX,AL
	ADD	EDX,EAX
	LOOP	@@1

@@9:	MOV	EAX,EDX
	POP	EDX
	RET
////************************************************************
	STRUC 0
TR_NEXT:	 DD	 ?;
TR_ESP: 	 DD	 ?;
TR_ROUTINE:	 DD	 ?;
TR_STRUCT_SIZE:  DD	 ?;
	ENDS
////**************************PROCEDURE*************************
	ALIGN	4,0x90
REGISTER_TRAP::
//IN:	 EBX=ADDRESS OF TRAP
//	 ECX=STACK
	PUSH	ESI
	PUSH	EAX
	MOV	EAX,TR_STRUCT_SIZE
	CALL	MALLOC
	MOV	EAX,DWORD FS:[TSS_TRAP_LIST]
	MOV	DWORD TR_NEXT[ESI],EAX
	MOV	DWORD FS:[TSS_TRAP_LIST],ESI
	MOV	DWORD TR_ESP[ESI],ECX
	MOV	DWORD TR_ROUTINE[ESI],EBX
	POP	EAX
	POP	ESI
	RET
////**************************PROCEDURE*************************
	ALIGN	4,0x90
END_TRAP::
	PUSH	EAX
	PUSH	ESI
	MOV	ESI,DWORD FS:[TSS_TRAP_LIST]
	MOV	EAX,DWORD TR_NEXT[ESI]
	MOV	DWORD FS:[TSS_TRAP_LIST],EAX
	CALL	FREE
	POP	ESI
	POP	EAX
	RET
////**************************PROCEDURE*************************
//OUT:	 NUKES EBX
	ALIGN	4,0x90
TRAP::
	MOV	EBX,DWORD FS:[TSS_TRAP_LIST]
	MOV	ESP,DWORD TR_ESP[EBX]
	CALL	END_TRAP
	JMP	DWORD TR_ROUTINE[EBX]
////**************************PROCEDURE*************************
SEED_LO:	DW	0;
SEED_HI:	DW	0;
	ALIGN	4,0x90
CMD_SRAND::
	PUSH	EBP
	MOV	EBP,ESP
	MOV	AX,WORD SF_PARAM1[EBP]
	MOV	WORD [SEED_LO],AX
	XOR	AX,AX
	MOV	WORD [SEED_HI],AX
	POP	EBP
	RET
////**************************PROCEDURE*************************
	ALIGN	4,0x90
CMD_RAND::
	PUSH	ESI
	PUSH	ECX
	MOVZX	ECX,WORD [SEED_HI]
	MOVZX	EBX,WORD [SEED_LO]
	MOV	EDX,0x15A
	MOV	EAX,0x4E35
	XCHG	SI,AX
	XCHG	DX,AX
	TEST	AX,AX
	JE	@@1
	MUL	BX
@@1:	JCXZ	@@2
	XCHG	CX,AX
	MUL	SI
	ADD	AX,CX
@@2:	XCHG	SI,AX
	MUL	BX
	ADD	DX,SI
	ADD	AX,1
	ADC	DX,0
	MOV	WORD [SEED_HI],DX
	MOV	WORD [SEED_LO],AX
	MOV	AX,DX
	AND	EAX,0x7FFF
	POP	ECX
	POP	ESI
	RET
	END
