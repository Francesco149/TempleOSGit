This demonstrates that linking can be done
at LOAD time.  This is the SparrowOS answer
to OBJ files.  

Right-Click MakeFile.CPP.Z and click INCLUDE or
do the following:

1) Do a [C:/SparrowOS/OSMain/Dsk/Dsk1b.CPP.Z,233] Dir() command

2) Right-Click on OD1.PRJ.Z and click COMPILE.
   Right-Click on OD2.PRJ.Z and click COMPILE.
   Right-Click on ODMain.PRJ.Z and click COMPILE.

3) In any order, do the following and ignore warnings.
   Right-Click on OD1.BIN.Z and click LOAD.
   Right-Click on ODMain.BIN.Z and click LOAD.
   Right-Click on OD2.BIN.Z and click LOAD.

4) Right-Click on ObjDemo.HPP.Z and click INCLUDE.

5) Type at the command-line:
  >ObjDemo("Param1","Param2");
  


#include the file [::/SparrowOS/Adam/BinRep.CPP.Z,1] ::/SparrowOS/Adam/BinRep.CPP.Z and
call [C:/SparrowOS/Adam/BinRep.CPP.Z,23] BinRep() to analyze the .BIN.Z files.  
