;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;л Small HP 95LX SysMgr compliant application to display currently open	л
;л SysMgr applications.							л
;л									л
;л Copyright 1991 Hewlett Packard Company				л
;л All rights reserved.							л
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

SYSMGR		equ	1

E_NONE		equ	0
E_KEY		equ	1
E_BREAK		equ	2
E_ACTIV		equ	3
E_DEACT		equ	4
E_TERM		equ	5
E_BRIDGE	equ	6
E_GROW		equ	7
E_SHRINK	equ	8
E_ALARM_EXP	equ	9
E_ALARM_DAY	equ	10
E_TIMECHANGE	equ	11

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

ifdef EMUL
ImagevecSeg  equ    9000h     ;
else
ImagevecSeg  equ    0F000h     ;
endif 

ImagevecOff0 equ    7         ;
ImagevecSize equ    34h       ;

PS_CLOSED       equ     0       ; not loaded in memory, no bits set
PS_ACTIVE       equ     1       ; foreground process
PS_SUSP         equ     2       ; suspended 
PS_EXIT         equ     4       ; task called m_fini, waiting to be unloaded
PS_YIELD        equ     8       ; task called m_yield, goes to end of queue
PS_EXIT_REFUSED equ     10h     ; task called m_no_fini

PS_NOWAIT       equ     1h      ; MASK for the nowait bit
PS_123          equ     1       ; Bit mask for 123


; must maintain size of 48 bytes

TCB     STRUC
        t_sp          dw ?       ; sp at last context sw/out
        t_ss          dw ?       ; ss at last context sw/out
        t_off_image   dw ?       ; task's imagevec offset
        t_seg_image   dw ?       ; task's imagevec segment
        t_ds          dw ?       ; saved DS register
        t_memseg      dw ?       ; base segment of allocation
        t_hotkey      dw ?       ; hotkey copied from imagevec
        t_membank     db 6 dup (?) ; memory mapping info.
        t_chipsel     db 6 dup (?) ; chip select array
        t_state       db ?       ; task's state(CLOSE,ACTIVE,SUSP,BACK,EXIT)
        t_is123       db ?       ; this task is 123 (and other flags)
        t_rsrc_seg    dw ?
        t_nowait      db ?       ; is task just testing for keys?
        t_extra       db ?       ; unused
        t_extname     db 12 dup(?) ; name of app as presented to user
        t_extra4      db 4 dup (?) ; unusused
TCB     ENDS

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

if SYSMGR

	DOSSEG

	.model	small

	.stack	2048


;* NOTE!!! ALL *INITIALIZED* data must go in the .DATA group.
;*	  ALL *UN-INITIALIZED* data must go in the .DATA? group
;*	This is because the un-initialized data doesn't get stored in the
;*	program file like the initialized data does, and the SysMgr (and E2M)
;*	know how to setup the uninitialized data space for you.

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

	.data
else

cseg	segment	para public 'code'
	assume	cs:cseg, ds:cseg
	org	100h
start:
	jmp	begin
endif

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

Header1		db	"What's Running?                         "
Header3		db	"ЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭЭ"
t0str		db	"JTASK0.EXE  ", 0
StateMsg	db	"Active      "
		db	"Suspended   "
		db	"Exited      "
		db	"Yielded     "
		db	"Exit refused"

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* NOTE: ALL items in the UN-INITIALIZED data group MUST be declared in the
;*	following format:
;*		label	opc	num dup (?)
;*	where 'label' is the name of the item, opc is DB, DW, DD, etc, and
;*	'num' is the number of (bytes, words, longs, etc) that you want.
;*	The value in the ()'s MUST be a question mark.  NO constants of any
;*	sort can be used in this group.

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

if SYSMGR
	.data?
endif

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

start_init_data	label	byte
;
StartNum	dw	1 dup (?)		; first item in table displayed
;
scratbuf	db	41 dup (?)		; format buffer
;
currow		dw	1 dup (?)
TableBase	dw	1 dup (?)		; base of TCB table (offset)
;
event		dw	1 dup (?)		; kind
evdata		dw	1 dup (?)		; data
evscan		db	1 dup (?)		; scan
		db	1 dup (?)		; shifts
		dw	1 dup (?)		; lics
		db	1 dup (?)		; fkey_num
		dd	1 dup (?)		; bridge
;
len_init_data	equ	$-start_init_data

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

if SYSMGR
	.code

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

	public	start

	assume	es:dgroup
start:

;* the following two lines are the requisite call to m_init() to log this
;* process into the SysMgr's stack.

	mov	di,0600h		; m_init()
	int	60h

else

begin:
	
endif

	cld
	push	ds			; DS points to our data space at entry
	pop	es			; make ES point there too

;* the following four lines of code zero out our uninitialized data space.

	lea	di,start_init_data	; point to our uninitialized data space
	mov	cx,len_init_data	; length of our uninitialized data space
	xor	al,al			; get a 0
	rep	stosb			; zero our uninitialized data space

	sti				; make sure interrupts are going

;* Now, find the TCB (Task Control Block) table in memory

	push	ds			; save our DS
	int	61h			; get the SysMgr data space segment
	push	ds
	pop	es			; put it in ES
	pop	ds			; restore our DS

	lea	si,t0str		; point to string to search for
	xor	di,di			; where to start search
	call	findstr			; find it
	jnc	foundtable		; jif found it
terminate:

if SYSMGR
	mov	di,0601h		; m_fini()
	int	60h
else
	mov	ax,4c00h
	int	21h
endif
;
foundtable:
	sub	di,32+48		; move back to start of table
	mov	TableBase,di		; save it
;
exec_redisp:

;* DISPLAY CURRENT MESSAGE

	call	do_screen		; show current screen in current mode

;* MAIN EVENT DISPATCH LOOP

exec:
	call	getkey			; don't come back until a key is ready

	cmp	al,27			; ESC key?
	jz	terminate		; jif yes

	jmp	exec

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* ISKEY checks for a key, returns NZ if a key is available, else Z

iskey:

if SYSMGR
	mov	di,0101h		; m_nevent()
	call	get_event

	mov	ax,event
	cmp	ax,E_KEY		; keystroke?
	jz	gotiskey		; jif yes

	cmp	ax,E_ACTIV		; activating?
	jz	is_activate		; jif yes

	cmp	ax,E_DEACT		; de-activating?
	jz	is_deactivate		; jif yes

	cmp	ax,E_TERM		; terminate?
	jz	terminate		; jif yes
iskeyfail:
	xor	ax,ax			; set ZR flag
	ret
gotiskey:
	xor	ax,ax
	inc	ax			; set NZ flag
	mov	ax,evdata		; get keycode
else
	mov	ah,1
	int	16h
endif
	ret


;* THE SYSTEM MANAGER FIRST SENDS A DE-ACTIVATE EVENT.  THE NEXT TIME YOU CALL
;* THE SYSMGR TO GET AN EVENT, IT DOESN'T RETURN UNTIL IT'S TIME TO RE-ACTIVATE
;* YOUR AP.

is_deactivate:
	jmp	iskey

;* WE'VE JUST BEEN RE-ACTIVATED, SO WE NEED TO REDRAW THE SCREEN

is_activate:
	call	activate_draw
	jmp	iskey

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* HANGS UNTIL A KEY IS AVAILABLE

getkey:

if SYSMGR
	mov	di,0100h		; m_event()
	call	get_event

	mov	ax,event
	cmp	ax,E_KEY		; keystroke?
	jz	gotkey			; jif yes

	cmp	ax,E_ACTIV		; activating?
	jz	activate		; jif yes

	cmp	ax,E_DEACT		; de-activating?
	jz	deactivate		; jif yes

	cmp	ax,E_TERM		; terminate?
	jnz	getkey			; jif no, ignore
	jmp	terminate
gotkey:
	mov	ax,evdata		; get keycode
else
	mov	ah,0
	int	16h
endif
	ret

;* THE SYSTEM MANAGER FIRST SENDS A DE-ACTIVATE EVENT.  THE NEXT TIME YOU CALL
;* THE SYSMGR TO GET AN EVENT, IT DOESN'T RETURN UNTIL IT'S TIME TO RE-ACTIVATE
;* YOUR AP.

deactivate:
	jmp	getkey

;* WE'VE JUST BEEN RE-ACTIVATED, SO WE NEED TO REDRAW THE SCREEN

activate:
	call	activate_draw
	jmp	getkey

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

activate_draw:
	push	ds
	pop	es			; make sure ES=DS

	call	do_screen

	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

do_screen:
	call	clear_screen

	xor	ax,ax			; x
	mov	bx,-3			; y
	xor	dx,dx			; STYLE
	lea	di,Header1
	mov	cx,40
	call	smstring

	xor	ax,ax			; x
	mov	bx,-1			; y
	xor	dx,dx			; STYLE
	lea	di,Header3
	mov	cx,40
	call	smstring

	push	ds
	pop	es			; put our data segment into ES
	int	61h			; get SysMgr data segment into DS

	cld

lea	di,scratbuf
mov	ax,ds
call	formax
mov	al,':'
stosb
mov	ax,es:TableBase
call	formax
mov	cx,di
lea	di,scratbuf
sub	cx,di
xor	ax,ax
mov	bx,-2
xor	dx,dx
push	ds
push	es
pop	ds
call	smstring
pop	ds

	mov	ax,es:StartNum
	mov	cx,17
	sub	cx,ax			; number of remaining entries possible

	shl	ax,1
	shl	ax,1
	shl	ax,1
	shl	ax,1			; *16
	mov	si,ax
	shl	ax,1			; *32
	add	si,ax			; *48
	add	si,es:TableBase		; = ptr to table entry to start with

	mov	ch,11			; max # screen lines
ds1:
	test	byte ptr [si].t_state,PS_ACTIVE+PS_SUSP+PS_EXIT+PS_YIELD+PS_EXIT_REFUSED
	jnz	ds2
dsnext:
	add	si,48			; move to next entry
	dec	cl			; dec number of remaining table entries
	jnz	ds1			; jif not done
dsexit:
	push	es
	pop	ds
	ret
ds2:
	lea	di,scratbuf
	mov	al,32
	stosb
	stosb
	xor	bx,bx
ds3:
	mov	al,[si+bx].t_extname
	stosb
	inc	bx
	cmp	bx,12
	jb	ds3			; loop til name is copied

	mov	al,32
	stosb
	stosb

	mov	al,[si].t_state		; get its state
	push	si
	push	ds
	push	cx

	push	es
	pop	ds

	xor	si,si
ds5:
	shr	al,1
	jc	ds4
	add	si,12
	jmp	ds5
ds4:
	add	si,offset StateMsg

	mov	dl,11
	sub	dl,ch
	xor	dh,dh			; row#

	mov	cx,12
	rep	movsb

	xor	ax,ax			; column
	mov	bx,dx			; row
	mov	cx,28			; len
	xor	dx,dx			; STYLE
	lea	di,scratbuf		; adr
	call	smstring

	pop	cx
	pop	ds
	pop	si
	dec	ch
	jnz	dsnext
	jmp	dsexit

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

smstring:
if SYSMGR
	xor	si,si			; OSTYLE
	push	si
	push	dx			; STYLE
	push	cx			; len of string
	push	ds
	push	di			; adr of string
	push	ax			; column#
	push	bx			; row#
	mov	di,0300h		; m_disp function numbers
	push	di
	push	di			; dummy ret adr
	int	60h			; call SysMgr
	add	sp,18			; clean up stack
else
	mov	si,di
sms1:
	push	cx
	lodsb
	mov	dl,al
	mov	ah,2
	int	21h
	pop	cx
	loop	sms1
endif
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* GET_EVENT provides common code for doing an M_EVENT() or M_NEVENT() call
;* depending upon the contents of DI at entry.

get_event:
	lea	ax,event
	push	ds
	push	ax			; push far ptr to 'event' structure
	push	di			; csvc arg
	push	di			; dummy ret adr (as if CSVC called)
	int	60h
	add	sp,8
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

clear_screen:

if SYSMGR
	mov	ax,40			; clear 40 columns
	push	ax
	mov	ax,16			;       16 rows
	push	ax
	xor	ax,ax			; starting at column 0
	push	ax
	mov	ax,-3
	push	ax			;                row -3
	mov	di,0301h		; F_M_CLEAR = 256*SC_SCREEN + 1
					;  (major/minor SysMgr function #)
	push	di
	push	di			; dummy CSVC ret addr
	int	60h			; call SysMgr
	add	sp,12			; clean up stack
endif
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; FINDSTR --- at entry:
;		DS:SI = ptr to string to find
;		ES:DI = ptr to memory area in which to find it
;	      at exit:
;		CY if not found
;		NC if found and ES:DI points to the start of the found string
findstr:
	cld
fs1:
	push	si			; save addr of string to find
	lodsb				; get first char of string
fs2:
	cmp	al,es:[di]		; check search area
	jz	fs3			; jif match
	inc	di
	jnz	fs2			; jif not past end of search area
fsfail:
	pop	si
	stc
	ret
fs3:
	push	di			; save start of maybe found string
	inc	di
fs4:
	lodsb				; get next char of string to find
	or	al,al			; end of string?
	jz	fsfound			; jif yes, done

	cmp	al,es:[di]		; match?
	jnz	fs5			; jif no
	inc	di
	jnz	fs4			; jif not end of search area to chk next
	pop	di
	jmp	fsfail
fs5:
	pop	di
	inc	di
	pop	si
	jmp	fs1
fsfound:
	pop	di			; recover adr of found string
	pop	si			; and adr of search string
	clc
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

formax:
	push	ax
	call	formah
	pop	ax
	mov	ah,al
formah:
	shr	ax,1
	shr	ax,1
	shr	ax,1
	shr	ax,1
	shr	al,1
	shr	al,1
	shr	al,1
	shr	al,1
	and	ax,0f0fh
	add	ax,3030h
	cmp	al,'9'
	jbe	alok
	add	al,7
alok:
	cmp	ah,'9'
	jbe	ahok
	add	ah,7
ahok:
	xchg	al,ah
	stosw
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

@curseg	ends
	end	start
