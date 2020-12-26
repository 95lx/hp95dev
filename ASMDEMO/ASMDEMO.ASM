;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;л Sample ASSEMBLY shell for an HP 95LX SysMgr compliant application	л
;л Copyright 1991 Hewlett Packard Company				л
;л All rights reserved.							л
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

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

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

msgcredit	db	"Many thanks to all involved, especially:",0
		db	"Dan T     Margaret K          Eric V",0
		db	"Tim W     Jeanette S          Mark B",0
		db	"Jerry E   Jeanine B           Chris B",0
		db	"                              Randy S",0
		db	"Eric E    Howard H  Terry B   Bob W",0
		db	"Stan B    Kevin B   Dennis E",0
		db	"Jean G    Dennis E  Ralph K   Bill W",0
		db	"Bill J    Randy H   Jack M    Ted B",0
		db	"Everett K Greg M    Dave S    Diana B",0
		db	"Pat M     Dan R     Jerry S   Gabe E",0
		db	"Jim P     Dennis S            Max J",0
		db	"Bruce S             Dennis Y  Dave O",0
		db	"Paul S    Preston B Steve H   Charlie P",0
		db	"Susan W   Mike B    Tom L",0
		db	"Lynn W",0
		db	0
msgcredit2	db	"Don C     Dave V    Kent S",0
		db	"Bill F    Tammy B   Norbert H",0
		db	"Dan H     Cary H    John A",0
		db	"Tom H     Debbie C  Karen O",0
		db	"Scott R   Ken C     Mark G",0
		db	"Diana R   Dan P     Thanhhoa H",0
		db	"Sandy A   Raan Y    Fred R",0
		db	"Sheri D   John S    Steve S",0
		db	"                    Greg U",0
		db	"Gary P    John D",0
		db	"Michael D Dolores E",0
		db	"          Dave G",0
		db	"Tom P     Tom H",0
		db	"Carl J    Dianne T",0
		db	" ",0
		db	" ",0
		db	0
msgcredit3	db	"Carmen W  Cindy B   Al B       Sarah H",0
		db	"Kermit Y  Nolan B   Mike B     Mark B",0
		db	"                    Peter B    Diane K",0
		db	"Kent H    Neal B    Rich B     Richard M",0
		db	"Shyam J   Jane B    Rob C      Kevin R",0
		db	"Randy P   Georgia C Ann C      Barbara W",0
		db	"Jack P    Peter K   Rich dF",0
		db	"          Sarah L   Roger D    Leslie I",0
		db	"Tina L    Ginny M   Dave G     Joanne S",0
		db	"Debbie H            Dan M",0
		db	"Mattie F  Eric S    Leon N",0
		db	"Lucy H    Mary Jo H Martin S",0
		db	"Claire S  Carol N   Eric S",0
		db	"Eva B     Ron B",0
		db	"Rhonda R  Bernie C",0
		db	"           ...and a cast of thousands...",0
		db	0

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* NOTE: ALL items in the UN-INITIALIZED data group MUST be declared in the
;*	following format:
;*		label	opc	num dup (?)
;*	where 'label' is the name of the item, opc is DB, DW, DD, etc, and
;*	'num' is the number of (bytes, words, longs, etc) that you want.
;*	The value in the ()'s MUST be a question mark.  NO constants of any
;*	sort can be used in this group.

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

	.data?

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

start_init_data	label	byte
;
dispnum		dw	1 dup (?)
dispmode	db	1 dup (?)
;
currow		dw	1 dup (?)
nextstr		dw	1 dup (?)
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

	.code

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

	public	start

start:
	cld

;* the following two lines are the requisite call to m_init() to log this
;* process into the SysMgr's stack.

	mov	di,0600h		; m_init()
	int	60h

	push	ds			; DS points to our data space at entry
	pop	es			; make ES point there too

;* the following four lines of code zero out our uninitialized data space.

	lea	di,start_init_data	; point to our uninitialized data space
	mov	cx,len_init_data	; length of our uninitialized data space
	xor	al,al			; get a 0
	rep	stosb			; zero our uninitialized data space

	call	my_init			; call my initialization routine

	sti				; make sure interrupts are going

exec_redisp:

;* DISPLAY CURRENT MESSAGE

	call	do_screen		; show current screen in current mode

;* MAIN EVENT DISPATCH LOOP

exec:
	call	getkey			; don't come back until a key is ready

	cmp	al,27			; ESC key?
	jz	terminate		; jif yes

	cmp	al,13			; ENTER key?
	jnz	nextscreen

	not	byte ptr dispmode	; switch display modes
	cmp	dispmode,0		; alpha?
	jz	go_alpha

	mov	ax,0020h		; G_Mode(G_GRAPHICS);
	int	5fh
	jmp	exec_redisp
go_alpha:
	mov	ax,0007h		; G_Mode(G_ALPHA);
	int	5fh
	jmp	exec_redisp
;
nextscreen:
	mov	ax,dispnum		; increment the current display number
	inc	ax
	cmp	ax,3
	jb	@F
	xor	ax,ax			; wrap back to first screen
@@:
	mov	dispnum,ax

	call	do_screen		; show that screen

	jmp	exec
;
terminate:

;* FIRST DO MY DE-INITIALIZATION

	call	my_fini

;* THEN TERMINATE TO THE SYSMGR (DO *NOT* USE NORMAL DOS PGM TERMINATE SEQUENCE)

	mov	di,0601h		; m_fini()
	int	60h

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* ISKEY checks for a key, returns NZ if a key is available, else Z

iskey:
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
	ret

;* THE SYSTEM MANAGER FIRST SENDS A DE-ACTIVATE EVENT.  THE NEXT TIME YOU CALL
;* THE SYSMGR TO GET AN EVENT, IT DOESN'T RETURN UNTIL IT'S TIME TO RE-ACTIVATE
;* YOUR AP.

is_deactivate:
	call	my_fini
	jmp	iskey

;* WE'VE JUST BEEN RE-ACTIVATED, SO WE NEED TO REDRAW THE SCREEN

is_activate:
	call	activate_draw
	jmp	iskey

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

;* HANGS UNTIL A KEY IS AVAILABLE

getkey:
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
	ret

;* THE SYSTEM MANAGER FIRST SENDS A DE-ACTIVATE EVENT.  THE NEXT TIME YOU CALL
;* THE SYSMGR TO GET AN EVENT, IT DOESN'T RETURN UNTIL IT'S TIME TO RE-ACTIVATE
;* YOUR AP.

deactivate:
	call	my_fini
	jmp	getkey

;* WE'VE JUST BEEN RE-ACTIVATED, SO WE NEED TO REDRAW THE SCREEN

activate:
	call	activate_draw
	jmp	getkey

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

activate_draw:
	push	ds
	pop	es			; make sure ES=DS

	call	my_init			; do my initialization
	call	do_screen

	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

do_screen:
	call	clear_screen

	mov	ax,dispnum

	lea	di,msgcredit		; point to first possible screen
	or	ax,ax			; screen 0?
	jz	ds_draw

	lea	di,msgcredit2
	dec	ax			; screen 1?
	jz	ds_draw

	lea	di,msgcredit3
ds_draw:
	xor	dx,dx			; init y-coord/row to 0

	cmp	dispmode,0		; alpha?
	jz	ds_alpha		; jif yes
ds_loop:
	xor	cx,cx			; set x-coord to 0
	mov	ax,0f00h
	int	5fh

	add	dx,8			; move down a row on display

	mov	si,di			; now skip to next string
ds_skip:
	lodsb
	or	al,al
	jnz	ds_skip
	mov	di,si
	cmp	byte ptr [di],0
	jnz	ds_loop

	ret
;
;
;
ds_alpha:
	sub	dx,3			; SysMgr 0,0 is third row of display
ds_aloop:
	mov	currow,dx		; save current row number

	mov	ax,0			; OSTYLE (not really used)
	push	ax
	mov	ax,0			; STYLE
	push	ax
	mov	si,di
ds_count:
	lodsb
	or	al,al			; eol?
	jnz	ds_count		; jif no

	mov	cx,si
	sub	cx,di			; get length of string
	dec	cx			; adjust for NULL char
	push	cx

	mov	nextstr,si		; save ptr to next string

	push	ds
	push	di			; address of string (far ptr)

	xor	ax,ax			; column=0
	push	ax

	push	dx			; push row
	mov	di,0300h		; m_disp major/minor function numbers
	push	di
	push	di			; dummy ret adr
	int	60h			; call SysMgr
	add	sp,18			; clean up stack

	mov	di,nextstr
	mov	dx,currow
	inc	dx

	cmp	byte ptr [di],0
	jnz	ds_aloop		; loop til done

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
	cmp	dispmode,0
	jnz	clear_graphics

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
	ret

clear_graphics:
	call	pen0			; G_ColorSel(0);
	xor	ax,ax
	xor	bx,bx
	mov	cx,239
	mov	dx,127
	call	rectangle		; draw solidfilled rect 0,0 to 239,127
	call	pen1			; G_ColorSel(1);
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

pen1:
	mov	ax,0901h
pencom:
	int	5fh
	ret
;
pen0:
	mov	ax,0900h
	jmp	pencom

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

rectangle:
	push	cx
	push	dx
	mov	cx,ax
	mov	dx,bx
	mov	ah,8			; G_Move(cx,dx)
	int	5fh
	pop	dx
	pop	cx
	mov	ax,0501h		; G_Rect(cx,dx,G_SOLIDFILL)
rectend:
	int	5fh
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

my_init:
	cmp	dispmode,0		; alpha mode?
	jz	noinit			; jif yes

;* GOTO GRAPHICS

	mov	ax,0020h		; goto GRAPHICS
	int	5fh
noinit:
	ret

my_fini:
	cmp	dispmode,0		; alpha mode?
	jz	nouninit		; jif yes

;* GOTO TEXT

	mov	ax,0007h		; goto TEXT
	int	5fh
nouninit:
	ret

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

@curseg	ends
	end	start
