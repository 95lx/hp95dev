;************************************************************************
;*									*
;*	CRT0.ASM							*
;*									*
;*	C program startup module for use with HP 95LX System Manager	*
;*	compliant applications.						*
;*									*
;*	Copyright 1991 Hewlett-Packard Company				*
;*	All rights reserved						*
;*									*
;*	This module should be assembled as:				*
;*									*
;*		MASM -Mx crt0;						*
;*									*
;************************************************************************

;*** The DOSSEG directive ensures proper ordering of the segments

	DOSSEG

;*** Only SMALL model programs are allowed as external SysMgr applications.

	.MODEL	small

;*** This is standard C startup stuff:

	public	__acrtused	; trick to force in startup

__acrtused = 9876h		; funny value not easily matched in SYMDEB

	extrn	_main:near

	.STACK	2048		; should be overridden at link time

	.CODE

;*** And now for the amazing C startup procedure:

_astart proc near

	jmp	_main		; just jump to MAIN function

;*** MAIN should *NOT* do an EXIT or a RETURN!!!! It *MUST* use the SysMgr
;*** function m_fini() to terminate!!!!

_astart endp

	end	_astart		
