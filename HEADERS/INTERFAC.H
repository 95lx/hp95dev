/*
 * Module interfac.h  Version 1.3  Date 07/30/91  Time 15:16:36
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
 * INTERFAC.H -- prototypes and macros for System Services
 * 		 Can be used by callers from applications and
 *		 from the System Manager itself.
 ****************************************************************/

/**************************************************************
  Services that are implemented in C in the System Manager
  are accessed through a common interface routine:

	c_service()

  Each of the applicable services has a macro implemented here.
  In addition to the documented arguments for the services,
  the macros makes the call to service with an argument 
  identifying the function.


  The extra constant is the service identifier.  This should be
  a word sized value, with the high byte equaling the service 
  class, and the low byte  equal to service number.  These 
  values must match the values in the general services 
  dispatcher (SERVICES.ASM) and the class specific dispatchers.  

  Note that ALL
  pointer arguments are to be FAR (4 bytes) when passed to the
  System Manager, despite what the specification may currently
  say.	


  Finally, the macro itself makes a call to c_service(), with
  the above constants used as the first argument.  Following
  that are the actual arguments as supplied.  IMPORTANT NOTE: 
  all pointer arguments should be cast INSIDE THE MACRO 
  DEFINITION to far.  This is the simplest way to make sure
  that near pointers inside applications get expanded to the
  proper size.



 **************************************************************

  All other functions that may be called by applications should
  have function prototypes in this file.  Again, ALL pointers
  will be FAR.  By using the prototype, the compiler will push	
  the segment value onto the stack, even though the parameter
  actually supplied may be a near pointer. 

  Each of these functions must have assembly language wrappers
  written to make the SERVICES calls.

 **************************************************************


 





/**************************************************************

 Below are function prototypes for the various forms of
 c_service(), depending on return type.  The services 
 below should use the correct form in order to specify
 the proper return value of the function.
 The two pointer versions of the call are just labels
 in csvc.asm to c_service() itself, thus only one
 function is called.
 -mdb

 **************************************************************/
 


int c_service(unsigned int,...);
void near * c_service_near_ptr (unsigned int,...);
void far * c_service_far_ptr(unsigned int,...);






/* assign constants for the service class numbers */
/* SC_MAXNUM is the maximum class number allowed */

#define SC_FP		0
#define SC_EVENT	1
#define SC_MENU 	2
#define SC_SCREEN	3
#define SC_EDIT 	4
#define SC_FILE 	5
#define SC_PM		6
#define SC_CB		7
#define SC_SOUND	8
#define SC_MM		9
#define SC_BG		10
#define SC_DTIME	11
#define SC_PRINTER	12
#define SC_CONFIG	13
#define SC_COMM 	14
#define SC_MISC 	15
#define SC_VIDVIR	16	
#define SC_VIDPHY	17	/* not used directly by accessories */
#define SC_GRDISP	18
#define SC_KEY		19
#define SC_RSRC		20
#define SC_HELP 	21
#define SC_COL		22

#define SC_MAXNUM	22	/* for error check */



/* SC_EVENT */

#define F_M_EVENT	(SC_EVENT * 256) + 0
#define m_event(a)	\
	c_service(F_M_EVENT,(void far *)(a) )

#define F_M_NEVENT    (SC_EVENT * 256) + 1
#define m_nevent(a)	\
	c_service(F_M_NEVENT,(void far *)(a))


#define F_M_SH_STATUS (SC_EVENT * 256) + 2
#define m_sh_status()	\
	c_service(F_M_SH_STATUS)

/* (SC_EVENT * 256) + 3 is reserved */

#define F_M_BRIDGE_SERV (SC_EVENT * 256) + 4
#define bridge_serv(a) \
	c_service(F_M_BRIDGE_SERV,(void far *)(a))

#define F_M_FLUSH_KB	(SC_EVENT * 256) + 5
#define m_flush_kb()	\
	c_service(F_M_FLUSH_KB)

#define F_M_YIELD	(SC_EVENT * 256) + 6
#define m_yield(a)	\
	c_service(F_M_YIELD,(void far *)(a) )

#define F_M_NO_FINI	(SC_EVENT * 256) + 7
#define m_no_fini(a)	\
	c_service(F_M_NO_FINI,(void far *)(a) )





/*SC_MENU */


#define F_MENU_SETUP	(SC_MENU * 256) + 0
#define menu_setup(a,b,c,d,e,f,g)	\
	c_service(F_MENU_SETUP,(void far *)(a),(void far *)(b), \
		(c),(d),(void far *)(e),(f),(void far *)(g) )

#define F_MENU_DIS	(SC_MENU * 256) + 1
#define menu_dis(a)	\
	c_service(F_MENU_DIS,(void far *)(a))


#define F_MENU_ON	(SC_MENU * 256) + 2
#define menu_on(a)	\
	c_service(F_MENU_ON,(void far *)(a))

#define F_MENU_OFF	(SC_MENU * 256) + 3
#define menu_off(a)	\
	c_service(F_MENU_OFF,(void far *)(a))

#define F_MENU_KEY	(SC_MENU * 256) + 4
#define menu_key(a,b,c) \
	c_service(F_MENU_KEY,(void far *)(a),(b),(void far *)(c))


#define F_FMENU_INIT	  (SC_MENU * 256) + 5
#define fmenu_init(a,b,c,d,e)	  \
	c_service(F_FMENU_INIT,(void far *)(a),(void far *)(b),(char far *)(c),(d),(e))


#define F_FMENU_DIS	 (SC_MENU * 256) + 6
#define fmenu_dis(a,b)	  \
	c_service(F_FMENU_DIS,(void far *)(a),(void far *)(b))

#define F_FMENU_KEY	 (SC_MENU * 256) + 7
#define fmenu_key(a,b,c) \
	c_service(F_FMENU_KEY,(void far *)(a),(void far *)(b),(c))

#define F_FMENU_OFF	  (SC_MENU * 256) + 8
#define fmenu_off(a,b)	 \
	c_service(F_FMENU_OFF,(void far *)(a),(void far *)(a))




/*SC_SCREEN*/


#define F_M_DISP	(SC_SCREEN * 256) + 0
#define m_disp(a,b,c,d,e,f)		\
	c_service(F_M_DISP,a,b,(void far *)(c),d,e)

#define F_M_CLEAR	(SC_SCREEN * 256) + 1
#define m_clear(a,b,c,d)	     \
	c_service(F_M_CLEAR,a,b,c,d)

#define F_M_SCROLL	(SC_SCREEN * 256) + 2
#define m_scroll(a,b,c,d,e)		\
	c_service(F_M_SCROLL,a,b,c,d,e)

#define F_M_XCHG	(SC_SCREEN * 256) + 3
#define m_xchg(a,b,c,d,e)	      \
	c_service(F_M_XCHG,a,b,c,d,(void far *)(e))

#define F_M_CHRATTR	(SC_SCREEN * 256) + 4
#define m_chrattr(a,b)		   \
	c_service(F_M_CHRATTR,(void far *)(a),b)

#define F_M_CHRRVRT	(SC_SCREEN * 256) + 5
#define m_chrrvrt(a,b,c,d)	       \
	c_service(F_M_CHRRVRT,a,b,(void far *)(c),d)

/* (SC_SCREEN * 256) + 6 is reserved */

#define F_M_CHRINV	(SC_SCREEN * 256) + 7
#define m_chrinv(a,b,c) 	    \
	c_service(F_M_CHRINV,a,b,c)

#define F_M_ROWS_COLS	(SC_SCREEN * 256) + 8
#define m_rows_cols()		  \
	c_service(F_M_ROWS_COLS)

#define F_M_SETMODE	(SC_SCREEN * 256) + 9
#define m_setmode(a)		\
	c_service(F_M_SETMODE,a)

#define F_M_GETMODE	(SC_SCREEN * 256) +10
#define m_getmode()		\
	c_service(F_M_GETMODE)

#define F_M_SETCUR	(SC_SCREEN * 256) +11
#define m_setcur(a,b)		 \
	c_service(F_M_SETCUR,a,b)



/*SC_EDIT*/

#define F_EDIT_INIT	(SC_EDIT * 256) + 0
#define edit_init(a,b,c,d,e,f)		\
	c_service(F_EDIT_INIT,(void far *)(a),(void far *)(b),c,d,e,f)

#define F_EDIT_TOP	(SC_EDIT * 256) + 1
#define edit_top(a,b,c,d,e,f,g,h)	   \
	c_service(F_EDIT_TOP,(void far *)(a),(void far *)(b),c,d,(void far *)(e), \
		   f,(void far *)(g),h)

#define F_EDIT_DIS	(SC_EDIT * 256) + 2
#define edit_dis(a)			\
	c_service(F_EDIT_DIS,(void far *)(a))

#define F_EDIT_KEY	(SC_EDIT * 256) + 3
#define edit_key(a,b,c) 		\
	c_service(F_EDIT_KEY,(void far *)(a),b,(void far *)(c))

#define F_MDIT_INI	(SC_EDIT * 256) + 4
#define mdit_ini(a,b,c,d,e,f,g,h,i,j)        \
	c_service(F_MDIT_INI,(void far *)(a),b,c,d,e,(void far *)(f),g,h,i,(void far *)j)

#define F_MDIT_DIS	(SC_EDIT * 256) + 5
#define mdit_dis(a)		       \
	c_service(F_MDIT_DIS,(void far *)(a))

#define F_MDIT_KEY	(SC_EDIT * 256) + 6
#define mdit_key(a,b)			\
	c_service(F_MDIT_KEY,(void far *)(a),b)

#define F_MDIT_FIL	(SC_EDIT * 256) + 7
#define mdit_fil(a,b)			\
	c_service(F_MDIT_FIL,(void far *)(a),(void far *)(b))


#define F_MDIT_MARK	(SC_EDIT * 256) + 8
#define mdit_mark(a)			\
	c_service(F_MDIT_MARK,(void far *)(a))

#define F_MDIT_UNMARK	(SC_EDIT * 256) + 9
#define mdit_unmark(a)			\
	c_service(F_MDIT_UNMARK,(void far *)(a))


#define F_MDIT_CUTMARK	(SC_EDIT * 256) + 10
#define mdit_cutmark(a)			\
	c_service(F_MDIT_CUTMARK,(void far *)(a))

#define F_MDIT_INS_STR  (SC_EDIT * 256) + 11
#define mdit_ins_str(a,b,c)                     \
        c_service(F_MDIT_INS_STR,(void far *)(a),(void far *)(b),c)



/*SC_FILE*/

#define F_M_OPEN	(SC_FILE * 256) + 0
#define m_open(a,b,c,d,e)		\
	c_service(F_M_OPEN,(void far *)(a),(void far *)(b),c,d,e)

#define F_M_OPENRO	(SC_FILE * 256) + 1
#define m_openro(a,b,c,d,e)		  \
	c_service(F_M_OPENRO,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_CREATE	(SC_FILE * 256) + 2
#define m_create(a,b,c,d,e)		  \
	c_service(F_M_CREATE,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_FCREAT	(SC_FILE * 256) + 3
#define m_fcreat(a,b,c,d,e)		  \
	c_service(F_M_FCREAT,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_READ	(SC_FILE * 256) + 4
#define m_read(a,b,c,d) 	      \
	c_service(F_M_READ,(void far *)(a),(void far *)(b),c,(void far *)(d))


#define F_M_WRITE	(SC_FILE * 256) + 5
#define m_write(a,b,c)		     \
	c_service(F_M_WRITE,(void far *)(a),(void far *)(b),c)


#define F_M_SEEK	(SC_FILE * 256) + 6
#define m_seek(a,b,c)		    \
	c_service(F_M_SEEK,(void far *)(a),b,c)


#define F_M_TELL	(SC_FILE * 256) + 7
#define m_tell(a,b)		  \
	c_service(F_M_TELL,(void far *)(a),(void far *)(b))


#define F_M_CLOSE	(SC_FILE * 256) + 8
#define m_close(a)		 \
	c_service(F_M_CLOSE,(void far *)(a))


#define F_M_SETPAT	(SC_FILE * 256) + 9
#define m_setpat(a,b,c,d)		\
	c_service(F_M_SETPAT,(void far *)(a),(void far *)(b),c,d)


#define F_M_MATCH	(SC_FILE * 256) + 10
#define m_match(a,b)		   \
	c_service(F_M_MATCH,(void far *)(a),(void far *)(b))


#define F_M_IDENT	(SC_FILE * 256) + 11
#define m_ident(a,b,c,d)	       \
	c_service(F_M_IDENT,(void far *)(a),b,c,(void far *)(d))


#define F_M_DELETE	(SC_FILE * 256) + 12
#define m_delete(a,b,c) 	      \
	c_service(F_M_DELETE,(void far *)(a),b,c)


#define F_M_RENAME	(SC_FILE * 256) + 13
#define m_rename(a,b,c,d,e,f)		    \
	c_service(F_M_RENAME,(void far *)(a),b,c,(void far *)(d),e,f)


#define F_M_GETDIR	(SC_FILE * 256) + 14
#define m_getdir(a,b,c) 	      \
	c_service(F_M_GETDIR,a,(void far *)(b),(void far *)(c))


#define F_M_SETDIR	(SC_FILE * 256) + 15
#define m_setdir(a,b)		    \
	c_service(F_M_SETDIR,(void far *)(a),b)


#define F_M_VOLUME	(SC_FILE * 256) + 16
#define m_volume(a,b)		    \
	c_service(F_M_VOLUME,(void far *)(a),(void far *)(b))


#define F_M_MKDIR	(SC_FILE * 256) + 17
#define m_mkdir(a,b,c)		     \
	c_service(F_M_MKDIR,(void far *)(a),b,c)


#define F_M_RMDIR	(SC_FILE * 256) + 18
#define m_rmdir(a,b,c)		     \
	c_service(F_M_RMDIR,(void far *)(a),b,c)


#define F_M_GETDRV	(SC_FILE * 256) + 19
#define m_getdrv(a)		  \
	c_service(F_M_GETDRV,(void far *)(a))


#define F_M_SETDRV	(SC_FILE * 256) + 20
#define m_setdrv(a)		  \
	c_service(F_M_SETDRV,a)


#define F_M_FDATE	(SC_FILE * 256) + 21
#define m_fdate(a,b)		   \
	c_service(F_M_FDATE,(void far *)(a),(void far *)(b))


#define F_M_GET_SYSDIR	(SC_FILE * 256) + 22
#define m_get_sysdir(a) 	      \
	c_service(F_M_GET_SYSDIR,(void far *)(a))


#define F_M_GETATTR	(SC_FILE * 256) + 23
#define m_getattr(a,b,c,d)		 \
	c_service(F_M_GETATTR,(void far *)(a),b,c,(void far *)(d))


#define F_M_SETATTR	(SC_FILE * 256) + 24
#define m_setattr(a,b,c,d)		 \
	c_service(F_M_SETATTR,(void far *)(a),b,c,d)


#define F_M_COMMON_OPEN	(SC_FILE * 256) + 25
#define m_common_open(a,b,c,d,e,f,g)		  \
	c_service(F_M_COMMON_OPEN,(void far *)(a),(void far *)(b),c,d,e,f,g)

#define F_M_COPYDT      (SC_FILE * 256) + 26
#define m_copydt(a,b)   \
        c_service(F_M_COPYDT,(void far *)(a),(void far *)(b))


#define F_M_GETFDT      (SC_FILE * 256) + 27
#define m_getfdt(a,b)   \
        c_service(F_M_GETFDT,(void far *)(a),(void far *)(b))


#define F_M_PUTFDT      (SC_FILE * 256) + 28
#define m_putfdt(a,b)   \
        c_service(F_M_PUTFDT,(void far *)(a),b)




/*SC_PM*/


#define F_M_INIT	(SC_PM * 256) + 0
#define m_init()	c_service(F_M_INIT)

#define F_M_FINI	(SC_PM * 256) + 1
#define m_fini()	c_service(F_M_FINI)

#define F_M_LOCK	(SC_PM * 256) + 2
#define m_lock()	c_service(F_M_LOCK)

#define F_M_UNLOCK	(SC_PM * 256) + 3
#define m_unlock()	c_service(F_M_UNLOCK)

#define F_M_SPAWN	(SC_PM * 256) + 4
#define m_spawn(a,b,c,d)	\
	c_service(F_M_SPAWN,(void far *)(a),b,c, (void far *)(d))

#define F_M_APPCOUNT    (SC_PM * 256) + 5
#define m_appcount()    c_service(F_M_APPCOUNT)

#define F_M_REBOOT    	(SC_PM * 256) + 6
#define m_reboot()      c_service(F_M_REBOOT)

#define F_M_SPAWNARG	(SC_PM * 256) + 7
#define m_spawnarg(a,b,c,d)	\
	c_service(F_M_SPAWNARG,(void far *)(a),b,(void far *)(c),d)

#define F_M_REG_APP_NAME (SC_PM * 256) + 8
#define m_reg_app_name(a)	\
	c_service(F_M_REG_APP_NAME,(void far *)(a))

#define  F_APP_NAME (SC_PM * 256) + 9
#define app_name(a)	\
	c_service_far_ptr(F_APP_NAME,(void far *)(a))



/*SC_CB*/


#define F_M_OPEN_CB	(SC_CB * 256) + 0
#define m_open_cb()		\
	c_service(F_M_OPEN_CB)

#define F_M_CLOSE_CB	(SC_CB * 256) + 1
#define m_close_cb()		\
	c_service(F_M_CLOSE_CB)

#define F_M_RESET_CB	(SC_CB * 256) + 2
#define m_reset_cb(a)		\
	c_service(F_M_RESET_CB,(void far *)(a))

/* (SC_CB * 256) + 3 is reserved */

#define F_M_NEW_REP	(SC_CB * 256) + 4
#define m_new_rep(a)		 \
	c_service(F_M_NEW_REP,(void far *)(a))

#define F_M_FINI_REP	(SC_CB * 256) + 5
#define m_fini_rep()		\
	c_service(F_M_FINI_REP)

#define F_M_REP_NAME	(SC_CB * 256) + 6
#define m_rep_name(a,b,c)	\
	c_service(F_M_REP+NAME,a,(void far *)(b),(void far *)(c))

#define F_M_REP_INDEX	(SC_CB * 256) + 7
#define m_rep_index(a,b,c)	\
	c_service(F_M_REP_INDEX,(void far *)(a),(void far *)(b),(void far *)(c))

#define F_M_CB_WRITE	(SC_CB * 256) + 8
#define m_cb_write(a,b) 	\
	c_service(F_M_CB_WRITE,(void far *)(a),b)

#define F_M_CB_READ	(SC_CB * 256) + 9
#define m_cb_read(a,b,c,d)	\
	c_service(F_M_CB_READ,a,b,(void far *)(c),d)



/*SC_SOUND*/


#define F_M_BEEP	(SC_SOUND * 256) + 0
#define m_beep()	c_service(F_M_BEEP)

#define F_M_THUD	(SC_SOUND * 256) + 1
#define m_thud()	c_service(F_M_THUD)

#define F_M_ASOUND	(SC_SOUND * 256) + 2
#define m_asound(a)	c_service(F_M_ASOUND,a)

#define F_M_SOUNDOFF	(SC_SOUND * 256) + 3
#define m_soundoff()	c_service(F_M_SOUNDOFF)

/*SC_MM*/


#define F_M_ALLOC	(SC_MM * 256) + 0
#define m_alloc(a)		\
	c_service_near_ptr(F_M_ALLOC,a)

/* (SC_MM * 256) + 1 is reserved */

#define F_M_FREE	(SC_MM * 256) + 2
#define m_free(a)	     \
	c_service(F_M_FREE,a)
	
#define F_M_ALLOC_LARGE	(SC_MM * 256) + 3
#define m_alloc_large(a)	\
	c_service_near_ptr(F_M_ALLOC_LARGE,a)

#define F_M_FREE_LARGE	(SC_MM * 256) + 4
#define m_free_large(a)		\
	c_service_near_ptr(F_M_FREE_LARGE,a)

/*SC_BG*/



/*SC_DTIME*/


#define F_M_DTINFO	(SC_DTIME * 256) + 0
#define m_dtinfo(a)		\
	c_service(F_M_DTINFO,(void far *)(a))

#define F_M_GETDTM	(SC_DTIME * 256) + 1
#define m_getdtm(a)	      \
	c_service(F_M_GETDTM,(void far *)(a))

#define F_M_SETDTM	(SC_DTIME * 256) + 2
#define m_setdtm(a)		\
	c_service(F_M_SETDTM,(void far *)(a))

#define F_M_XALARM	(SC_DTIME * 256) + 3
#define m_xalarm(a)	 \
	c_service(F_M_XALARM,a)

#define F_M_ALARM	(SC_DTIME * 256) + 4
#define m_alarm(a,b)		\
	c_service(F_M_ALARM,(void far *)(a),b)

#define F_M_START_SW	(SC_DTIME * 256) + 5
#define m_start_sw(a)	   \
	c_service(F_M_START_SW,(void far *)(a))

#define F_M_GET_SW	(SC_DTIME * 256) + 6
#define m_get_sw(a,b,c)	    \
	c_service(F_M_GET_SW,(void far *)(a),(void far *)(b),(void far *)(c))

#define F_M_STOP_SW	(SC_DTIME * 256) + 7
#define m_stop_sw(a)	       \
	c_service(F_M_STOP_SW,(void far *)(a))

#define F_M_TELLTIME	 (SC_DTIME * 256) + 8
#define m_telltime(a,b,c)      \
	c_service(F_M_TELLTIME,a,b,c)

#define F_M_GET_SETTINGS (SC_DTIME *256) +9
#define m_get_settings(a,b)      \
	c_service(F_M_GET_SETTINGS, (void far *)(a),(void far *)(b))

#define F_M_SET_SETTINGS (SC_DTIME *256) +10
#define m_set_settings(a,b)      \
	c_service(F_M_SET_SETTINGS, (void far *)(a),(void far *)(b))

#define F_M_START_TIMER	(SC_DTIME * 256) + 11
#define m_start_timer(a)	   \
	c_service(F_M_START_TIMER,(void far *)(a))

#define F_M_STOP_TIMER	(SC_DTIME * 256) + 12
#define m_stop_timer(a)	       \
	c_service(F_M_STOP_TIMER, (void far *)(a))

#define F_M_GET_TIMER	(SC_DTIME * 256) + 13
#define m_get_timer(a,b,c)    \
	c_service(F_M_GET_TIMER,(void far *)(a),(void far *)(b),(void far *)(c))


#define F_M_TELL_ANYTIME   (SC_DTIME * 256) + 14
#define m_tell_anytime(a,b,c,d,e)      \
	c_service_far_ptr(F_M_TELL_ANYTIME,a,b,c, (void far *)(d), (void far *)(e))


#define F_M_GET_SETTINGS_ADDR (SC_DTIME *256) +15
#define m_get_settings_addr()      \
	c_service_far_ptr(F_M_GET_SETTINGS_ADDR)

#define F_M_PARSE_DATE (SC_DTIME * 256) + 16
#define m_parse_date(a,b,c)      \
	c_service(F_M_PARSE_DATE,a,(void far *)(b),(void far *)(c))

#define F_M_PARSE_TIME (SC_DTIME * 256) + 17
#define m_parse_time(a,b,c)      \
	c_service(F_M_PARSE_TIME,a,(void far *)(b),(void far *)(c))


#define F_M_SET_DATERULE (SC_DTIME * 256) + 18
#define m_set_daterule(a)      \
        c_service(F_M_SET_DATERULE,a)

#define F_M_SET_TIMERULE (SC_DTIME * 256) + 19
#define m_set_timerule(a)      \
        c_service(F_M_SET_TIMERULE,a)

#define F_M_POSTTIME (SC_DTIME * 256) + 20
#define m_posttime()      \
        c_service(F_M_POSTTIME)

#define F_M_DAY_TRIGGER (SC_DTIME * 256) + 21
#define m_day_trigger(a)	\
	c_service(F_M_DAY_TRIGGER,a);





/*SC_PRINTER */

#define F_M_OPEN_PRINTER	(SC_PRINTER * 256) + 0
#define m_open_printer()	\
	c_service(F_M_OPEN_PRINTER)


#define F_M_CLOSE_PRINTER	(SC_PRINTER * 256) + 1
#define m_close_printer()	\
	c_service(F_M_CLOSE_PRINTER)

#define F_M_WRITE_PRINTER	(SC_PRINTER * 256) + 2
#define m_write_printer(a,b)	\
	c_service(F_M_WRITE_PRINTER,(void far *)(a),b)

#define F_M_INIT_PRINTER	(SC_PRINTER * 256) + 3
#define m_init_printer()	\
	c_service(F_M_INIT_PRINTER)

#define F_M_TRANS_PRINTER	(SC_PRINTER * 256) + 4
#define m_trans_printer(a,b)	\
	c_service(F_M_TRANS_PRINTER,a,(void far *)(b))

#define F_M_FALL_PRINTER	(SC_PRINTER * 256) + 5
#define m_fall_printer(a,b)	\
	c_service(F_M_FALL_PRINTER,a,(void far *)(b))



/*SC_CONFIG*/






/*SC_COMM*/

#define F_COMM_INIT	(SC_COMM * 256) + 0
#define InitCom(a)	\
	c_service(F_COMM_INIT,(void far *)(a))

#define F_COMM_OPEN	(SC_COMM * 256) + 1
#define ComOpen(a,b)	\
	c_service(F_COMM_OPEN,(int far *) (a), (int)(b))

#define F_COMM_CLOSE	(SC_COMM * 256) + 2
#define ComClose(a)	\
	c_service(F_COMM_CLOSE,(int) (a))

#define F_COMM_GETMDM	(SC_COMM * 256) + 3
#define ComGetModem(a)	\
	c_service(F_COMM_GETMDM, (int)(a))

#define	F_COMM_ANSWER	(SC_COMM * 256) + 4
#define	ComAnswer(a,b)	\
	c_service(F_COMM_ANSWER, (int) a, (int) b)

#define	F_COMM_DIAL	(SC_COMM * 256) + 5
#define	ComDial(a,b)	\
	c_service(F_COMM_DIAL, (int) (a), (char far *) (b))

#define	F_COMM_RESET	(SC_COMM * 256) + 6
#define	ComReset(a,b)	\
	c_service(F_COMM_RESET, (int) (a), (int) (b))

#define	F_COMM_HANGUP	(SC_COMM * 256) + 7
#define	ComHangUp(a)	\
	c_service(F_COMM_HANGUP, (int) (a))

#define	F_COMM_SNDBTS	(SC_COMM * 256) + 8
#define	ComSendBytes(a,b,c,d)	\
	c_service(F_COMM_SNDBTS, (int) (a), (char far *) (b), (int) (c), (int far *) (d))

#define	F_COMM_QRYTXQ	(SC_COMM * 256) + 9
#define	ComQryTxQue(a,b,c)	\
	c_service(F_COMM_QRYTXQ, (int) (a), (int far *) (b), (int far *) (c))

#define	F_COMM_QRYRXQ	(SC_COMM * 256) + 10
#define	ComQryRxQue(a,b,c)	\
	c_service(F_COMM_QRYRXQ, (int) (a), (int far *) (b), (int far *) (c))

#define	F_COMM_RCVBTS	(SC_COMM * 256) + 11
#define	ComReceiveBytes(a,b,c)	\
	c_service(F_COMM_RCVBTS, (int) (a), (char far *) (b), (int far *) (c))

#define	F_COMM_HAZCMD	(SC_COMM * 256) + 12
#define	ComHayesCommand(a,b,c)	\
	c_service(F_COMM_HAZCMD, (int) (a), (char far *) (b), (int) (c))

#define	F_COMM_COMAND	(SC_COMM * 256) + 13
#define	ComCommand(a,b,c)	\
	c_service(F_COMM_COMAND, (int) (a), (char far *) (b), (int) (c))

#define	F_COMM_BREAK	(SC_COMM * 256) + 14
#define	ComBreak(a,b)		\
	c_service(F_COMM_BREAK, (int) (a), (int) (b))

#define	F_COMM_FRCXON	(SC_COMM * 256) + 15
#define	ComForceXon(a)		\
	c_service(F_COMM_FRCXON, (int) (a))

#define	F_COMM_FRCXOF	(SC_COMM * 256) + 16
#define	ComForceXoff(a)		\
	c_service(F_COMM_FRCXOF, (int) (a))

#define	F_COMM_SETDTR	(SC_COMM * 256) + 17
#define	ComSetDtr(a,b)		\
	c_service(F_COMM_SETDTR, (int) (a), (int) (b))

#define	F_COMM_XMITNG	(SC_COMM * 256) + 18
#define	ComXmitting(a)		\
	c_service(F_COMM_XMITNG, (int) (a))

#define	F_COMM_STATUS	(SC_COMM * 256) + 19
#define	ComStatus(a)		\
	c_service(F_COMM_STATUS, (int) (a))

#define	F_COMM_SET	(SC_COMM * 256) + 20
#define	ComSet(a,b)	\
	c_service(F_COMM_SET, (int) (a), (com_settings far *) (b))

#define	F_COMM_GET	(SC_COMM * 256) + 21
#define	ComGet(a,b)	\
	c_service(F_COMM_GET, (int) (a), (com_settings far *) (b))

#define	F_COMM_CNFGUR	(SC_COMM * 256) + 22
#define	ComConfigure(a,b,c,d,e)	\
	c_service(F_COMM_CNFGUR, (int) (a), (int) (b), (int) (c), (int) (d), (int) (e))

#define F_COMM_QRYERR	(SC_COMM * 256) +23
#define ComQryErr(a)		\
	c_service(F_COMM_QRYERR, (int) a)




/*SC_MISC*/

#define F_M_ERRMSG	(SC_MISC * 256) + 0
#define m_errmsg(a,b,c,d)		\
	c_service(F_M_ERRMSG,a,(void far *)(b),c,(void far *)(d))

#define F_DRAWBOX	(SC_MISC * 256) + 1
#define drawbox(a)		 \
	c_service(F_DRAWBOX,(void far *)(a))

#define F_SHOWNAME	(SC_MISC * 256) + 2
#define showname(a)		 \
	c_service(F_SHOWNAME,(void far *)(a))

#define F_MESSAGE	(SC_MISC * 256) + 3
#define message(a,b,c,d)	       \
	c_service(F_MESSAGE,(void far *)(a),b,(void far *)(c),d)

#define F_MSG_OFF	(SC_MISC * 256) + 4
#define msg_off()		\
	c_service(F_MSG_OFF)

#define F_M_COM_TIMER_ADDR  (SC_MISC * 256) + 5
#define com_timer_addr()		  \
	c_service_far_ptr(F_M_COM_TIMER_ADDR)

#define F_M_COM_TIMER_COUNT_ADDR  (SC_MISC * 256) + 6
#define com_timer_count_addr()		  \
	c_service_far_ptr(F_M_COM_TIMER_COUNT_ADDR)


#define F_M_SYS_RSRC_ADDR (SC_MISC * 256) + 7
#define m_sys_rsrc_addr()		\
	c_service_far_ptr(F_M_SYS_RSRC_ADDR)


/* SC_MISC * 8 is reserved */

#define F_MESSAGE3      (SC_MISC * 256) + 9
#define message3(a,b,c,d,e,f)          \
        c_service(F_MESSAGE3,(void far *)(a),b,(void far *)(c),d,(void far *)(e),f)


#define F_M_DISABLE_MACROS (SC_MISC * 256) + 10
#define m_disable_macros()              \
        c_service(F_M_DISABLE_MACROS)


#define F_M_ENABLE_MACROS (SC_MISC * 256) + 11
#define m_enable_macros()               \
        c_service(F_M_ENABLE_MACROS)

#define F_M_FORM_FT (SC_MISC * 256) + 13
#define m_form_ft(a)		\
	c_service_far_ptr(F_M_FORM_FT,(void far *)(a))

#define F_M_RAM_IV_INFO (SC_MISC * 256) + 14
#define m_ram_iv_info(a)		\
	c_service_far_ptr(F_M_RAM_IV_INFO,(void far *)(a))



/*SC_VIDVIR*/

#define	F_M_DIRTY_SYNC		((SC_VIDVIR * 256) + 5)
#define	m_dirty_sync()			\
   c_service(F_M_DIRTY_SYNC)

/*SC_VIDPHY*/



/*SC_RSRC*/

#define F_MAP_RESOURCE_FILE   (SC_RSRC * 256) + 0
#define map_resource_file(a)             \
        c_service(F_MAP_RESOURCE_FILE,(void far *)(a))


#define F_GET_RESOURCE_PTR   (SC_RSRC * 256) + 1
#define get_resource_ptr(a)              \
        c_service_far_ptr(F_GET_RESOURCE_PTR,a)

#define F_GET_RSRC_TAB_PTR   (SC_RSRC * 256) + 2
#define get_rsrc_tab_ptr()	\
        c_service_far_ptr(F_GET_RSRC_TAB_PTR)

#define F_INIT_SYSGMR_RSRCS	(SC_RSRC * 256) + 3
#define init_sysmgr_resources()	\
	c_service(F_INIT_SYSGMR_RSRCS)




/*SC_HELP*/


#define F_M_HELP_INIT      (SC_HELP * 256) + 0
#define m_help_init(a,b,c)		  \
	c_service(F_M_HELP_INIT,(void far *)(a),(void far *)(b),c)

#define F_M_HELP_DISPLAY      (SC_HELP * 256) + 1
#define m_help_display(a)		  \
	c_service(F_M_HELP_DISPLAY,(void far *)(a))

#define F_M_HELP_KEY      (SC_HELP * 256) + 2
#define m_help_key(a,b)		  \
	c_service(F_M_HELP_KEY,(void far *)(a),b)

#define F_M_HELP_TERM      (SC_HELP * 256) + 3
#define m_help_term(a)		  \
	c_service(F_M_HELP_TERM,(void far *)(a))


/*SC_COL*/

#define	F_M_ColInit			(SC_COL * 256) + 0
#define	m_col_init()				\
	c_service(F_M_ColInit)

#define	F_M_ColCpStr		(SC_COL * 256) + 1
#define	m_col_cpstr(a,b,c,d)	\
	c_service(F_M_ColCpStr,(void far *)(a),b,(void far *)(c),d)

#define	F_M_ColLicsStr		(SC_COL * 256) + 2

#define	F_M_ColLicsChar	(SC_COL * 256) + 3

#define	F_M_ColToLower		(SC_COL * 256) + 4
#define	m_col_tolower(a,b)		\
	c_service(F_M_ColToLower,(void far *)(a),b)

#define	F_M_ColCpSearch	(SC_COL * 256) + 5
#define	m_col_cpsearch(a,b,c,d,e)	\
	c_service(F_M_ColCpSearch,(void far *)(a),b,(void far *)(c),d,e)

#define	F_M_ColToUpper		(SC_COL * 256) + 6
#define	m_col_toupper(a,b)		\
	c_service(F_M_ColToUpper,(void far *)(a),b)

