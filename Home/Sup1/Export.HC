//This is NOT Terry's work. I wrote this program to export historical
//versions of TempleOS to git.
//The goal is to obtain something easily browsable in a normal text editor
//and easy to diff in git while not loosing too much information. Docs are
//converted to plaintext but retain links and a few decorations, binary data
//is dumped to a DAT file for docs (embedded as hex strings in code in
//old versions).
//As an added bonus, I try to match each iteration's coding style.
//This code is public domain.

Option(OPTf_WARN_PAREN,ON);

//Just in case the filesystem gets corrupted while I develop it
U0 BackupExport()
{
  Copy("D:/Export.HC","C:/Export.HC");
  Copy("D:/Export.HC","D:/ExportB.HC");
  Copy("D:/Export.HC","C:/ExportB.HC");
}

//Appears to hang on this version...
//BackupExport

#define EXPORT_W 78

U0 ExportBinary(U8 *src_file,CDocBin *b)
{
  I64 len;
  U8 *buf,*buf1;

  len=StrLen(src_file);
  buf1=MAlloc(len+1);
  StrCpy(buf1,src_file);
  buf=buf1+len-1;
  while (buf>buf1 && *buf!='.')
    buf--;
  *buf=0;
  buf=MStrPrint("%s%d.DAT",buf1,b->num);
  Free(buf1);
  "%s\n",buf;
  FileWrite(buf,b->data,b->size);
  Free(buf);
}

U8 *ResolveLink(U8 *link_st)
{ //Adapted from [C:/Adam/DolDoc/DocEd.HC.Z,189] Ed. Resolves stuff like MN:Func to the file
  U8 *res,*filename=NULL,*pattern=NULL;
  I64 num;
  if (EdLinkCvt(link_st,&filename,&pattern,&num,0)==-1)
    res=StrNew(link_st);
  else if (num!=0)
    res=MStrPrint("%s,%d",filename,num);
  else
    res=MStrPrint("%s,%s",filename,pattern);
  Free(filename);
  Free(pattern);
  return res;
}

U8 *IndentString(I64 n)
{
  U8 *s=MAlloc(n+1);
  MemSet(s,' ',n);
  s[n]=0;
  return s;
}

U8 *StrAdd(U8 *base,U8 *addition) {
  I64 base_len=StrLen(base);
  I64 addition_len=StrLen(addition);
  I64 required=base_len+addition_len+1;
  I64 base_size=MSize(base);
  U8 *res=base;
  if (base_size<=required) {
    while (base_size<=required)
      base_size*=2;
    res=MAlloc(base_size);
    MemCpy(res,base,base_len);
    Free(base);
  }
  MemCpy(res+base_len,addition,addition_len+1);
  return res;
}

U8 *Wrap(U8 *base,U8 *s,I64 *line_len,I64 indent=0)
{ //Performs word wrapping on base, starting from s, line_len is state.
  //Also adds indent spaces to every soft and hard newline.
  //Returns new value for base.
  //This is supposed to also take tabs into account but fuck that shit
  //NOTE: at this point I could use [C:/Adam/Win.HC.Z,357] WinHorz and append to a CDoc
  I64 len;
  U8 *p,*buf;
  U8 *end;
  U8 *lbstring;
  I64 lbstring_len;

  lbstring=MAlloc(indent+2);
  lbstring[0]='\n';
  MemSet(lbstring+1,' ',indent);
  lbstring[indent+1]=0;
  lbstring_len=indent+1;

  s-=*line_len;
  end=s+EXPORT_W;
  while (StrLen(s)>EXPORT_W) {
    for (p=s;*p && p<end;p++) {
      if (*p=='\n') {
        buf=StrNew(p+1);
        if(p>s && *(p-1)=='\r')
          p--;
        *p=0;
        len=p-base;
        base=StrAdd(base,lbstring);
        base=StrAdd(base,buf);
        Free(buf);
        p=base+len+lbstring_len-1;
        s=p+1;
        *line_len=0;
        end=s+EXPORT_W;
      }
    }
    if (StrLen(s)<=EXPORT_W)
      break;
    for (p=s+EXPORT_W-1;p>s;p--) {
      if (*p==' ')
        break;
    }
    if (*p!=' ') {
//Truncate words longer than a line
      while (StrLen(s)>EXPORT_W) {
        buf=StrNew(s+EXPORT_W);
        s[EXPORT_W]=0;
        len=s-base;
        base=StrAdd(base,lbstring);
        base=StrAdd(base,buf);
        Free(buf);
        s=base+len+EXPORT_W+lbstring_len;
        *line_len=0;
      }
      break;
    }
    buf=StrNew(p+1);
    *p=0;
    len=p-base;
    base=StrAdd(base,lbstring);
    base=StrAdd(base,buf);
    Free(buf);
    *line_len=0;
    s=base+len+lbstring_len;
  }
  *line_len=StrLen(s);
  Free(lbstring);
  return base;
}

U8 *StrToASCII(U8 *s)
{ //Convert non-standard ASCII
  U8 *ex,*buf,*buf1;
  I64 len;
  for (buf=s;*buf;buf++) {
    switch (*buf) {
      case CH_SHIFT_SPACE: ex=" "; break; //Special space, not conv. to tabs
      case CH_CURSOR: ex=""; break; //Cursor position
      case ' ': ex=" "; break;
      case 'pi': ex="pi"; break;
      case 'inf': ex="inf"; break;
      case 'theta': ex="theta"; break;
      case 'phi': ex="phi"; break;
      case 'omega': ex="omega"; break;
      case '-': ex="-"; break;
      case 'u':
      case 'u':
      case 'u':
      case 'u':
      case 'u':
      case 'u': ex="u"; break;
      case 'alpha': ex="alpha"; break;
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+': ex="+"; break;
      case '-': ex="-"; break;
      case '|':
      case '|': ex="|"; break;
      case '_': ex="_"; break;
      case 'a':
      case 'a':
      case 'a':
      case 'a': ex="a"; break;
      case 'c':
      case 'c':
      case 'c': ex="c"; break;
      case 'e':
      case 'e':
      case 'e':
      case 'e': ex="e"; break;
      case 'i':
      case 'i':
      case 'i':
      case 'i': ex="i"; break;
      case 'a':
      case 'a':
      case 'a':
      case 'a': ex="a"; break;
      case 'E': ex="E"; break;
      case 'ae': ex="ae"; break;
      case 'AE': ex="AE"; break;
      case 'o':
      case 'o':
      case 'o':
      case 'o':
      case 'o':
      case 'o': ex="o"; break;
      case 'y': ex="y"; break;
      case 'pounds': ex="pounds"; break;
      case 'yen': ex="yen"; break;
      case 'Pt': ex="Pt"; break;
      case 'f': ex="f"; break;
      case 'n':
      case 'n': ex="n"; break;
      case '?': ex="?"; break;
      case ''':
      case ''': ex="'"; break;
      case '1/2': ex="1/2"; break;
      case '1/4': ex="1/4"; break;
      case '!': ex="!"; break;
      case '<<': ex="<<"; break;
      case '>>': ex=">>"; break;
      case '.': ex="."; break;
      case ':': ex=":"; break;
      case '#': ex="#"; break;
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+':
      case '+': ex="+"; break;
      case '-':
      case '-': ex="-"; break;
      case '|':
      case '|': ex="|"; break;
      case 'beta': ex="beta"; break;
      case 'gamma': ex="gamma"; break;
      case 'SIGMA': ex="SIGMA"; break;
      case 'sigma': ex="sigma"; break;
      case 'upsilon': ex="upsilon"; break;
      case 'delta': ex="delta"; break;
      case 'phi': ex="phi"; break;
      case 'ei': ex="ei"; break;
      case 'eta': ex="eta"; break;
      case '===': ex="==="; break;
      case '+-': ex="+-"; break;
      case '>=': ex=">="; break;
      case ' ': ex-"<="; break;
      case 'integral_top': ex="integral_top"; break;
      case 'integral_bot': ex="integral_bot"; break;
      case '/': ex="/"; break;
      case '~=': ex="~="; break;
      case 'degrees': ex="degrees"; break;
      case '.':
      case '.': ex="."; break;
      case 'sqrt': ex="sqrt"; break;
      case '^n': ex="^n"; break;
      case '^2': ex="^2"; break;
      case '.': ex="."; break;
      case 255: ex="\\xFF"; break; //Only used in the extended chars demo
      default:
        if (*buf&0x80||*buf<7)
          "%s: extended ASCII char %d '%c'\n",ST_WARN_ST,*buf,*buf;
        goto nex_char;
    }
    if (StrLen(ex)!=1) {
      buf1=StrNew(buf+1);
      *buf=0;
      s=StrAdd(s,ex);
      len=StrLen(s)-1;
      s=StrAdd(s,buf1);
      buf=s+len;
      Free(buf1);
    } else
      *buf=*ex;
nex_char:
  }
  return s;
}

U0 FilePassThrough(U8 *in_path,U8 *out_path)
{
  I64 len;
  U8 *out=FileRead(in_path,&len);
  FileWrite(out_path,out,len);
  Free(out);
}

Bool StrEqAny(U8 *s,...)
{ //Returns TRUE if any of the strings match s
  I64 i;
  for (i=0;i<argc;i++) {
    if (!StrCmp(s,argv[i]))
      return TRUE;
  }
  return FALSE;
}

U8 *StrMatchAny(U8 *s,...)
{ //Returns a pointer to the first substring matched
  U8 *res;
  I64 i;
  for (i=0;i<argc;i++) {
    if (res=StrMatch(argv[i],s))
      break;
  }
  return res;
}

U8 *UnZExtension(U8 *out_path)
{ //Changes out_path to a non-compressed extension, returns ptr to extension.
  //See [C:/Doc/Glossary.DD.Z,1] Filename Extention Types.
  U8 *ex=FileExtDot(out_path)+1;
  if (StrEqAny(ex,"TXT.Z","LOG.Z","CPP.Z","HPP.Z","PRJ.Z","MAP.Z","DAT.Z",
                   "BIN.Z","AUT.Z","GRA.Z","HC.Z","HH.Z","DATA.Z","DD.Z",
                   "IN.Z","GR.Z","BMP.Z"))
    StrLastRem(ex,".");
  else if (IsDotZ(out_path))
    ex[4]='C';
  return ex;
}

//All these string operations are crazy slow because I don't preallocate mem
//but it's good enough for now.
//This whole thing should've been designed to do one big allocation and be
//done with it but it's too late now.

U0 ExportSingle(U8 *in_path,U8 *out_path,Bool debug=FALSE)
{
  CDoc *l;
  CDocEntry *cl;
  CDocBin *b;
  U8 *out,*buf,*ex,*lnk;
  I64 len,flags=0;
  Bool is_code;
  I64 indent=0,line_len=0,prev_len=0;
  U8 *indents=StrNew("");


//Windows stuff should just be passed through with the original extension
//Big TXTs that aren't in DD format
  if (StrMatchAny(out_path,"/Windows/","/Linux/","/Sup1Bin/","/Sup1Texts/",
	"AugustWords.TXT","BibleWords.TXT","ChessWords.TXT","Countries.TXT",
	"HappyWords.TXT","Ingredients.TXT","Letters.TXT","LinuxDict.TXT",
	"HuckWords.TXT","PCIDevices.TXT","Bible.TXT","Words1000.TXT",
	"Words10k.TXT","GodDict.TXT")) {
    FilePassThrough(in_path,out_path);
    return;
  }

  ex=UnZExtension(out_path);

  if (StrEqAny(ex,"BIN.C","BIN","DAT","GRA","ISO","BMP","DATA","GR","BMP",
	"JPG","JPEG","jpg","JPEG","html","htm","HTM","HTML")) {
    FilePassThrough(in_path,out_path);
    return;
  }

  l=DocRead(in_path);
  if (!l) {
    FilePassThrough(in_path,out_path);
    return;
  }

  is_code=StrEqAny(ex,"CPP","HPP","PRJ","AUT","HC","HH","IN");

  out=StrNew("");
  for (b=l->bin_head.next;b!=&l->bin_head;b=b->next) {
    if (StrCmp(ex,"MAP"))
      ExportBinary(out_path,b);
//MP files are symbol maps and the binary data is just debug info (line nums)
//so there's no need to create hundreds of bin files just for the line nums
  }
  if (*out)
    out=StrAdd(out,"\n");
  for (cl=l->head.next;cl!=l;cl=cl->next) {
    switch (cl->type_u8) {
      case DOCT_ERROR:
      case DOCT_TEXT:
      case DOCT_ANCHOR:
      case DOCT_TREE: //Trees text includes the +] at the beginning
      case DOCT_BTTN:
        if (!cl->tag)
          break;
        if (cl->de_flags & DOCEF_CENTER_X)
          buf=IndentString((EXPORT_W-StrLen(cl->tag))/2);
        else if (cl->de_flags & DOCEF_RIGHT_X)
          buf=IndentString(EXPORT_W-StrLen(cl->tag));
        else
          buf=NULL;
        if (buf) {
//This should actually go back to the beginning of the line but having text
//before a justify tag actually breaks even in J OS itself so I assume there's
//no invalid usage anywhere
          out=StrAdd(out,buf);
          Free(buf);
        }
        if (debug) {
//CL breaks everything and the file is truncated when you open it
          len=StrLen(out);
          out=StrAdd(out,cl->tag);
          for (buf=out+len;*buf;buf++) {
            if (*buf=='$') *buf='_';
          }
        } else
          out=StrAdd(out,cl->tag);
        break;
      case DOCT_INS_BIN_SIZE:
        if (is_code)
          buf=MStrPrint("%s__BIN_%d_SIZE",out,cl->bin_data->num);
        else
          buf=MStrPrint("%s%d",out,cl->bin_data->size);
        Free(out); out=buf;
        break;
      case DOCT_INS_BIN:
        if (is_code) {
          buf=MStrPrint("%s__BIN_%d",out,cl->bin_data->num);
          Free(out); out=buf;
        } else
          out=StrAdd(out,"(binary data)");
        break;
      case DOCT_SPRITE:
        if (is_code)
          buf=MStrPrint("%s/* %s <%d> (image) */",
            out,cl->tag,cl->bin_data->num);
        else
          buf=MStrPrint("%s%s <%d> (image)",out,cl->tag,
            cl->bin_data->num);
        Free(out); out=buf;
        break;
      case DOCT_MACRO:
        if (cl->tag)
          out=StrAdd(out,cl->tag);
        else
          out=StrAdd(out,"(macro)");
        out=StrAdd(out,"\n");
        if (cl->de_flags & DOCEF_RIGHT_MACRO) {
          buf=MStrPrint("%s`%s`\n",out,cl->right_macro);
          Free(out); out=buf;
        }
        if (cl->de_flags & DOCEF_LEFT_MACRO) {
          buf=MStrPrint("%s`%s`\n",out,cl->left_macro);
          Free(out); out=buf;
        }
        break;
      case DOCT_LINK:
        if (cl->aux_str) {
          lnk=ResolveLink(cl->aux_str);
          buf=MStrPrint("%s[%s] %s",out,lnk,cl->tag);
        } else {
          lnk=ResolveLink(cl->tag);
          buf=MStrPrint("%s[%s]",out,lnk);
        }
        Free(lnk);
        Free(out); out=buf;
        break;
      case DOCT_SONG: //This is used to autoplay songs in documents
        out=StrAdd(out,cl->tag);
        out=StrAdd(out," ");
        out=StrAdd(out,cl->aux_str);
        break;
      case DOCT_NEW_LINE:
      case DOCT_SOFT_NEW_LINE:
        out=StrAdd(out,"\n");
        prev_len++;
        if (!(flags & DOCF_WORD_WRAP)) {
          line_len=indent;
          out=StrAdd(out,indents);
        } else
          line_len=0; //Wrap will take care of it
        break;
      case DOCT_TAB:
        out=StrAdd(out,"\t");
        break;
      case DOCT_BLINK:
        out=StrAdd(out,"**");
        break;
      case DOCT_UNDERLINE:
        out=StrAdd(out,"_");
        break;
      case DOCT_WORD_WRAP:
        if (cl->attr)
          flags|=DOCF_WORD_WRAP;
        else
          flags&=~DOCF_WORD_WRAP;
        break;
      case DOCT_INDENT:
        indent+=cl->attr;
//This is tricky - an indent tag effectively moves the following text, even
//if there's something before it, so if you have foo$ID,2$bar it will display
//as fbar. to handle this I'm just gonna go back to the beginning of the line
//and overwrite. Not quite the same behaviour but close enough.
//We also need to re-wrap the line.
        for (buf=out+StrLen(out)-1;*buf>out && *buf!='\n';buf--)
          line_len++;
        buf[1]=0;
        Free(indents); indents=IndentString(indent);
        out=StrAdd(out,indents);
        break;
      case DOCT_HTML_CODE:
        out=StrAdd(out,"```\n");
        out=StrAdd(out,cl->tag);
        out=StrAdd(out,"\n```");
        break;
      case DOCT_PMT: //Marks end of C:/User/Prompt> so you can't erase it
      case DOCT_FOREGROUND:
      case DOCT_BACKGROUND:
      case DOCT_DFT_FOREGROUND:
      case DOCT_DFT_BACKGROUND:
      case DOCT_LEFT_MARGIN:
      case DOCT_RIGHT_MARGIN:
      case DOCT_CURSOR_MOVEMENT:
      case DOCT_SHIFTED_X:
      case DOCT_SHIFTED_Y:
      case DOCT_CURSOR:
      case DOCT_INVERT:
      case DOCT_HEADER:
      case DOCT_FOOTER:
      case DOCT_PAGE_BREAK:
      case DOCT_HIGHLIGHT:
      case DOCT_PAGE_LEN:
        break;
      default:
        buf=MStrPrint("%s /* Unknown token: %d */ ",out,cl->type_u8);
        Free(out); out=buf;
        "%s: Unknown token: %d\n",ST_WARN_ST,cl->type_u8;
        break;
    }
    if (flags & DOCF_WORD_WRAP)
      out=Wrap(out,out+prev_len,&line_len,indent);
    prev_len=StrLen(out);
  }
  out=StrToASCII(out);
//Terry uses a mix of tabs and spaces all over the place and converting tabs
//to 8 spaces is not good enough because some formatting relies on tabs align
//so I'm just gonna leave it
  FileWrite(out_path,out,StrLen(out));
  Free(out);
  DocDel(l);
  Free(indents);
}

//Adapted from [C:/Adam/ABlkDev/ADskA.HC.Z,103] CopyTree2 which is internally used by [C:/Adam/ABlkDev/ADskA.HC.Z,122] CopyTree
U0 PutExportTree(CDirEntry *ent,I64 src_dir_len,I64 dst_dir_len,
  U8 *dst_dir,Bool debug=FALSE)
{
  U8 *path;
  while (ent) {
    path=MAlloc(dst_dir_len+StrLen(ent->full_name)+1);
    MemCpy(path,dst_dir,dst_dir_len);
    path[dst_dir_len]='/';
    StrCpy(path+dst_dir_len+1,ent->full_name+src_dir_len);
    "%s\n",path;
    if (ent->attr & RS_ATTR_DIR) {
      DirMk(path);
      if (ent->sub)
        PutExportTree(ent->sub,src_dir_len,dst_dir_len,dst_dir,debug);
    } else
      ExportSingle(ent->full_name,path,debug);
    Free(path);
    ent=ent->next;
  }
}

extern CDirEntry *FilesFind2(U8 *files_find_mask,I64 fuf_flags);
//Adapted from [C:/Adam/ABlkDev/ADskA.HC.Z,122] CopyTree
U0 ExportTreeImpl(U8 *src_mask="T:/*",U8 *dst_mask="D:/Git",
  Bool debug=FALSE)
{
  CDirContext *dirc;
  CDirEntry *ent;
  I64 dst_dir_len,src_dir_len;
  U8 *dst_dir,*src_dir;

  if (dirc=DirContextNew(src_mask,TRUE)) {
    ent=FilesFind2(dirc->mask,1<<FUf_RECURSE);
    src_dir=DirCur;
    src_dir_len=StrLen(src_dir);
    Free(src_dir);
    DirContextDel(dirc);
    if (dirc=DirContextNew(dst_mask,TRUE,TRUE)) {
      dst_dir=DirCur;
      dst_dir_len=StrLen(dst_dir);
      PutExportTree(ent,src_dir_len,dst_dir_len,dst_dir,debug);
      DirContextDel(dirc);
      Free(dst_dir);
    }
  }
}

//This is pretty neat - instead of hogging the current window and making it
//change dir, we spawn a new terminal and programmatically send keystrokes
//to start the export.

U0 ExportTree() {
  CTask *task=User("#include \"D:/Export.HC\"
ExportTreeImpl;
ExportSingle(\"D:/Export.HC\",\"D:/Git/Export.HC\");
");
  WinToTop(task);
}
