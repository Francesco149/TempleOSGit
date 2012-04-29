This demonstrates that linking can be done
at LOAD time.  This is the LoseThos answer
to OBJ files.  

Right-Click MakeFile.CPZ and click INCLUDE or
do the following:

1) Do a [C:/LT/OSMain/Dsk/Dsk1b.CPZ,236] Dir() command

2) Right-Click on OD1.SPZ and click COMPILE.
   Right-Click on OD2.SPZ and click COMPILE.
   Right-Click on ODMain.SPZ and click COMPILE.

3) In any order, do the following and ignore warnings.
   Right-Click on OD1.BIZ and click LOAD.
   Right-Click on ODMain.BIZ and click LOAD.
   Right-Click on OD2.BIZ and click LOAD.

4) Right-Click on ObjDemo.HPZ and click INCLUDE.

5) Type at the command-line:
  >ObjDemo("Param1","Param2");
  


#include the file [::/LT/Adam/BinRep.APZ] ::/LT/Adam/BinRep.APZ and
call [C:/LT/Adam/BinRep.APZ,22] BinRep() to analyze the .BIZ files.  
