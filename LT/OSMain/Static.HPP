#exe {KeepPrivate(ON);}

#include "/LT/OSMain/BootDefs.HPZ"
#include "/LT/OSMain/Adam1a.HPZ"
#exe {Externs2Imports(ON);};
#include "/LT/Compiler/CmpDefs.HPZ"
#include "/LT/OSMain/Adam1b.HPZ"
#include "/LT/OSMain/Adam1c.HPZ"
#exe {
  Externs2Imports(OFF);
  KeepPrivate(OFF);
};


