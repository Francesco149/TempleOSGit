If you insist on doing AOT compiled, old school, programs instead of  
just-in-time compiled programs, this directory tree has some examples.  The 
main reason to do AOT programs is to make closed-source applications.  Why
not 
do it like the programs in /TempleOS/Apps, instead?

See [C:/TempleOS/Doc/ImportExports.TXT.Z,1] Scoping and Linkages for details
on extern, import, _extern, _import, etc.

Demo1 shows a multiple file AOT module.


Demo2 shows a multiple module project.  If you #include the MakeFile you'll 
see a clear example of how syms are exported and imported by modules and
bound 
to syms in task's hash tables.

 
