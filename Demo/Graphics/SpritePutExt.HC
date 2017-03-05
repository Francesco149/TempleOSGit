/*If you press <CTRL-t> you will see this:

$IB,"<1>",BI=1,BP="::/Demo/Graphics/SpritePut.HC,2"$

That imports an external sprite.  It's
called a [C:/Kernel/KernelA.HH.Z,979] DOCEF_BIN_PTR_LINK.  See [C:/Doc/DolDocOverview.DD.Z,1] BP="".

Press <CTRL-r> and "Ptr to Sprite".
Then, enter a file and the number of
the sprite in the file.  Note: the
number can get messed-up if you change
the order of sprites in the source file.
You can link to a tag string instead of a num.
*/

U0 SpritePutExt()
{
  I64 i;
  for (i=0;i<3;i++) {
    "US Map:";
    Sprite(__BIN_1);
    "%h12c",'\n'; //"h" is a secondary arg.  [C:/Doc/Print.DD.Z,1] Print 12 newline chars.
  }
}

SpritePutExt;
