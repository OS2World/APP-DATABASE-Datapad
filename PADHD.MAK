#=============================================================
#
#	PADHD.MAK - Makefile for project c:\padhd\padhd.prj
#		Created on 07/24/95 at 00:14
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +PADHD.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = C:\BCOS2\LIB
INCLUDEPATH = C:\BCOS2\INCLUDE


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================
LINK_EXCLUDE =  \
 padhd.res

LINK_INCLUDE =  \
 search.obj \
 padhd.obj \
 import.obj \
 edit.obj \
 add.obj \
 about.obj \
 padxtra.obj \
 pstruct.obj \
 padhd.def

#=============================================================
#		Explicit Rules
#=============================================================
padhd.exe: padhd.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)
  $(TLINK) /v /wdef /wmsk /Toe /aa /L$(LIBPATH) @&&|
C:\BCOS2\LIB\C02.OBJ+
search.obj+
padhd.obj+
import.obj+
edit.obj+
add.obj+
about.obj+
padxtra.obj+
pstruct.obj
padhd,padhd
C:\BCOS2\LIB\C2MT.LIB+
C:\BCOS2\LIB\OS2.LIB
padhd.def
|
  rc.exe PADHD.res padhd.exe

#=============================================================
#		Individual File Dependencies
#=============================================================
SEARCH.obj: padhd.cfg SEARCH.C 

PADHD.res: padhd.cfg PADHD.RC 
	brcc.exe -R -I$(INCLUDEPATH) -FO PADHD.res PADHD.RC

PADHD.obj: padhd.cfg PADHD.C 

IMPORT.obj: padhd.cfg IMPORT.C 

EDIT.obj: padhd.cfg EDIT.C 

ADD.obj: padhd.cfg ADD.C 

ABOUT.obj: padhd.cfg ABOUT.C 

PADXTRA.obj: padhd.cfg PADXTRA.C 

PSTRUCT.obj: padhd.cfg PSTRUCT.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
padhd.cfg: padhd.mak
  copy &&|
-RT-
-xp
-xd-
-x-
-j150
-L$(LIBPATH)
-I$(INCLUDEPATH)
-vi-
-sm
-v
-w-obs
-w-aus
-w-eff
-w-par
-w-ofp
| padhd.cfg


