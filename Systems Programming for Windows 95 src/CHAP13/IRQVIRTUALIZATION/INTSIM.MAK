# Microsoft Visual C++ generated build script - Do not modify

PROJ = INTSIM
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\VXDBOOK\CHAP13\IRQVIR~2\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /Zi /AM /Od /D "_DEBUG" /FR /GA /Fd"INTSIM.PDB" 
CFLAGS_R_WEXE = /nologo /G3 /W3 /AM /O1 /D "NDEBUG" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /CO /ALIGN:16 /STACK:10240
LFLAGS_R_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /ALIGN:16 /STACK:10240
LIBS_D_WEXE = mafxcwd oldnames libw commdlg shell olecli olesvr mlibcew
LIBS_R_WEXE = mafxcw oldnames libw commdlg shell olecli olesvr mlibcew
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = INTSIM.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		INTSIM.SBR


STDAFX_DEP = c:\vxdbook\chap13\irqvir~2\stdafx.h


INTSIM_DEP = c:\vxdbook\chap13\irqvir~2\stdafx.h \
	c:\vxdbook\chap13\irqvir~2\intsim.h


all:	$(PROJ).EXE $(PROJ).BSC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

INTSIM.OBJ:	INTSIM.CPP $(INTSIM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c INTSIM.CPP

INTSIM.RES:	INTSIM.RC $(INTSIM_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r INTSIM.RC


$(PROJ).EXE::	INTSIM.RES

$(PROJ).EXE::	STDAFX.OBJ INTSIM.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
INTSIM.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\ddk\lib\+
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) INTSIM.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	INTSIM.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) INTSIM.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
