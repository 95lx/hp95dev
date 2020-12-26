# nmake makefile to create both a tkernel and a system manager compliant
# version of smdemo.  The tkernel version is called smdemo.exe and the
# system manager version is called smdemo.exm.

all: smdemo.exe smdemo.exm

######## system manager compliant version

tmpsmdem.obj: smdemo.c
    cl /c /AS /Gs /Fotmpsmdem.obj smdemo.c

tmpsmdem.exe: tmpsmdem.obj 
    link @<<tmpsmdem.lnk
/M /NOE /NOI tmpsmdem.obj+..\tools\csvc.obj+..\tools\crt0.obj
tmpsmdem.exe
tmpsmdem.map;
<<NOKEEP

smdemo.exm: tmpsmdem.exe
    e2m tmpsmdem
    copy tmpsmdem.exm smdemo.exm
    del tmpsmdem.exm



######## tkernel version

smdemo.obj: smdemo.c
    cl /c /AS /Zi /Fosmdemo.obj smdemo.c

smdemo.exe: smdemo.obj
    link @<<smdemo.lnk
/CO /M /NOE /NOI smdemo.obj+..\tools\csvc.obj
smdemo.exe
smdemo.map;
<<NOKEEP


