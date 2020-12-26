# nmake makefile to create both a tkernel and a system manager compliant
# version of smhello.  The tkernel version is called smhello.exe and the
# system manager version is called smhello.exm.

all: smhello.exe smhello.exm

# system manager compliant version

smhello.exm: jsmhello.exe
	..\tools\e2m jsmhello
	copy jsmhello.exm smhello.exm
	del jsmhello.exm

jsmhello.exe: jsmhello.obj
	link @<<jsmhello.lnk
/M /NOE /NOI jsmhello.obj+..\tools\csvc.obj+..\tools\crt0.obj
jsmhello.exe
jsmhello.map;
<<NOKEEP

jsmhello.obj: smhello.c
	cl /c /AS /Gs /Fojsmhello.obj smhello.c

# tkernel version

smhello.exe: smhello.obj
	link @<<smhello.lnk
/M /NOE /NOI smhello.obj+..\tools\csvc.obj
smhello.exe
smhello.map;
<<NOKEEP

smhello.obj: smhello.c
	cl /c /AS /Fosmhello.obj smhello.c
