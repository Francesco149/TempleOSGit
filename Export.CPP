//This is NOT Terry's work. I wrote this program to export historical
//versions of TempleOS to git.
//The goal is to obtain something easily browsable in a normal text editor
//and easy to diff in git while not loosing too much information. Docs are
//converted to plaintext but retain links and a few decorations, binary data
//is dumped to a DAT file for docs (embedded as hex strings in code in
//old versions).
//As an added bonus, I try to match each iteration's coding style.
//This code is public domain.

OptOn(OPTf_WARN_PAREN);

//Just in case the filesystem gets corrupted while I develop it
U0 BackupExport()
{
  Copy("E:/Export.CPP","C:/Export.CPP");
  Copy("E:/Export.CPP","E:/ExportB.CPP");
  Copy("E:/Export.CPP","C:/ExportB.CPP");
}

//Appears to hang on this version...
//BackupExport

#define EXPORT_W 78

U0 ExportBinary(U8 *src_file,CDocBin *b)
{
  U64 len;
  U8 *buf,*buf1;

  len=StrLen(src_file);
  buf1=MAlloc(len+1);
  StrCpy(buf1,src_file);
  buf=buf1+len-1;
  while (buf>buf1 && *buf!='.')
    buf--;
  *buf=0;
  buf=MSPrintF("%s%d.DAT",buf1,b->num);
  Free(buf1);
  "%s\n",buf;
  FileWrite(buf,b->data,b->size);
  Free(buf);
}

U8 *ResolveLink(U8 *link_st)
{ //Adapted from [C:/SparrowOS/Adam/Doc/DocMain.CPP.Z,3] EdJoin. Resolves stuff like MN:Func to the file
  U8 *res,*filename=NULL,*pattern=NULL;
  I64 num;
  if (EdLinkCvt(link_st,&filename,&pattern,&num,0)==-1)
    res=StrNew(link_st);
  else if (num!=0)
    res=MSPrintF("%s,%d",filename,num);
  else
    res=MSPrintF("%s,%s",filename,pattern);
  Free(filename);
  Free(pattern);
//Temporary hack to prevent random diff's on every file link
  if (!StrNCmp(res,"D:/",3))
    res[0]='C';
  return res;
}

U8 *IndentString(I64 n)
{
  U8 *s=MAlloc(n+1);
  MemSet(s,' ',n);
  s[n]=0;
  return s;
}

U8 *Wrap(U8 *base,U8 *s,I64 *line_len,I64 indent=0)
{ //Performs word wrapping on base, starting from s, line_len is state.
  //Also adds indent spaces to every soft and hard newline.
  //Returns new value for base.
  //This is supposed to also take tabs into account but fuck that shit
  //NOTE: at this point I could use [C:/SparrowOS/Adam/Window.CPP.Z,723] SetWinHorz and append to a CDoc
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
  U8 *ext,*buf,*buf1;
  U64 len;
  for (buf=s;*buf;buf++) {
    switch (*buf) {
      case CH_SHIFT_SPACE: ext=" "; break; //Special space, not conv. to tabs
      case CH_CURSOR: ext=""; break; //Cursor position
      case 'pi': ext="pi"; break;
      case 'inf': ext="inf"; break;
      case 'theta': ext="theta"; break;
      case 'phi': ext="phi"; break;
      case 'omega': ext="omega"; break;
      case '-': ext="-"; break;
      case 'u': ext="u"; break;
      case 'alpha': ext="alpha"; break;
      case 255: ext="\\xFF"; break; //Only used in the extended chars demo
      default:
        if (*buf&0x80)
          "W: extended ASCII char: '%c'\n",*buf;
        goto next_char;
    }
    if (StrLen(ext)!=1) {
      buf1=StrNew(buf+1);
      *buf=0;
      s=StrAdd(s,ext);
      len=StrLen(s)-1;
      s=StrAdd(s,buf1);
      buf=s+len;
      Free(buf1);
    } else
      *buf=*ext;
next_char:
  }
  return s;
}

U0 FilePassThrough(U8 *in_path,U8 *out_path)
{
  U64 len;
  U8 *out=FileRead(in_path,&len);
  FileWrite(out_path,out,len);
  Free(out);
}

Bool StrEqAny(U8 *s,...)
{ //Returns TRUE if any of the strings match s
  U64 i;
  for (i=0;i<argc;i++) {
    if (!StrCmp(s,argv[i]))
      return TRUE;
  }
  return FALSE;
}

U8 *StrStrAny(U8 *s,...)
{ //Returns a pointer to the first substring matched
  U8 *res;
  U64 i;
  for (i=0;i<argc;i++) {
    if (res=StrStr(s,argv[i]))
      break;
  }
  return res;
}

U8 *UnZExtension(U8 *out_path)
{ //Changes out_path to a non-compressed extension, returns ptr to extension.
  //See [::/SparrowOS/Doc/Glossary.TXT.Z,1] Filename Extention Types.
  U8 *ext=FileExtDot(out_path)+1;
  if (StrEqAny(ext,"TXT.Z","LOG.Z","CPP.Z","HPP.Z","PRJ.Z","MAP.Z","DAT.Z",
                   "BIN.Z","AUT.Z","GRA.Z"))
    ext[3]=0;
  else if (IsDotZ(out_path))
    ext[4]='C';
  return ext;
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
  U8 *out,*buf,*ext,*lnk;
  U64 len,flags=0;
  Bool is_code;
  I64 indent=0,line_len=0,prev_len=0;
  U8 *indents=StrNew("");

//Windows stuff should just be passed through with the original extension
  if (StrStrAny(out_path,"/Windows/","/Linux/")) {
    FilePassThrough(in_path,out_path);
    return;
  }

  ext=UnZExtension(out_path);

  if (StrEqAny(ext,"BIN.C","BIN","DAT","GRA","ISO","BMP")) {
    FilePassThrough(in_path,out_path);
    return;
  }

  for (buf=ext;buf>out_path && *buf!='/';buf--);
  buf++;

  if (StrEqAny(buf,"Bible.TXT","PCIDevices.TXT")) {
//Special case: I know these large files don't have any Doc syntax so no point
//in wasting time parsing them
//NOTE: PCIDevices contains non-ASCII but they shouldn't be converted because
//it's not SparrowOS extended ASCII, it was probably created in Linux/Windows
    FilePassThrough(in_path,out_path);
    return;
  }

  l=DocRead(in_path);
  if (!l) {
    FilePassThrough(in_path,out_path);
    return;
  }

  is_code=StrEqAny(ext,"CPP","HPP","PRJ","AUT");

  out=StrNew("");
  for (b=l->bin_root.next;b!=&l->bin_root;b=b->next) {
    if (StrCmp(ext,"MAP"))
      ExportBinary(out_path,b);
//MP files are symbol maps and the binary data is just debug info (line nums)
//so there's no need to create hundreds of bin files just for the line nums
  }
  if (*out)
    out=StrAdd(out,"\n");
  for (cl=l->root.next;cl!=l;cl=cl->next) {
    switch (cl->type_u8) {
      case DOCT_ERROR:
      case DOCT_TEXT:
      case DOCT_ANCHOR:
      case DOCT_TREE: //Trees text includes the +] at the beginning
      case DOCT_BUTTON:
        if (!cl->tag)
          break;
        if (cl->flags1 & DOCEF1_CENTER_X)
          buf=IndentString((EXPORT_W-StrLen(cl->tag))/2);
        else if (cl->flags1 & DOCEF1_RIGHT_X)
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
      case DOCT_INS_BINARY_TYPE:
        if (is_code)
          buf=MSPrintF("%s__BIN_%d_TYPE",out,cl->bin_data->num);
        else
          buf=MSPrintF("%s%d",out,cl->bin_data->type);
        Free(out); out=buf;
        break;
      case DOCT_INS_BINARY_SIZE:
        if (is_code)
          buf=MSPrintF("%s__BIN_%d_SIZE",out,cl->bin_data->num);
        else
          buf=MSPrintF("%s%d",out,cl->bin_data->size);
        Free(out); out=buf;
        break;
      case DOCT_INS_BINARY:
        if (is_code) {
          buf=MSPrintF("%s__BIN_%d",out,cl->bin_data->num);
          Free(out); out=buf;
        } else
          out=StrAdd(out,"(binary data)");
        break;
      case DOCT_SPRITE:
        if (is_code)
          buf=MSPrintF("%s/* %s <%d> (image) */",
            out,cl->tag,cl->bin_data->num);
        else
          buf=MSPrintF("%s%s <%d> (image)",out,cl->tag,
            cl->bin_data->num);
        Free(out); out=buf;
        break;
      case DOCT_MACRO:
        if (cl->tag)
          out=StrAdd(out,cl->tag);
        else
          out=StrAdd(out,"(macro)");
        out=StrAdd(out,"\n");
        if (cl->flags1 & DOCEF1_RIGHT_MACRO) {
          buf=MSPrintF("%s`%s`\n",out,cl->right_macro);
          Free(out); out=buf;
        }
        if (cl->flags1 & DOCEF1_LEFT_MACRO) {
          buf=MSPrintF("%s`%s`\n",out,cl->left_macro);
          Free(out); out=buf;
        }
        break;
      case DOCT_LINK:
        if (cl->aux_str) {
          lnk=ResolveLink(cl->aux_str);
          buf=MSPrintF("%s[%s] %s",out,lnk,cl->tag);
        } else {
          lnk=ResolveLink(cl->tag);
          buf=MSPrintF("%s[%s]",out,lnk);
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
      case DOCT_HIDE_START:
        out=StrAdd(out,"<-");
        break;
      case DOCT_HIDE_END:
        out=StrAdd(out,">");
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
      case DOCT_FOREGROUND:
      case DOCT_BACKGROUND:
      case DOCT_LINK_FOREGROUND:
      case DOCT_MACRO_FOREGROUND:
      case DOCT_TREE_FOREGROUND:
      case DOCT_BINARY_FOREGROUND:
      case DOCT_USER_FOREGROUND:
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
      case DOCT_HTML_CODE:
      case DOCT_HTML_IMG:
      case DOCT_HTML_VIDEO:
      case DOCT_HIGHLIGHT:
        break;
      default:
        buf=MSPrintF("%s /* Unknown token: %d */ ",out,cl->type_u8);
        Free(out); out=buf;
        "Unknown token: %d\n",cl->type_u8;
        break;
    }
    if (flags & DOCF_WORD_WRAP)
      out=Wrap(out,out+prev_len,&line_len,indent);
    prev_len=StrLen(out);
  }
//Special case: this file just contains garbage because there's a hex view
  if (!StrStr(out_path,"/SparrowOS/Demo/MultiCore/RadixSort.CPP"))
    out=StrToASCII(out);
//Terry uses a mix of tabs and spaces all over the place and converting tabs
//to 8 spaces is not good enough because some formatting relies on tabs align
//so I'm just gonna leave it
  FileWrite(out_path,out,StrLen(out));
  Free(out);
  DocDel(l);
  Free(indents);
}

//Adapted from [C:/SparrowOS/Adam/Dsk/Dsk2a.CPP.Z,128] PutCopyTree which is internally used by [C:/SparrowOS/Adam/Dsk/Dsk2a.CPP.Z,148] CopyTree
U0 PutExportTree(CDirEntry *ent,U64 src_dir_len,U64 dst_dir_len,
  U8 *dst_dir,Bool debug=FALSE)
{
  U8 *path;
  while (ent) {
    path=MAlloc(dst_dir_len+StrLen(ent->full_name)+1);
    MemCpy(path,dst_dir,dst_dir_len);
    path[dst_dir_len]='/';
    StrCpy(path+dst_dir_len+1,ent->full_name+src_dir_len);
    "%s\n",path;
    if (ent->attr & _ATTR_DIR) {
      MkDir(path);
      if (ent->sub)
        PutExportTree(ent->sub,src_dir_len,dst_dir_len,dst_dir,debug);
    } else
      ExportSingle(ent->full_name,path,debug);
    Free(path);
    ent=ent->next;
  }
}

//Adapted from [C:/SparrowOS/Adam/Dsk/Dsk2a.CPP.Z,148] CopyTree
U0 ExportTreeImpl(U8 *src_mask="T:/*",U8 *dst_mask="E:/Git",
  Bool debug=FALSE)
{
  CFileAccess *fa;
  CDirEntry *ent;
  U64 dst_dir_len,src_dir_len;
  U8 *dst_dir,*src_dir;

  fa=FileAccessNew(src_mask,TRUE);
  if (fa) {
    ent=SysFilesFind(fa->mask,1<<FUf_RECURSE);
    src_dir=CurDir;
    src_dir_len=StrLen(src_dir);
    Free(src_dir);
    FileAccessDel(fa);
    fa=FileAccessNew(dst_mask,TRUE,TRUE);
    if (fa) {
      dst_dir=CurDir;
      dst_dir_len=StrLen(dst_dir);
      PutExportTree(ent,src_dir_len,dst_dir_len,dst_dir,debug);
      FileAccessDel(fa);
      Free(dst_dir);
    }
    CDirLstDel(ent);
  }
}

//This is pretty neat - instead of hogging the current window and making it
//change dir, we spawn a new terminal and programmatically send keystrokes
//to start the export.

U0 ExportTree() {
  CTask *task=SpawnUser("#include \"E:/Export.CPP\"
ExportTreeImpl;
ExportSingle(\"E:/Export.CPP\",\"E:/Git/Export.CPP\");
");
  WinToTop(task);
}
