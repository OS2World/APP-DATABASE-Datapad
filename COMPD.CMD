BCC -c -sm -v padhd.c about.c add.c import.c edit.c padxtra.c search.c
TLINK /aa /Toe /c /v /LC:\BCOS2\LIB c02 padhd.obj about.obj add.obj import.obj edit.obj padxtra.obj search.obj, padhd, padhd,c2mt os2,
BRCC -r padhd.rc
RC padhd.res padhd.exe
