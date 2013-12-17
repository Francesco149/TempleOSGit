#help_index "Doc"
#help_file "::/Doc/Doc"

//See [C:/Doc/Doc.TXT.Z,1]
extern I64 EdLeftClickLink(CDoc *doc,CDocEntry *doc_e);
extern I64 TermRightClickLink(CDoc *doc,CDocEntry *doc_e);
extern U0 DocPutSPartial(CDoc *doc,U8 *st);
extern U0 DocFormBackward(CDoc *doc,Bool giveup=FALSE);
extern U0 EdUndoFilter(CDoc *doc);

#define DHT_DOC_CMD	1
#define DHT_DOC_FLAG	2
#define DHT_COLOR	4

CHashTable *doc_hash,*doc_highlight_hash;
I64 doc_dft_flags1[DOCT_NUM_TYPES];
I32 doc_dft_type_flags[DOCT_NUM_TYPES];

#include "DocNew"
#include "DocPlain"

U0 DocInit()
{ //[C:/Kernel/Adam1a.HPP.Z,947] CMD Defines
  CLex *lx;
  I64 i;
  CHashGeneric *temph;

  DefineLstLoad("ST_DOC_CMDS",
	"TX\0CR\0SR\0CU\0TB\0CL\0PB\0DA\0PL\0LM\0RM\0HD\0FO\0ID\0"
	"FG\0BG\0FD\0BD\0PT\0WW\0UL\0IV\0BK\0SX\0SY\0CM\0AN\0LK\0BT\0"
	"CB\0MA\0MU\0HX\0TR\0SP\0IB\0IT\0IS\0BP\0SO\0HL\0LP\0CC\0HC\0ER\0");
  DefineLstLoad("ST_DOC_FLAGS",
	"A\0L\0"
	"LC\0LE\0LM\0LA\0RC\0RE\0RM\0RA\0"
	"TC\0BI\0X\0Q\0LX\0CX\0RX\0TY\0CY\0BY\0SX\0SY\0SCX\0SCY\0"
	"HL\0LP\0CC\0WW\0BK\0IV\0SEL\0UL\0"
	"B\0S\0BD\0C\0CA\0RD\0P\0T\0Z\0H\0TR\0SK\0PU\0PRY\0MRX\0"
	"FST\0SIF\0WD\0HT\0HS\0FS\0NC\0DD\0UD\0U8\0U16\0U32\0U64\0"
	"D\0WR\0HTML\0BP\0"
	);
  DefineLstLoad("ST_LINK_TYPES","FI\0FA\0FF\0FL\0MN\0PI\0PF\0PL\0BF\0DF\0HI\0AD\0");

  doc_hash=HashTableNew(512);
  HashDefineLstAdd("ST_DOC_CMDS",DHT_DOC_CMD,doc_hash);
  HashDefineLstAdd("ST_DOC_FLAGS",DHT_DOC_FLAG,doc_hash);

  doc_highlight_hash=MAllocIdentical(asm_hash);
  doc_highlight_hash->next=NULL;
 
  MemSet(doc_dft_flags1,	0,sizeof(doc_dft_flags1));
  MemSet(doc_dft_type_flags,	0,sizeof(doc_dft_type_flags));

  lx=LexNew("DA+P+T+A+U64;CM+LE+RE;LK+L+A+UL;AN+A;"
	"MA+X+LM+UL;MU+X+LE+UL;BT+X+LE+B;"
	"CB+CA+P+U8;TR+C+CA+TR+UL;HX+P+Z;SO+A;"
	"SP+BI;IB+BI;IT+BI;IS+BI;",LF_DONT_FREE_BUF);
  lx->htc.hash_table_lst=NULL;
  Lex(lx);
  while (lx->token==TK_IDENT) {
    if (temph=HashFind(lx->ident,doc_hash,DHT_DOC_CMD)) {
      i=temph->user_data0;
      Lex(lx);
      PrsDocFlags(lx,&doc_dft_flags1[i],&doc_dft_type_flags[i]);
    }
    if (lx->token==';')
      Lex(lx);
  }
  LexDel(lx);

  for (i=0;i<DOCT_NUM_TYPES;i++)
    doc_dft_flags1[i]&=~DOCG_BL_IV_UL;
}
DocInit;

#help_index "Doc;International"
#help_file "::/Doc/International"

public U0 InternationalDocInit()
{//For international, call after loading new ST_COLORS
  HashDefineLstAdd("ST_COLORS",DHT_COLOR,doc_hash);
}
InternationalDocInit;

#help_index "Doc"
//--TEMPLEOS LINK TYPES--
//  "filename"
//  "FI:filename"
//  "FA:filename,anchor"
//  "FF:filename,pattern"
//  "FF:filename,pattern:occurnum"
//  "FL:filename,linenum"
#define LK_FILE 	0
#define LK_FILE_ANCHOR	1
#define LK_FILE_FIND	2
#define LK_FILE_LINE	3
//  "MN:SymName"
#define LK_MAN_PAGE	4
//  "PI:filename"
//  "PF:filename,pattern"
//  "PF:filename,pattern:occurnum"
//  "PL:filename,linenum"
#define LK_PLAIN	5
#define LK_PLAIN_FIND	6
#define LK_PLAIN_LINE	7
//  "BF:book,pattern"
#define LK_BIBLE_FIND	8
//  "DF:word"
//  "DF:word,defnum"
#define LK_DEF		9
//  "HI:index"
#define LK_HELP_INDEX	10
//  "AD:address_number"
#define LK_ADDR		11
#define LK_PLACE_ANCHOR	12

#include "DocHighlight"
#include "DocRecalc2a"
#include "DocRecalc2b"
#include "DocFile"
#include "DocOpt"
#include "DocPopUp"
#include "DocCtrlL"
#include "DocMacro"
#include "DocFind"
#include "DocEd2a"
#include "DocEd2b"
#include "DocCodeTools"
#include "DocBible"
#include "DocTree"
#include "DocMain"
#help_index ""

/*
TempleOS Doc's can have "cursor movement" cmds which can move the cursor up 
the screen and layer on existing text.  It can also have call-back funs which 
supply live, changing text.  For these reasons, you can't assume you know 
where the visible portion of the document is and must process an extensive 
portion of the document each time it is placed on the screen, becoming CPU 
intensive on big documents.  The doc->recalc_start member attempts to limit 
what is recalculated.  This is tricky, howeve, and optimizations create minor 
glitches.  Similarly, an optimization to only refresh the entire document 1
in 
10 times it is drawn to the screen creates minor glitches.  Since the Doc 
handles the cmd line, it must also be fast when adding text output from 
programs.  doc->recalc_start comes into play here.

*/