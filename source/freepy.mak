# Microsoft Developer Studio Generated NMAKE File, Based on freepy.dsp
!IF $(CFG)" == "
CFG=freepy - Win32 Debug
!MESSAGE No configuration specified. Defaulting to freepy - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "freepy - Win32 Release" && "$(CFG)" != "freepy - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freepy.mak" CFG="freepy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freepy - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "freepy - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freepy - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\freepy.dll"


CLEAN :
	-@erase "$(INTDIR)\freepy.obj"
	-@erase "$(INTDIR)\freepy.res"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\handle.obj"
	-@erase "$(INTDIR)\HZlib.obj"
	-@erase "$(INTDIR)\HZmodei.obj"
	-@erase "$(INTDIR)\HZmodenormal.obj"
	-@erase "$(INTDIR)\HZmodephtoch.obj"
	-@erase "$(INTDIR)\HZmodeu.obj"
	-@erase "$(INTDIR)\HZmodev.obj"
	-@erase "$(INTDIR)\HZspecial.obj"
	-@erase "$(INTDIR)\HZutil.obj"
	-@erase "$(INTDIR)\imm.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\uicand.obj"
	-@erase "$(INTDIR)\uicomp.obj"
	-@erase "$(INTDIR)\uistatus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\freepy.dll"
	-@erase "$(OUTDIR)\freepy.exp"
	-@erase "$(OUTDIR)\freepy.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEPY_EXPORTS" /Fp"$(INTDIR)\freepy.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\freepy.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freepy.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\freepy.pdb" /machine:I386 /def:".\freepy.def" /out:"$(OUTDIR)\freepy.dll" /implib:"$(OUTDIR)\freepy.lib" 
DEF_FILE= \
	".\freepy.def"
LINK32_OBJS= \
	"$(INTDIR)\freepy.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\handle.obj" \
	"$(INTDIR)\HZlib.obj" \
	"$(INTDIR)\HZmodei.obj" \
	"$(INTDIR)\HZmodenormal.obj" \
	"$(INTDIR)\HZmodephtoch.obj" \
	"$(INTDIR)\HZmodeu.obj" \
	"$(INTDIR)\HZmodev.obj" \
	"$(INTDIR)\HZspecial.obj" \
	"$(INTDIR)\HZutil.obj" \
	"$(INTDIR)\imm.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\uicand.obj" \
	"$(INTDIR)\uicomp.obj" \
	"$(INTDIR)\uistatus.obj" \
	"$(INTDIR)\freepy.res"

"$(OUTDIR)\freepy.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\freepy.dll" "$(OUTDIR)\freepy.bsc"


CLEAN :
	-@erase "$(INTDIR)\freepy.obj"
	-@erase "$(INTDIR)\freepy.res"
	-@erase "$(INTDIR)\freepy.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\handle.obj"
	-@erase "$(INTDIR)\handle.sbr"
	-@erase "$(INTDIR)\HZlib.obj"
	-@erase "$(INTDIR)\HZlib.sbr"
	-@erase "$(INTDIR)\HZmodei.obj"
	-@erase "$(INTDIR)\HZmodei.sbr"
	-@erase "$(INTDIR)\HZmodenormal.obj"
	-@erase "$(INTDIR)\HZmodenormal.sbr"
	-@erase "$(INTDIR)\HZmodephtoch.obj"
	-@erase "$(INTDIR)\HZmodephtoch.sbr"
	-@erase "$(INTDIR)\HZmodeu.obj"
	-@erase "$(INTDIR)\HZmodeu.sbr"
	-@erase "$(INTDIR)\HZmodev.obj"
	-@erase "$(INTDIR)\HZmodev.sbr"
	-@erase "$(INTDIR)\HZspecial.obj"
	-@erase "$(INTDIR)\HZspecial.sbr"
	-@erase "$(INTDIR)\HZutil.obj"
	-@erase "$(INTDIR)\HZutil.sbr"
	-@erase "$(INTDIR)\imm.obj"
	-@erase "$(INTDIR)\imm.sbr"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Process.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\uicand.obj"
	-@erase "$(INTDIR)\uicand.sbr"
	-@erase "$(INTDIR)\uicomp.obj"
	-@erase "$(INTDIR)\uicomp.sbr"
	-@erase "$(INTDIR)\uistatus.obj"
	-@erase "$(INTDIR)\uistatus.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\freepy.bsc"
	-@erase "$(OUTDIR)\freepy.dll"
	-@erase "$(OUTDIR)\freepy.exp"
	-@erase "$(OUTDIR)\freepy.ilk"
	-@erase "$(OUTDIR)\freepy.lib"
	-@erase "$(OUTDIR)\freepy.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEPY_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\freepy.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\freepy.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\freepy.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\freepy.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\handle.sbr" \
	"$(INTDIR)\HZlib.sbr" \
	"$(INTDIR)\HZmodei.sbr" \
	"$(INTDIR)\HZmodenormal.sbr" \
	"$(INTDIR)\HZmodephtoch.sbr" \
	"$(INTDIR)\HZmodeu.sbr" \
	"$(INTDIR)\HZmodev.sbr" \
	"$(INTDIR)\HZspecial.sbr" \
	"$(INTDIR)\HZutil.sbr" \
	"$(INTDIR)\imm.sbr" \
	"$(INTDIR)\Process.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\uicand.sbr" \
	"$(INTDIR)\uicomp.sbr" \
	"$(INTDIR)\uistatus.sbr"

"$(OUTDIR)\freepy.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=IMM32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\freepy.pdb" /debug /machine:I386 /def:".\freepy.def" /out:"$(OUTDIR)\freepy.dll" /implib:"$(OUTDIR)\freepy.lib" /pdbtype:sept 
DEF_FILE= \
	".\freepy.def"
LINK32_OBJS= \
	"$(INTDIR)\freepy.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\handle.obj" \
	"$(INTDIR)\HZlib.obj" \
	"$(INTDIR)\HZmodei.obj" \
	"$(INTDIR)\HZmodenormal.obj" \
	"$(INTDIR)\HZmodephtoch.obj" \
	"$(INTDIR)\HZmodeu.obj" \
	"$(INTDIR)\HZmodev.obj" \
	"$(INTDIR)\HZspecial.obj" \
	"$(INTDIR)\HZutil.obj" \
	"$(INTDIR)\imm.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\uicand.obj" \
	"$(INTDIR)\uicomp.obj" \
	"$(INTDIR)\uistatus.obj" \
	"$(INTDIR)\freepy.res"

"$(OUTDIR)\freepy.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("freepy.dep")
!INCLUDE "freepy.dep"
!ELSE 
!MESSAGE Warning: cannot find "freepy.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "freepy - Win32 Release" || "$(CFG)" == "freepy - Win32 Debug"
SOURCE=.\freepy.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\freepy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\freepy.obj"	"$(INTDIR)\freepy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\freepy.rc

"$(INTDIR)\freepy.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\global.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\global.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\handle.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\handle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\handle.obj"	"$(INTDIR)\handle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZlib.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZlib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZlib.obj"	"$(INTDIR)\HZlib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodei.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZmodei.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZmodei.obj"	"$(INTDIR)\HZmodei.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodenormal.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZmodenormal.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZmodenormal.obj"	"$(INTDIR)\HZmodenormal.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodephtoch.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZmodephtoch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZmodephtoch.obj"	"$(INTDIR)\HZmodephtoch.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodeu.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZmodeu.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZmodeu.obj"	"$(INTDIR)\HZmodeu.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZmodev.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZmodev.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZmodev.obj"	"$(INTDIR)\HZmodev.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZspecial.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZspecial.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZspecial.obj"	"$(INTDIR)\HZspecial.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HZutil.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\HZutil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\HZutil.obj"	"$(INTDIR)\HZutil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\imm.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\imm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\imm.obj"	"$(INTDIR)\imm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Process.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\Process.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\Process.obj"	"$(INTDIR)\Process.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\subs.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\subs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\subs.obj"	"$(INTDIR)\subs.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uicand.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\uicand.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\uicand.obj"	"$(INTDIR)\uicand.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uicomp.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\uicomp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\uicomp.obj"	"$(INTDIR)\uicomp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\uistatus.c

!IF  "$(CFG)" == "freepy - Win32 Release"


"$(INTDIR)\uistatus.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "freepy - Win32 Debug"


"$(INTDIR)\uistatus.obj"	"$(INTDIR)\uistatus.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

