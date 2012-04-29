/*
The +D flag causes a define-string
look-up and substitution.

It is available in the <CTRL-L> menu
under text entries for use in
documentation.
 
*/


DefineLoad("COMPANY","Trivial Solutions");
"Company: $TX+D,\"\",\"COMPANY\"$\n";

DefinePrintF("COMPANY_TIME","%T",GetCurTimeLTDate);

#exe {
  StreamPrintF(
    "#define COMPANY_AGE %h56D\n",GetCurTimeLTDate-A2LTDate("8/1/2003"));
};


"Age : $TX+D,\"\",\"COMPANY_AGE\"$ years\n";
"Time: $TX+D,\"\",\"COMPANY_TIME\"$\n";
