# Microsoft Developer Studio Project File - Name="OS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OS.mak" CFG="OS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OS - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OS - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "OS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f OS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "OS.exe"
# PROP BASE Bsc_Name "OS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f OS.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "OS.exe"
# PROP Bsc_Name "OS.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "OS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f OS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "OS.exe"
# PROP BASE Bsc_Name "OS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "OSCOMP"
# PROP Rebuild_Opt "/a"
# PROP Target_File "OS.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "OS - Win32 Release"
# Name "OS - Win32 Debug"

!IF  "$(CFG)" == "OS - Win32 Release"

!ELSEIF  "$(CFG)" == "OS - Win32 Debug"

!ENDIF 

# Begin Group "CPP"

# PROP Default_Filter ".CPP"
# Begin Source File

SOURCE=..\..\KERNEL\CPP\COMM.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\DISK.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\EDIT.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\EDIT.MEN
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\FAT32.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\FIFO.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\FORM.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\HELP.TXT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\INPTR.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\JOY.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\ROOT.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\ROOT2.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\RULES.TXT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\SPRINTF.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\STRINGS.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\SYSTEM.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\SYSTEXT.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\TEMP.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\TODO.TXT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\USER.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\USER.MEN
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\UTILS.CPP
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\CPP\WINDOW.CPP
# End Source File
# End Group
# Begin Group "ASM"

# PROP Default_Filter ".ASM,.INC"
# Begin Source File

SOURCE=..\..\KERNEL\ASM\BOOT.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\CALLS.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\CHARIO.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\CMD.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\CODE.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\COMPILE.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\COMPILE.INC
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\CRT.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\DEBUG.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\END.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\GRMODE.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\IRQHAND.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\KEYBD.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\LEX.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\LTF.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\MAP.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\MEMORY.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OPT.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSINC.INC
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSINC2.INC
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSMAIN.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\PARSE.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\SCHED.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\STRING.ASM
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\SYSUTILS.ASM
# End Source File
# End Group
# Begin Group "BAT"

# PROP Default_Filter ".BAT"
# Begin Source File

SOURCE=..\..\KERNEL\ASM\BU.BAT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\doit.bat
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSCOMP.BAT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSFILES.TXT
# End Source File
# Begin Source File

SOURCE=..\..\KERNEL\ASM\OSLINK.BAT
# End Source File
# End Group
# End Target
# End Project
