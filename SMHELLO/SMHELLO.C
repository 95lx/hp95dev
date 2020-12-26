/*
 * SMHELLO.C - Small example of a System Manager compliant program.
*/

#define	TRUE	1
#define	FALSE	0

#include "..\headers\interfac.h"
#include "..\headers\event.h"

/* function prototypes */

void app_init(void);
void app_term(void);
void app_awake(void);
void app_sleep(void);
void app_break(void);
int app_key(void);
void app_display(char *msg);

/* global variables */

EVENT appevent;

/*-----------------------------------------------------------------------*/
void main(void)
{
	int done = FALSE;

	m_init();		/* init call to system manager */
	app_init();		/* application initialization */

						/* event loop */
	do	{
		m_event(&appevent);	  					/* get next event */
		switch (appevent.kind) {
			case E_ACTIV:
				app_awake();						/* reactivate app */
				break;
			case E_DEACT:
				app_sleep();						/* prepare for suspension */
				break;
			case E_TERM:
				done = TRUE; 						/* being terminated */
				break;
			case E_BREAK:
				app_break();						/* app ctrl-break handler */
				break;
			case E_KEY:
				done = app_key();   		/* process key */
				break;
			}
		}
	while (!done);

	app_term();		/* application termination */
	m_fini();		/* terminate call to system manager, never returns */
}
/*-----------------------------------------------------------------------*/
void app_init(void)
/*
 * Initialize application
 */
{
	app_display("app_init");
}
/*-----------------------------------------------------------------------*/
void app_term(void)
/*
 * Terminate application
 */
{
}
/*-----------------------------------------------------------------------*/
void app_awake(void)
/*
 * Reactivates application after suspension.
 */
{
	app_display("app_awake");
}
/*-----------------------------------------------------------------------*/
void app_display(char *msg)
{
	char usage_str[] = "press q to exit ...";
	char sysdir[64];

	m_setmode(1);			/* set text mode */
	drawbox("SM Hello");

	m_disp(1,5,msg,strlen(msg),0,0);
	m_disp(3,5,usage_str,strlen(usage_str),1,0);
}
/*-----------------------------------------------------------------------*/
void app_sleep(void)
/*
 * Prepares application for suspension.
 */
{
}
/*-----------------------------------------------------------------------*/
void app_break(void)
/*
 * Application control-break handler.
 */
{
}
/*-----------------------------------------------------------------------*/
int app_key(void)
/*
 * Application keystroke processor.
 *
 * Returns TRUE if user has requested termination, else returns FALSE.
 */
{
	if (appevent.data == 'q') {
		return TRUE;
		}
	else {
		m_beep();				/* signal error */
		return FALSE;
		}
}
/*-----------------------------------------------------------------------*/
