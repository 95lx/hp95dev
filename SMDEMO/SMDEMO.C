/*
 * SMDEMO.C - Example of Menu and File Menu Services.
*/

/* System Manager header files */

#include "..\headers\interfac.h"
#include "..\headers\event.h"
#include "..\headers\fmenu.h"
#include "..\headers\menu.h"
#include "..\headers\edit.h"
#include "..\headers\edit.h"
#include "..\headers\smtime.h"

/* C standard header files */

#include <stdlib.h>

/* defines */

typedef int BOOL;
#define	TRUE	1
#define	FALSE	0
enum { NORM, INV };
enum states { MAIN, MENU, FSEL, COUNT, DONE };

enum strings { TITLEs, MENUs, MAINkey1s, MAINkey2s,
                    HELPkey1s, HELPkey2s, ESCs, FSELtitles, _last_string };
#define NSTRINGS _last_string
/* s(x) below converts a string number to pointer to the string. */
#define s(x) strptr[x]

/* Menu states */
enum { File, Count, Quit, _last_menu };
#define N_MENU  _last_menu

/* Key codes returned by System manager */
#define MENUKEY  0xC800
#define TMENUKEY 0x8500		/* F11 key which acts like menu under tkernel */
#define F1  0x3B00
#define ESC  27

/* File-menu defines */
#define NFILES 50
#define FM_BUF_SIZE ( NFILES * (sizeof (FILEINFO) ) )

#define MAX_PERMS 64

/* function prototypes */
void app_init(void);
void app_term(void);
void app_awake(void);
void app_sleep(void);
void app_break(void);
int app_key(void);
void app_display(void);
void ESCmsg( char * msg );
void disable_light_sleep(void);
void enable_light_sleep(void);
void InterruptibleLoop(BOOL (*RepeatingFunction)(void));
void ResetPermutations(void);
BOOL Permutation(void);
 

/* global variables */

EVENT event;
FILEINFO fmbuffer[NFILES];
MENUDATA menu;
EDITDATA edit_data;
enum states state = MAIN;
char * strptr[ NSTRINGS ];

FMENU fmenu = { (char far *) "C:\\_DAT",
					 (char far *) "*.TXT",
					 ( FILEINFO far * ) fmbuffer,
					FM_BUF_SIZE, -2, 0, 13,	40, 3
					};

/* Variables for computing permutations */
int Perm[MAX_PERMS];
int PermSpots;
long int PermCount;

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void main(void)
{
	m_init();		/* init call to system manager */
	app_init();		/* application initialization */
						/* event loop */
	do	{
		m_event(&event);	  		/* get next event */
		switch (event.kind) {
			case E_ACTIV:
				app_awake();		/* reactivate app */
				break;
			case E_DEACT:
				app_sleep();		/* prepare for suspension */
				break;
			case E_TERM:
				state = DONE; 		/* being terminated */
				break;
			case E_BREAK:
				app_break();		/* app ctrl-break handler */
				break;
			case E_KEY:
				app_key();	 		/* process key */
				break;
			}
		}
	while ( state != DONE );

	app_term();		/* application termination */
	m_fini();		/* terminate call to system manager, never returns */
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_init(void)
/*
 * Application specific initialization.
 *
 * Initialize text strings.  This routine could be changed later to read
 * the strings from a file.  This would be required, for example, to
 * provide for localization to different languages.
*/
{
	int i;
	char * sp[ NSTRINGS ] = {
		"SM Demo",
        "File\0Permutations\0Quit",
		"                                        ",
		"Help                                    ",
		"                                        ",
		"Help                                    ",
  		"Press ESC to continue",
  		"File to use:"
  		};

	/* templates for function keys */
	/*
		"   ......  ......  ......  ......  ....."
		".....  ......  ......  ......  ......   "
	*/

	for( i = 0; i < NSTRINGS; i++ )
		strptr[ i ] = sp[ i ];
	
	app_display();
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_term(void)
/*
 * Terminate application
*/
{
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_awake(void)
/*
 * Reactivates application after suspension.
*/
{
	app_display();
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_display(void)
/*
 * Application displayer.
*/
{
	switch( state ) {
		case MAIN:
  			drawbox( s( TITLEs ) );
  			m_telltime( DATE_N_TIME, -3, 19 );
  			m_disp( 11, 0, s( MAINkey1s ), 40, INV, 0 );
  			m_disp( 12, 0, s( MAINkey2s ), 40, INV, 0 );
  			m_setcur( 0, -1 );
  			break;
		
		case MENU:
  			drawbox( "" );
  			menu_dis( &menu );
  			m_disp( 11, 0, s( HELPkey1s ), 40, INV, 0 );
  			m_disp( 12, 0, s( HELPkey2s ), 40, INV, 0 );
  			break;

		case FSEL:
  			fmenu_dis( &fmenu, &edit_data );
  			m_disp( 11, 0, s( HELPkey1s ), 40, INV, 0 );
  			m_disp( 12, 0, s( HELPkey2s ), 40, INV, 0 );
  			break;

        case COUNT:
            break;
		
		} /* end switch on state */
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_sleep(void)
/*
 * Prepares application for suspension.
 */
{
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_break(void)
/*
 * Application control-break handler.
 */
{
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
int app_key(void)
/*
 * Application keystroke processor.
*/
{
	int menuselect;
	char txtpattern[] = "*.txt";

	switch( state ) {
  		
		case MAIN:
    		switch( event.data ) {
      		case MENUKEY:
			 	case TMENUKEY:			/* menu key under tkernel */
        			state = MENU;
        			menu_setup( &menu, s( MENUs ), N_MENU, 1, NULL, 0, NULL );
        			menu_on( &menu );
        			app_display();
        			break;
      	  	case F1:  // Help
        			ESCmsg( "Help subsystem would be entered" );
        			break;
      		default:
					m_beep();
      		}
			break;

  		case MENU:
    		if( event.data == ESC ) {
      		state = MAIN;
      		menu_off( &menu );
      		app_display();
      		}
    		else if( event.data == F1 )
      		ESCmsg( "Help subsystem would be entered" );
    		else {
      		menu_key( &menu, event.data, &menuselect );
      		if ( menuselect > -1 )
					menu_off( &menu );
      		switch( menuselect ) {
        			case File:
          			state = FSEL;
          			fmenu.fm_pattern = txtpattern;
						edit_data.prompt_window = 1;
						edit_data.prompt_line_length = 0;
						edit_data.message_line = s( FSELtitles );
						edit_data.message_line_length = strlen( s( FSELtitles ) );
          			fmenu_init( &fmenu, &edit_data, "*.txt", 9, 1 );
          			app_display();
          			break;

                    case Count:
                    ResetPermutations();
                    InterruptibleLoop(Permutation);
                    state=MAIN;
                    app_display();
                    break;

        			case Quit:
          			state = DONE;
          			break;
        			}
      		}
    		break;

  		case FSEL:
    		if( event.data == F1 )
      		ESCmsg( "Help subsystem would be entered" );
    		else
      		switch( fmenu_key( &fmenu, &edit_data, event.data ) ) {
        			case RET_OK:
						break;
        			case RET_ACCEPT:
						fmenu_off( &fmenu, &edit_data );
						state = MAIN;
						app_display();
						m_disp(5,5,edit_data.edit_buffer,
							strlen(edit_data.edit_buffer),0,0);
						break;
        			case RET_ABORT:
          			fmenu_off( &fmenu, &edit_data );
          			state = MAIN;
          			app_display();
          			break;
        			default:
						m_beep();
        		}
    		break;

		} /* end switch on state */
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ESCmsg( char * msg )
/*
 * Display a message which requires ESC to continue.
 * Lock the user into responding to this message with the m_lock()
 * call which will prevent task-switches.
 */
{
	message( msg, strlen( msg ), s( ESCs ), strlen( s( ESCs ) ) );
	m_lock();
	while (1) {
		m_event( &event );
  		if ( event.kind == E_KEY )	{
			if ( event.data == ESC)
				break;
			else
				m_beep();
			}
		}
	m_unlock();
	msg_off();
}
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void InterruptibleLoop(BOOL (*RepeatingFunction)(void))
/*
 * The only special note to be taken of this process, is that it 
 * continously calls RepeatingFunction AND allows the user to task-switch 
 * to other System Manager applications (also allowing alarms to function).
 * RepeatingFunction should return a TRUE when it is finished, else a FALSE.
 * The equivalent loop above (ESCmsg) cannot task-switch because
 * of the m_lock() call.
 * NOTE:  The calls to disable_light_sleep() and enable_light_sleep()
 *        MUST BE PAIRED WITHIN THIS PROGRAM.  Special precautions are 
 *        taken to prevent a task-switch while light sleep is disabled.
 *        A switch away while light sleep is disabled can severly limit
 *        battery life.
 */
{
    BOOL Done=FALSE;

    disable_light_sleep();   /* Allow process to run at full speed */

    while (!Done) {
        Done = (*RepeatingFunction)();
        m_nevent( &event );     /* Look at next event (to avoid .5 sec timeout) */
        switch (event.kind) {
            case E_BREAK:       /* Allow a break key to terminate */
                Done = TRUE;
                break;
            case E_KEY:
					 m_event( &event );
                if (event.data==ESC)  Done=TRUE;  /* Or an escape key to terminate */
                break;
            case E_ACTIV:
                app_display();
                disable_light_sleep();  /* Coming back from switch--disable l.s. */
                break;
            case E_DEACT:
                enable_light_sleep();   /* Switching away--re-enable l.s. */
                break;
            }
        }
   
    enable_light_sleep();   /* Allow CPU to save as much battery life as possible */
}
/*-----------------------------------------------------------------------*/
void disable_light_sleep(void)
/*
 * Use 95's special BIOS routine to disable light sleep
 */
{
_asm {
     mov ax,4e00h
     int 15h
     }
}
/*-----------------------------------------------------------------------*/
void enable_light_sleep(void)
/*
 * Use 95's BIOS call
 */
{
_asm {
     mov ax,4e01h
     int 15h
     }
}


/*-----------------------------------------------------------------------*/
void ResetPermutations()
/*
 * Start at very first permutation 
 */
{
  int i;

  PermSpots = strlen(edit_data.edit_buffer);
  for (i=0; i<PermSpots; i++)  Perm[i] = i;
  PermCount = 0;    
}
/*-----------------------------------------------------------------------*/
void ShowPerm(void)
/*
 * Diplay current perm. count and current perm.
 */
{
  int i;
  char Buffer[MAX_PERMS];

  ltoa(PermCount,Buffer,10);
  m_disp(3,5,Buffer,strlen(Buffer),0,0);

  for (i=0; i<PermSpots; i++)  Buffer[i] = edit_data.edit_buffer[Perm[i]];
  Buffer[PermSpots] = 0;

  m_disp(5,5,Buffer,strlen(Buffer),0,0);    /* Show current perm */
  m_dirty_sync();                           /* And ensure it is displayed */
}
/*-----------------------------------------------------------------------*/
void NextAvailable(int level)
/*
 * For the level-th character, find the next available (ie. not already used)
 * character beyond the current one.
 */
{
  int i;
  BOOL AlreadyHit[MAX_PERMS];

  for (i=0; i<MAX_PERMS; i++)  AlreadyHit[i] = FALSE;   /* Zero out array */

  for (i=0; i<level; i++)  AlreadyHit[Perm[i]] = TRUE;  /* Set previous perms */

  for (i=Perm[level]+1; i<PermSpots; i++)  if (!AlreadyHit[i])  {  /* Look for available perm. following current one */
    Perm[level] = i;
    return;
    }

  Perm[level] = -1;   /* Indicate no more possibilities for this position */
}
/*-----------------------------------------------------------------------*/
BOOL Permutation(void)
/*
 * Advance current perm to the next perm.
 * Return TRUE if done, otherwise FALSE.
 */
{ 
  int PermLevel=PermSpots-1;   /* Start looking at last position */
  int i;
  
  PermCount++;                 /* Count perms. */
  ShowPerm();                  /* and show last one */

  NextAvailable(PermLevel);

  while (PermLevel>0 && Perm[PermLevel]==-1) {     /* If current level has rolled off end */
    PermLevel--;                /* Decrease depth and */
    NextAvailable(PermLevel);   /* Bump up index */
    for (i=PermLevel+1; i<PermSpots; i++)  Perm[i] = 0;  /* Zap all to right */
    }
  if (PermLevel<0)  return TRUE;            /* Done. */

  /* Fill up all to right of scrolled spot with available chars */
  for (i=PermLevel+1; i<PermSpots; i++)  NextAvailable(i);   

  return FALSE;
} 
