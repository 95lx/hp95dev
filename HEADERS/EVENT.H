/*
 * Module event.h  Version 1.1  Date 06/19/91  Time 17:17:31
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

#ifndef M_EVENT


#define LO_BYTE		0x00ff
#define FLAG_SCAN	0xE800

enum event_kind {
	E_NONE,		/* no events available */
	E_KEY,		/* keystroke available */
	E_BREAK,	/* CNTRL_BREAK encountered */
	E_ACTIV,	/* reactivation event, always follows E_DEACT */
	E_DEACT,	/* about to sleep, next event request is delayed */
	E_TERM,		/* forced closure of application */
	E_BRIDGE,	/* 123 bridge service request, only ret. to 123 */
	E_GROW,	 	/* request to 123 to grow */
	E_SHRINK,	/* request to 123 to shrink */
	E_ALARM_EXP,	/* application's alarm has expired */
	E_ALARM_DAY,	/* daily chance to set an alarm */
	E_TIMECHANGE	/* system date or time has been changed */
};



typedef struct {
	enum event_kind kind;
	unsigned int data;	/* ASCII (CP 850) translation of keystroke */
	unsigned char scan;	/* scan code from BIOS */
	unsigned char shifts;	/* shifts register, when function returns */
				/* not necessarily when key struck */
	unsigned int lics;	/* LICS translation of keystroke */
	unsigned char fkey_num;	/* function key number for 123 only */
	void far * bridge;	/* pointer to 123 bridge data structure */
} EVENT;





/* Field usages by event_kind 

	TYPE		FIELDS USED
	-----------------------------------------------	
	E_NONE		shifts
	E_KEY		data,scan,fkey_num,lics,shifts
	E_BREAK		none
	E_ACTIV		none
	E_DEACT		none
	E_TERM		none
	E_BRIDGE	bridge
	E_GROW		data
	E_SHRINK	data
	E_ALARM_EXP	data,bridge
	E_ALARM_DAY	none
	E_TIMECHANGE	bridge (-> timechange structure )
 */
	

#define M_EVENT
#endif

