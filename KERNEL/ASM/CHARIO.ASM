	ALIGN	4,0x90
GET_CHAR::
	CALL	GET_KEY
	INT	I_PUT_CHAR
	RET
////**************************PROCEDURE*************************
// IN:	 ES:[EDI]=ADDRESS OF BUFFER
//	 ECX	 =SIZE OF BUFFER
// OUT:  ES:[EDI]=BYTE AFTER LAST BYTE ENTERED
//	 ECX	 =NUM CHARS READ
	ALIGN	4,0x90
GET_STRING::
	PUSH	EAX
	PUSH	EBX
	XOR	EBX,EBX
	DEC	ECX
@@1:	OR	ECX,ECX 	//ROOM LEFT?
	JE	@@9
	INT	I_GET_CHAR
	CMP	AL,13		//RETURN?
	JE	@@9
	CMP	AL,8		//BACK SPACE?
	JE	@@2
	STOSB
	INC	EBX
	DEC	ECX
	JMP	@@1

@@2:	OR	EBX,EBX
	JZ	@@1
	INC	ECX
	DEC	EDI
	DEC	EBX
	JMP	@@1

@@9:	MOV	AL,0
	STOSB
	MOV	ECX,EBX
	POP	EBX
	POP	EAX
	RET
////**************************PROCEDURE*************************
	ALIGN	4,0x90
PUT_CHAR::
	JMP	DWORD CRT_OUT
////**************************PROCEDURE*************************
	ALIGN	4,0x90
PUT_STRING::
// IN:	 ES:[ESI]=STRING
//	 ECX=COUNT
	PUSH	ECX
	MOV	ECX,0xFFFFFFFF
	CALL	FIXED_PUT_STRING
	POP	ECX
	RET
FIXED_PUT_STRING::
	OR	ESI,ESI
	JZ	@@200
	PUSH	ESI
	PUSH	ECX
	XOR	EAX,EAX
	OR	ECX,ECX
	JZ	@@100
@@1:	LODSB
	OR	AL,AL
	JZ	@@100
	INT	I_PUT_CHAR
	LOOP	@@1

@@100:	POP	ECX
	POP	ESI
@@200:	RET
////**************************PROCEDURE*************************
// IN:	 EAX=NUMBER TO PRINT
	ALIGN	4,0x90
PUT_DEC::
	PUSH	ECX
	MOV	ECX,8
	CALL	FIXED_PUT_HEX
	POP	ECX
	RET
PUT_HEX::
	PUSH	ECX
	MOV	ECX,8
	CALL	FIXED_PUT_HEX
	POP	ECX
	RET
PUT_HEX_WORD::
	PUSH	ECX
	MOV	ECX,4
	CALL	FIXED_PUT_HEX
	POP	ECX
	RET
PUT_HEX_BYTE::
	PUSH	ECX
	MOV	ECX,2
	CALL	FIXED_PUT_HEX
	POP	ECX
	RET
FIXED_PUT_HEX::
	PUSH	EAX			//-****
	PUSH	EBX			//-****
	PUSH	EDI			//-****
	PUSH	EBP			//-****
	SUB	ESP,8			//-****

	MOV	EDI,ESP

	PUSH	ECX			//-****
	CALL	BINARY_TO_FIXED_HEX
	POP	ECX			//+****

	MOV	EBP,ESP

@@1:	MOV	AL,BYTE [EBP]
	INC	EBP
	INT	I_PUT_CHAR
	LOOP	@@1

	ADD	ESP,8			//+****
	POP	EBP			//+****
	POP	EDI			//+****
	POP	EBX			//+****
	POP	EAX			//+****
	RET
	END
