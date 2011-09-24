asm {
#include "/LT/OSMain/BootDefs.HPZ"

#define MODULE_SIZE		CD_BOOT_LOADER_SIZE
#define BOOT_HIGH_LOC	((BOOT_RAM_LIMIT-(BOOT_STK_SIZE+MODULE_SIZE))>>4)


USE16
BCD_START::
//DL is supposed to have the BIOS drive number
	CLD
	MOV	AX,BOOT_HIGH_LOC
	MOV	ES,AX

	CLI
	MOV	SS,AX
	MOV	SP,BOOT_STK_SIZE+MODULE_SIZE
	STI

	CALL	GET_IP
GET_IP:
	POP	BX
	SUB	BX,GET_IP
	SHR	BX,4
//Copy this bootloader code to just under 0x80000
	MOV	AX,CS
	ADD	AX,BX
	MOV	DS,AX
	MOV	CX,MODULE_SIZE
	XOR	SI,SI
	XOR	DI,DI
	REP_MOVSB

	MOV	AX,BOOT_HIGH_LOC
	MOV	DS,AX

//My assembler doesn't support 16-bit very well.
	DU8	0xEA;	//JMP BOOT_HIGH_LOC:BCD_MAIN
	DU16	BCD_MAIN,BOOT_HIGH_LOC;

BCD_BIOS_DRV_NUM:	DU8	0;
BCD_PAGE:		DU8	0;

BCD_DAP:		DU8	16,0,1,0; //One blk at a time
BCD_DAP_BUF:		DU16	0,0;
BCD_DAP_BLK:		DU64	0;
	
BCD_LOSETHOS_MSG:
	DU8	"\r\n\n\n\n\n"
"                   *  ********************************  \n\r"
"                   *                 *                 *\n\r"
"                   *   **   *** **** * *  *  **   ***  *\n\r"
"                   *  *  * *    *    * *  * *  * *     *\n\r"
"                   *  *  * *    *    * *  * *  * *     *\n\r"
"                   *  *  *  **  ***  * **** *  *  **   *\n\r"
"                   *  *  *    * *    * *  * *  *    *  *\n\r"
"                   *  *  *    * *    * *  * *  *    *   \n\r"
"                   *   **  ***  **** * *  *  **  ***   *\n\r"
"                   *                                    \n\r"
"                   ***********************************  \n\r"
"\n\n\n\n\nLoading LoseThos",0;

BCD_NOT64_MSG:
	DU8	"LoseThos requires a 64-bit capable processor.\n\r",0;


//These get patched by [C:/LT/Opt/Boot/InsBoot.APZ,35] CDBootFileCreate
BCD_BLK_LO::		DU16	0;
BCD_BLK_HI::		DU16	0;
BCD_BLK_CNT::		DU16	0;

BCD_PROGRESS_STEP::	DU32	0;
BCD_PROGRESS_VAL::	DU32	0;

BCD_PUTCHAR::
	MOV	AH,0xE
	MOV	BL,7 //Might be foreground color on some BIOS's
	MOV	BH,U8 [BCD_PAGE]
	INT	0x10
BCD_RET::
	RET
BCD_PUTS::
@@1:	LODSB
	OR	AL,AL
	JZ	BCD_RET
	CALL	BCD_PUTCHAR
	JMP	@@1

BCD_MAIN::
	MOV	U8 [BCD_BIOS_DRV_NUM],DL //Passed in by BIOS

	MOV	AH,0xF
	INT	0x10
	MOV	U8 [BCD_PAGE],BH //video page

	MOV	EAX,0x80000001
	CPUID
	BT	EDX,29
	JC	@@10
	MOV	SI,BCD_NOT64_MSG
	CALL	BCD_PUTS
@@05:	JMP	@@05

@@10:	MOV	SI,BCD_LOSETHOS_MSG
	CALL	BCD_PUTS

	MOV	AX,BOOT_RAM_BASE/16
	MOV	ES,AX
	XOR	ECX,ECX
	MOV	CX,U16 [BCD_BLK_CNT]

	MOV	EAX,(80-7-9)*65536	//80 columns
	XOR	EDX,EDX
	DIV	ECX
	MOV	U32 [BCD_PROGRESS_STEP],EAX
	MOV	U32 [BCD_PROGRESS_VAL],0

	MOV	AX,U16 [BCD_BLK_LO]
	MOV	DX,U16 [BCD_BLK_HI]

@@15:	PUSH	CX	//Blk cnt

//READ BLK
	PUSH	AX	//Blk lo
	PUSH	DX	//Blk hi
	PUSH	ES	//Buf seg
	MOV	U16 [BCD_DAP_BLK],AX
	MOV	U16 [BCD_DAP_BLK+2],DX
	MOV	AX,ES
	MOV	U16 [BCD_DAP_BUF+2],AX //ES:0000
	MOV	SI,BCD_DAP //DS:SI=DAP
	MOV	AH,0x42
	MOV	DL,U8 [BCD_BIOS_DRV_NUM]
	INT	0x13

	POP	AX	//ES
	ADD	AX,CD_BLK_SIZE/16
	MOV	ES,AX
	POP	DX
	POP	AX
	INC	AX
	JNZ	@@20
	INC	DX

@@20:	PUSH	AX
	MOV	BX,U16 [BCD_PROGRESS_VAL+2]
	MOV	EAX,U32 [BCD_PROGRESS_STEP]
	ADD	U32 [BCD_PROGRESS_VAL],EAX
	CMP	U16 [BCD_PROGRESS_VAL+2],BX
	JE	@@25
	MOV	AL,'.'
	CALL	BCD_PUTCHAR
@@25:	POP	AX

	POP	CX
	LOOP	@@15

//See [C:/LT/OSMain/BIOS32.CPZ,292] BootCdProbe().
	MOV	EBX,U32 [BCD_BLK_LO]
	SHL	EBX,16
	MOV	BX,BOOT_CODE_CDROM  //See sys_boot_code
`Grep("sys_boot_code","/LT/"TEXT_FILE_MASK);View;`


//My assembler doesn't support 16-bit very well.
	DU8	0xEA;	//JMP BOOT_RAM_BASE:0000
	DU16	0,BOOT_RAM_BASE/16;
//Continues here [::/LT/OSMain/OSStartUp.CPZ,1] ::/LT/OSMain/OSStartUp.CPZ
BCD_END::
#assert $<MODULE_SIZE
	END
}