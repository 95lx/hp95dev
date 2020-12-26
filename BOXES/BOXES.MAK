# nmake makefile to create both a tkernel and a system manager compliant
# version of boxes.  The tkernel version is called boxes.exe and the
# system manager version is called boxes.exm.

all: boxes.exe boxes.exm

######## system manager compliant version

tmpboxes.obj: boxes.c
	cl /c /AS /Gs /Fotmpboxes.obj boxes.c

tmpboxes.exe: tmpboxes.obj
	link @<<tmpboxes.lnk
/M /NOE /NOI tmpboxes.obj+..\tools\csvc.obj+..\tools\crt0.obj
tmpboxes.exe
tmpboxes.map;
<<NOKEEP

boxes.exm: tmpboxes.exe
	e2m tmpboxes
	copy tmpboxes.exm boxes.exm
	del tmpboxes.exm



######## tkernel version

boxes.obj: boxes.c
	cl /c /AS /Zi /DTKERNEL /Foboxes.obj boxes.c

boxes.exe: boxes.obj
	link @<<boxes.lnk
/CO /M /NOE /NOI boxes.obj+..\tools\csvc.obj
boxes.exe
boxes.map;
<<NOKEEP


