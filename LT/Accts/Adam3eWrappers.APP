//Change anything you want in this
//acct file or others.

#help_index "Utils;Cmd Line (Typically)"

//These are customized quick commands.
//Feel free to add more or change.
public U64 F(U8 *pattern,U8 *fu_flags=NULL)
{
  return Grep(pattern,"/LT/"TEXT_FILE_MASK,fu_flags);
}

public U64 R(U8 *pattern,U8 *replace_text=NULL,U8 *fu_flags="+l-i")
{
  return Grep(pattern,"/LT/"TEXT_FILE_MASK,fu_flags,replace_text);
}

public U64 FD(U8 *pattern,U8 *fu_flags=NULL)
{
  return Grep(pattern,TEXT_FILE_MASK,fu_flags);
}

public U64 RD(U8 *pattern,U8 *replace_text=NULL,U8 *fu_flags="+l-i")
{
  return Grep(pattern,TEXT_FILE_MASK,fu_flags,replace_text);
}
#help_index ""
