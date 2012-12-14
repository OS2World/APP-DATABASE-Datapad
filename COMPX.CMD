BCC -c -sm padhd.c padxtra.c
TLINK /aa /Toe /c /LC:\BCOS2\LIB c02 padhd.obj about.obj add.obj import.obj edit.obj padxtra.obj search.obj, padhd, padhd,c2mt os2,
BRCC -r padhd.rc
RC padhd.res padhd.exe
