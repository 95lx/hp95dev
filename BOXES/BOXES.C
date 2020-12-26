/*
 * BOXES.C - Example of writing a HP95LX System Manager compliant application.
 *
 * 8-30-91  A.J.G.
 *
 *
 * -- To attach this program to the Control-Memo key, add the following
 *    line to the file C:\_DAT\APNAME.LST
 *
 *    C:\BOXES.EXM,BE00,BOXES
 *
 *    The first parameter is the system manager file that is executed,
 *    the second is the scan code of the corresponding key, and the
 *    third is the name that is used by the System Manager when prompting
 *    the user during app shutdown.
 */

/* Include System Manager header files */

#include "..\headers\interfac.h"
#include "..\headers\event.h"
#include "..\headers\fmenu.h"
#include "..\headers\menu.h"
#include "..\headers\fileio.h"
#include "..\headers\m_error.h"
#include "..\headers\edit.h"
#include "..\headers\smtime.h"
#include "..\headers\settings.h"
#include "..\headers\comio.h"

/* C standard header files */

#include <stdlib.h>
#include <ctype.h>


/**************************** Defines ******************************/

typedef int BOOL;
#define    TRUE    1
#define    FALSE    0

enum screen_attributes { NORM, INV };   /* Attributes as used by m_disp(...) */


/*** Program States ***
 * The states below are used to facilitate writing an event-driven program.
 * The program doles out responsibilites depending on the current mode.  
 *
 * All keystrokes are handled respective to the program state (app_key),
 * as well as the display generation (app_display).
 *
 * If the user switches to a different task, we will still know the proper 
 * state so we can reconstruct the screen.
 *---------------------------
 * A quick diagram of the state design used in this program:
 *
 *
 *  ATTRACT ÄÄ1ÄÄ MENU ÄÄ2ÄÄ OPENMENU ÄÄ6ÄÄ FOPEN
 *       \           ³
 *   ³     3          ÀÄÄÄÄ7ÄÄ OPPONENT ÄÄÄ8ÄÄ NAME¿
 *   4      \HELP                                    ³
 *   ³                                          ÄÄÄÄÙ
 *   
 *  GAME
 *   ³
 *   5
 *   
 *  FSAVE
 *
 * Transition  Cause
 * =========== ======
 *          1  Pressing MENU key on 95; pressing F11 on PC running TKERNEL
 *             ESCaping from menu returns to Attract mode.
 *          2  Selecting File from main menu.
 *          3  Pressing F1 on attract screen brings up help; ESC returns to attract
 *          4  Pressing F2 to start a game; pressing ESC to end it
 *          5  Pressing F1 during game play to save the game.
 *          6  Selection Open from the File menu
 *          7  Selection from the Opponent menu
 *          8  Selecting Human from the Opponent menu
 ***/
enum states { ATTRACT, MENU, FILEMENU, OPPONENTMENU, NAME,
              HELP, FOPEN, FSAVE, GAME, DONE};


/*** Program Strings ***
 * Keeping the strings in one data structure makes localization for international
 * versions of the program much simpler than keeping multiple versions of code.
 ***/
enum strings { TITLEs, MENUs, FILEMENUs, OPPONENTMENUs,
               MAINkey1s, MAINkey2s,
               HELPkey1s, HELPkey2s,
               GAMEkey1s, GAMEkey2s,
               YOUROPPONENTISs,
               COMPUTERs,HUMANs,
               ESCs, FOPENtitle, FSAVEtitle,
               NAMEPROMPTs,
               CONNECTINGs,
               TIMEOUTs,
               LOWMEMORY,
               YOURMOVEs,
               MYMOVEs,
               CONTINUEs,
               OPPONENTSMOVEs,
               IWONs,
               OPPONENTWONs,
               YOUWONs,
               TIEs,
               IMAHEADs,
               YOURAHEADs,
               OPPONENTAHEADs,
               OPPONENTSURRENDERs,
               ABORTGAMEs,
               BADFILESAVEs,
               BADFILEOPENs,
               ATTRACT1s,ATTRACT2s,ATTRACT3s,
               HELP1s,HELP2s,HELP3s,HELP4s,HELP5s,HELP6s,HELP7s,
               HELP8s,HELP9s,HELP10s,HELP11s,
               _last_string };
#define NUMBER_STRINGS _last_string
/* s(x) below converts a string number to pointer to the string. */
#define s(x) strptr[x]

char *stringconstant[ NUMBER_STRINGS ] = {
    "Boxes",                                       /* Program title */
    "File\0Opponent\0Print\0Quit",                 /* Top level menu */
    "New\0Open",                                   /* File menu */
    "Computer\0Human",                             /* Opponent menu */

    "    Begin                               ",    /* Attract keys (MAIN) */
    "Help                                    ",

    "                                        ",    /* Menu keys (HELP) */
    "Help                                    ",

    "                                   Quit ",    /* Game keys (GAME) */
    "Save                                    ",

    "Your opponent is ",
    "the Computer.",
    "a Human.",
    "Press ESC to continue",
    "File to open:",
    "File to save:",
    "Input your name:",
    "Please wait, connecting...",
    "IR Aborted! Computer will take over.",
    "LOW MEMORY!  Boxes cannot execute.",
    "Your move.           ",
    "My move...           ",
    "Continue your move!  ",
    "'s move...",
    "GAME OVER.  I WON!",
    "GAME OVER.  YOU LOST!",
    "GAME OVER.  YOU WON!",
    "GAME OVER.  IT'S A TIE!",
    "I'm ahead by ",
    "You're ahead by ",
    "'s ahead by ",
    " surrendered.",
    "Abort the game (Y/N)?",
    "The game file cannot be saved!",
    "The game file cannot be opened!",
    "  BOXES--A simple strategy game.",
    "      Press F1 for help,",
    "            F2 to begin.",
    "The object of this game is simple: you",
    "and the computer are trying to build",
    "boxes.  Whoever completes a box gets to",
    "claim it, and must move again.  After",
    "all boxes are finished, the higher box",
    "tally determines the victor.",
    "  Use the arrows to move the cursor,",
    "  and the spacebar to mark a box side.",
    "",
    "-------Press any key to exit help-------",
    ""
    };

/* templates for function keys */
/*
    "   ......  ......  ......  ......  ....."
    ".....  ......  ......  ......  ......   "
*/



/*** Menu States ***
 * The menu states are enumerated just like the program states so we can
 * leave processing and return to the same spot in the menu.
 */
enum { MENU_FILE, MENU_OPTIONS, MENU_PRINT, MENU_QUIT, _last_menu };
#define NUMBER_MENUS  _last_menu

enum { FILE_NEW, FILE_OPEN, _last_file_menu };
#define NUMBER_FILEMENUS _last_file_menu

enum { OPP_COMPUTER, OPP_HUMAN, _last_opp_menu };
#define NUMBER_OPPONENTMENUS _last_opp_menu



/* Key codes returned by System manager */
#define MENUKEY   0xC800
#define TMENUKEY  0x8500               /* F11 under TKernel */

#define CPACKMENUKEY  (app_settings->MenuKeyCode)
/* Usually F11 key which acts like menu under the Connectivity Pack,
 * but use whatever the system returns as the current menu key
 * (we'll get that from system settings later)
 */

#define F1          0x3B00
#define F2        0x3C00
#define F10          0x4400
#define ESC          27
#define ENTER       13
#define SPACE_BAR 32
#define UP_KEY    0x4800
#define DOWN_KEY  0x5000
#define LEFT_KEY  0x4b00
#define RIGHT_KEY 0x4d00

/* File-menu defines */
#define MAX_FILES 50
#define FM_BUF_SIZE ( MAX_FILES * (sizeof (FILEINFO) ) )


/* function prototypes */
void app_init(void);
void app_term(void);
void app_awake(void);
void app_sleep(void);
void app_break(void);
int app_key(void);
void app_display(void);
int  ESCmsg(char * msg, BOOL Response);
void disable_light_sleep(void);
void enable_light_sleep(void);
void InterruptibleLoop(BOOL (*RepeatingFunction)(int key));

void PrintBoard(void);
void OpenGame(char *Filename);
void SaveGame(char *Filename);
void MakeConnection(BOOL Open);
void NewGame(void);
void GameLoop(void);
void ShowCursor(BOOL Show);
void ShowScore(void);
void ShowBoard(void);
BOOL MainLoop(int key);
 

/* global state variables */
                              /* Various System Manager Structures */
EVENT app_event;                  /* Holds all info about an event */
FILEINFO fmbuffer[MAX_FILES];     /* Used for file names in File Menu */
SETTINGS *app_settings;           /* System settings--ptr returned by m_get_settings_addr */
MENUDATA app_menu;                /* Current menu and its status */
EDITDATA app_edit;                /* Current edit buffer and its status */

enum states state;
char * strptr[ NUMBER_STRINGS ];

FMENU app_fmenu = { (char far *) "C:\\_DAT",         /* Path */
                     (char far *) "*.BOX",           /* File spec pattern */
                     ( FILEINFO far * ) fmbuffer,    /* Buffer for file names */
                    FM_BUF_SIZE,                     /* Size of file name buffer (in bytes) */
                    -2,                              /* start line (-2 is second line) */
                    0,                               /* start column */
                    13,                              /* number lines (13 leaves fkey room) */
                    40,                              /* number cols */
                    3                                /* files per line  */
                    };

/* Constants & Variables for actual game */

char PlayerName[20],FriendsName[20];

char Board1[]="+  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +\r\n";
char Board2[]="                                              \r\n";


#define NUMBER_ROWS    5
#define NUMBER_COLUMNS 13

/* Bits for box sides & ownership */
#define TOP_SIDE    1
#define LEFT_SIDE   2
#define BOTTOM_SIDE 4
#define RIGHT_SIDE  8
#define ALL_SIDES   (TOP_SIDE|BOTTOM_SIDE|LEFT_SIDE|RIGHT_SIDE)
#define YOU_BIT    16
#define ME_BIT     32


int NumberSides[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
int Rank[5] = {2,3,1,4,0};
/*** Logic for box ranking goes like this:
  Sides  Rank
  -----  ----
    0     2     Better than two sides
    1     3        With one side is pretty good
    2     1        Not good, but cannot not make a move
    3     4        Any with 3 sides, go for it!
    4     0        Can't do one with 4 sides
***/

typedef char SCREEN[NUMBER_COLUMNS][NUMBER_ROWS];

SCREEN *Screen;

#ifdef TKERNEL
/* The following is a kludge for debugging under tkernel--tkernel does not
 * currently support the memory allocation functions (m_alloc,m_free,etc.).
 * If we are compiling for the tkernel version, we must allocate memory
 * out of the data segment.  In the actual System Manager initialization
 * code, we do an m_alloc.  In the tkernel version, we pretend to, but just
 * point Screen to the array TKernelScreen.
 * Note that we set TKERNEL from a /D(efine) switch in the make file.
 */
SCREEN TKernelScreen;
#endif


com_handle ComIR;
#define ACK 6
#define ENQ 5
#define EOT 4
#define EOF 26


BOOL Opponent,MyTurn,Connecting;

int boxes,score;

int cursorx,cursory,cursorside;
BOOL cursorstate;
int cursorwait;


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void main(void)
{
    m_init();       /* init call to system manager--must be executed */
    app_init();     /* application specific initialization */

    /* app_event loop */

    while (state != DONE) {
        m_event(&app_event);        /* Get next event, */
        switch (app_event.kind) {   /* and process whatever it may be */
            case E_ACTIV:
                app_awake();        /* reactivate app after suspension (user task switch)*/
                break;
            case E_DEACT:
                app_sleep();        /* prepare for suspension */
                break;
            case E_TERM:
                state = DONE;       /* we are being terminated */
                break;
            case E_BREAK:
                app_break();        /* app ctrl-break handler */
                break;
            case E_KEY:
                app_key();             /* process key */
                break;
            }
        }

    app_term();        /* application termination */
    m_fini();        /* terminate call to system manager, never returns */
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
    for( i = 0; i < NUMBER_STRINGS; i++ )
        strptr[ i ] = stringconstant[ i ];
  

    /* Expand data segment to allocate for the "Screen" (ie. the boxes block matrix).
     * The data segment is expanded by the requested number of bytes, and a pointer
     * returned to the first available byte of that expansion, or a 0 is returned
     * to indicate failure (either reached 64K DS limit or out of memory).
     * NOTE: The system manager may rearrange segments whenever ANY application 
     * begins or requests more memory.  For this reason, FAR pointers in System
     * Manager apps are a big no-no!
     */

#ifdef TKERNEL                       /* For an explanation of this version */
    Screen = &TKernelScreen;         /* dependent code, look above at the  */
#else                                /* definition of TKernelScreen...     */
    Screen = m_alloc(sizeof(SCREEN));
#endif

    if (!Screen) {
      ESCmsg(s(LOWMEMORY),FALSE);
      state = DONE;
      return;
      }

    /* Use m_get_settings_addr to get the system settings address */
    app_settings = m_get_settings_addr();

    Opponent = FALSE;
    Connecting = FALSE;  /* Only set during connection */

    NewGame();        /* Start off by assuming a new game */

    state=ATTRACT;
    app_display();    /* Display "attract" screen */
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
void ShowAttractScreen(void)
/*
 * Displays screen for attract mode
 */
{
  int i;
  char Buffer[40];

  for (i=0; i<3; i++)
    m_disp(2+i,0,s(ATTRACT1s+i),strlen(s(ATTRACT1s+i)),0,0);

  strcpy(Buffer, s(YOUROPPONENTISs));
  strcat(Buffer, Opponent?s(HUMANs):s(COMPUTERs));
  m_disp(7,4,Buffer,strlen(Buffer),0,0);
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ShowHelpScreen(void)
/*
 * Displays help screen
 */
{
  int i;

  for (i=0; i<12; i++)
    m_disp(i,0,s(HELP1s+i),strlen(s(HELP1s+i)),0,0);
}



/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void DisplayFKeys(int n,int n1)
/*
 * Displays fkey lines n & n1 from system string table on bottom two lines
 */
{
  int i;

  m_disp(11,0,s(n),40,INV,0);  /* m_disp(Row,Col,String,String_length,Attr,Dummy); */
  m_disp(12,0,s(n1),40,INV,0); /* We know the f_key strings will be 40 chars long */

}



/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_display(void)
/*
 * Application displayer.
 *
 * Note that this function has to be able to reconstruct the ENTIRE screen,
 * at any time (well, almost--excluding special functions like ESCmsg below
 * that call m_lock/m_unlock to prevent context switches).
 *
 * If an application switch happens, we will have a blank screen upon
 * reentering our application.  Because of this, it is best to save the
 * program state in various variables, and redisplay the screen as a whole.
 */
{
    switch( state ) {
        case ATTRACT:
            drawbox( s( TITLEs ) );   /* Create the app-standard double line title "box"*/
            m_telltime( DATE_N_TIME, -3, 19 );   /* Show DATE_N_TIME (see smtime.h for more)... */
/*
 * Note for all System Manager display calls: 
 *  þ Rows are first, Columns are second.
 *  þ Rows begin with -3 (0 is the first non-"reserved" screen row)
 *  þ Columns begin with 0 
 * For the above m_telltime(DATE_N_TIME, -3,19), the timestamp is shown
 * on the top line, 19 chars over.
 */
            DisplayFKeys(MAINkey1s,MAINkey2s);

            ShowAttractScreen();

            m_setcur( 0, -1 );  /* Any non-physical screen address means "hide the cursor" */
            break;

        case MENU:
        case FILEMENU:
        case OPPONENTMENU:
            drawbox( "" );   /* Clear the display, but show the ÍÍ border */
            menu_dis( &app_menu );      /* Show the menu as currently set in the structure */

            DisplayFKeys(HELPkey1s,HELPkey2s);
            break;

        case FOPEN:   /* Note that the display code is shared, but the actual */
        case FSAVE:   /* contents of the app_fmenu struct will be different */
            fmenu_dis( &app_fmenu, &app_edit ); /* Show the current file menu */

            DisplayFKeys(HELPkey1s,HELPkey2s);
            break;

        case HELP:
            ShowHelpScreen();
            DisplayFKeys(MAINkey1s,HELPkey1s);
            m_setcur(0,-1);
            break;

        case NAME:
            drawbox("");
            edit_dis(&app_edit);  /* Show the edit line as currently set */
            DisplayFKeys(HELPkey1s,HELPkey2s);
            break;

        case GAME:
            drawbox("");         /* Erase screen, but show line */

            ShowScore();
            ShowBoard();

            DisplayFKeys(GAMEkey1s, GAMEkey2s);
            m_setcur( 0, -1 );  /* Any non-physical screen address means "hide the cursor" */
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
void app_menu_key(int key)
/*
 * Handles keystrokes when inside the menu system (main level).
 */
{
  int app_menuselect;

  switch (key) {
     case ESC:
         state = ATTRACT;              /* Pressing ESC clears menu line- */
         menu_off( &app_menu );        /* Turn off menus */
         app_display();                /* And redisplay */
         break;
     case F1:
         ESCmsg("Put Help for menu here!",FALSE);   /* No help from Menu */
         break;

     default:
         menu_key( &app_menu, app_event.data, &app_menuselect );  /* Send System Manager the menu key */

         switch( app_menuselect ) {     /* and branch on the result */
           case -1:                     /* -1 means no final selection made */
               menu_dis(&app_menu);     /*    so redisplay menu */
               break;
           case MENU_FILE:              /* Else, a sequential 0-based number (that we have enum-ed) */
               state = FILEMENU;

               menu_setup( &app_menu, s( FILEMENUs ), NUMBER_FILEMENUS, 1, NULL, 0, NULL );
               menu_on( &app_menu );
               app_display();
               break;
           case MENU_OPTIONS:
               state = OPPONENTMENU;

               menu_setup( &app_menu, s( OPPONENTMENUs ), NUMBER_OPPONENTMENUS, 1, NULL, 0, NULL );
               menu_on( &app_menu );
               app_display();
               break;

           case MENU_PRINT:
               PrintBoard();
               state = ATTRACT;
               app_display();
               break;

           case MENU_QUIT:
               state = DONE;
               break;
           }
   }
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_filemenu_key(int key)
/*
 * Handles keystrokes when inside the menu system (file level).
 */
{
  int app_menuselect;

  switch (key) {
     case ESC:
         state = MENU;           /* Pressing ESC backs up to prev. menu */
         menu_setup( &app_menu, s( MENUs ), NUMBER_MENUS, 1, NULL, 0, NULL );
         menu_on( &app_menu );
         app_display();
         break;
     case F1:
         ESCmsg("Put Help for file menu here!",FALSE);   /* No help from Menu */
         break;

     default:
         menu_key( &app_menu, app_event.data, &app_menuselect );  /* Send System Manager the menu key */

         switch( app_menuselect ) {     /* and branch on the result: */
           case -1:                     /* -1 means no final selection yet made */
               menu_dis(&app_menu);     /*   so show new menu cursor */
               break;
           case FILE_NEW:              /* Else, a sequential 0-based number (that we have enum-ed) */
               NewGame();
               state = ATTRACT;
               menu_off( &app_menu );
               app_display();
               break;

           case FILE_OPEN:
               state = FOPEN;
               app_fmenu.fm_pattern = "*.BOX";  /* Set the file menu search pattern */
               app_edit.prompt_window = TRUE;      /* We do want a prompt */
               app_edit.prompt_line_length = 0;    /* Dummy, but must be set */
               app_edit.message_line = s( FOPENtitle );  /* The actual prompt */
               app_edit.message_line_length = strlen( s( FOPENtitle ) );  /* Length of the prompt */
               fmenu_init( &app_fmenu, &app_edit, "*.BOX", 9, 1 );        /* Initialize the file-menu */
               app_display();
               break;

           }
   }
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void app_oppmenu_key(int key)
/*
 * Handles keystrokes when inside the opponent menu.
 */
{
  int app_menuselect;

  switch (key) {
     case ESC:
         state = MENU;           /* Pressing ESC backs up to prev. menu */
         menu_setup( &app_menu, s( MENUs ), NUMBER_MENUS, 1, NULL, 0, NULL );
         menu_on( &app_menu );
         app_display();
         break;
     case F1:
         ESCmsg("Put Help for opponent menu here!",FALSE);   /* No help from Menu */
         break;

     default:
         menu_key( &app_menu, app_event.data, &app_menuselect );  /* Send System Manager the menu key */

         switch( app_menuselect ) {     /* and branch on the result: */
           case -1:                     /* -1 means no final selection yet made */
               menu_dis(&app_menu);     /*   so show new menu cursor */
               break;

           case OPP_HUMAN:              /* Else, a sequential 0-based number (that we have enum-ed) */

               edit_top(&app_edit,      /* Set up edit on top line */
                        PlayerName, strlen(PlayerName),   /* Inital value */
                        20,                               /* Max length */
                        s(NAMEPROMPTs),strlen(s(NAMEPROMPTs)),  /* First line prompt & len */
                        "",0);                                  /* Second line prompt & len */
               /* Can also use edit_init(&e,init_buf,ini_len,max_len,display_line,display_col);
                * for "random" screen placement of edit fields
                */

               Opponent = TRUE;         /* Set Human player flag */
               state = NAME;
               app_display();
               break;

           case OPP_COMPUTER:
               Opponent = FALSE;
               state = ATTRACT;
               app_display();
               break;

           }
   }
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
int app_key(void)
/*
 * Application keystroke processor.
 */
{
  int Result;
  int i;

    switch( state ) {

        case ATTRACT:
            if (app_event.data==CPACKMENUKEY)     /* If running under tkernel, convert current menu */
                app_event.data=MENUKEY;        /* key to look like the MENU key on the 95 */

            switch( app_event.data ) {
                case TMENUKEY:
                case MENUKEY:
                     state = MENU;
                     menu_setup( &app_menu, s( MENUs ), NUMBER_MENUS, 1, NULL, 0, NULL );
                     menu_on( &app_menu );
                     app_display();
                     break;
                case F1:  /* Help */
                     state=HELP;
                     app_display();
                     break;
                case F2:
                     GameLoop();        /* Play the game */
                     break;
                default:
                     m_beep();
            }
            break;

        case HELP:
            if (app_event.data==F2)    /* F2 begins game from help screen */
              GameLoop();
            else {
              state = ATTRACT;          /* Any other key aborts help */
              app_display();
              }
            break;

        case NAME:
            edit_key(&app_edit, app_event.data, &Result);  /* Process key app_edit--return Result */
            edit_dis(&app_edit);
            if (Result==1) {

              i = strlen(app_edit.edit_buffer)-1;      /* Strip off spaces */

              while (i>0 && app_edit.edit_buffer[i]==' ')  i--;

              app_edit.edit_buffer[++i] = 0;

              strcpy(PlayerName,app_edit.edit_buffer);

              state=ATTRACT;
              app_display();
              }
            break;

        case MENU:
            app_menu_key(app_event.data);
            break;
        case FILEMENU:
            app_filemenu_key(app_event.data);
            break;
        case OPPONENTMENU:
            app_oppmenu_key(app_event.data);
            break;

        case FOPEN:
        case FSAVE:
            if( app_event.data == F1 )
              ESCmsg("Help subsystem would be entered",FALSE);
            else
          switch (fmenu_key( &app_fmenu, &app_edit, app_event.data ) ) {
                case RET_OK:
                        break;
                    case RET_ACCEPT:         /* User picks something... */
                        fmenu_off( &app_fmenu, &app_edit );  /* wipe file stuff off screen */
                        if (state==FOPEN)
                          OpenGame(app_edit.edit_buffer);    /* Use the returned filename to open the file */
                        else
                          SaveGame(app_edit.edit_buffer);    /* Use returned name to save */

                        state = ATTRACT;
                        app_display();          /* Re-enter attract mode */
                        break;
                    case RET_ABORT:
                        fmenu_off( &app_fmenu, &app_edit );
                        state = ATTRACT;
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
int ESCmsg( char * msg , BOOL Response)
/*
 * Display a message which requires ESC to continue;
 *   if Response is TRUE, look for a Y or N as well.  Return the key
 *   that breaks the message.
 * Lock the user into responding to this message with the m_lock()
 * call which will prevent task-switches.
 */
{   /* Show two line message box: message(line1,len_line1,line2,len_line2); */
    /* --For a three line message, call message3(l1,len_l1,l2,len_l2,l3,len_l3); */

    if (Response)
      message(msg, strlen(msg), "", 0);
    else
      message(msg, strlen(msg), s( ESCs ), strlen( s( ESCs ) ) );

    m_lock();              /* Keep m_event call from acknowledging Application keys */
    while (1) {
        m_event( &app_event );
        if ( app_event.kind == E_KEY )  {  /* Look at type of event (only respond to KEY) */
           if (Response) {
             app_event.data = toupper(app_event.data);
             if (app_event.data=='Y' || app_event.data=='N')
               break;
             }

           if (app_event.data == ESC)    /* If the key type is an ESC, quit, */
              break;
           else
              m_beep();        /* else m_beep() (high pitched beep) */
           }
        }
    m_unlock();            /* Restore ability of application keys to steal process */

    msg_off();             /* Restore screen contents underneath message box */

    return app_event.data;   /* Return breaking keystroke */
}

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void InterruptibleLoop(BOOL (*RepeatingFunction)(int key))
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
        Done = (*RepeatingFunction)(0);   /* Call our repeating function--a return of TRUE terminates loop */
        m_nevent( &app_event );     /* Look at n(ext)event (to avoid .5 sec event timeout imposed by m_event) */

        switch (app_event.kind) {
            case E_BREAK:       /* Allow a break key to terminate */
                Done = TRUE;
                break;

            case E_KEY:
                m_event(&app_event);
                Done = (*RepeatingFunction)(app_event.data);  /* Call repeater with keystroke--TRUE is terminate */
                break;

            case E_ACTIV:               /* Being reactivated: */
                app_display();          /* Redisplay screen */
                disable_light_sleep();  /* Coming back from task switch--disable l.s. */
                break;

            case E_DEACT:
                enable_light_sleep();   /* Switching away to another task--re-enable l.s. */
                break;
            }
        }
   
    enable_light_sleep();   /* Allow CPU to save as much battery life as possible */
}

/*-----------------------------------------------------------------------*/
void disable_light_sleep(void)
/*
 * Use 95's special BIOS routine to disable light sleep
 * Interrupt 15h, Function 4Eh, AL=0 to disable, 1 to enable light sleep
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
 * Interrupt 15h, Function 4Eh, AL=0 to disable, 1 to enable light sleep
 */
{
_asm {
     mov ax,4e01h
     int 15h
     }
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/*--   Actual game logic follows...                                    --*/
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void PrintBoard(void)
{
  int i;

  m_open_printer();    /* Needed before any printer access--user sets up printer through SETUP */

  for (i=0; i<10; i++) {
    m_write_printer(Board1,strlen(Board1));  /* print -- needs buffer and length */
    m_write_printer(Board2,strlen(Board2));
    }

  m_close_printer();   /* Relinquish printer control */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void SendChar(char x, BOOL TurnAround)
/*
 * Sends a char x.  If TurnAround, switch from send to receive mode.
 */
{
  char Buffer[2];
  int len;

  Buffer[0] = x;

  len = 1;
  ComSendBytes(ComIR, Buffer, (TurnAround)?COM_CTL_SETRCV:0, &len);  /* Note we use a *pointer* to len */
  /* Send an "x" (0=No options, COM_CTL_SETRCV=transfers IR to receive mode) */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
int GetChar(void)
/*
 * Gets a char from Com line.  Returns char or -1 on a timeout.
 * Will wait (for some time) until a char is available.
 */
{
  char Buffer[2];
  int len;
  int loop=0;

  while (loop++<32767) {        /* Timeout value */
    len = 1;
    ComReceiveBytes(ComIR, Buffer, &len);  /* Get a *max* of len bytes; put them into buffer */
    if (len) return Buffer[0];
    }

  ESCmsg(s(TIMEOUTs),FALSE);     /* Tell user we're breaking */

  MakeConnection(FALSE);         /* Actually break */
  Opponent = FALSE;
  MyTurn = TRUE;                 /* and set program flags */

  return -1;
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL RequestACK(void)
/*
 * Waits for an ACK on opposing end, otherwise a timeout occurs and
 * the mode automatically goes into computer player.
 * Returns TRUE if the ACK was sent, FALSE if a timeout occurred.
 */
{
  char Buffer[2];
  int free,size;
  int loop=0;

  while (loop++<32767) {       /* Timeout value */

    ComQryRxQue(ComIR, &size, &free);   /* Look at Rx buffer fill status */
    if (size!=free) {                   /* Size is buffer size (constant), free is bytes available */
      size=1;               /* Accept one byte */
      ComReceiveBytes(ComIR, Buffer, &size);     /* And put it in Buffer */

      if (size==1 && Buffer[0]==ACK)  return TRUE;
      }
    }

  ESCmsg(s(TIMEOUTs),FALSE);     /* Tell user we're breaking */

  MakeConnection(FALSE);         /* Actually break */
  Opponent = FALSE;
  MyTurn = TRUE;                 /* and set program flags */

  return FALSE;
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL GetName(void)
/*
 * Gets other player's name, returns TRUE if sucessful
 */
{
 int len,ch;


 len = 0;

 do {
   ch = GetChar();                         /* Keep getting chars */
   if (ch!=-1)  FriendsName[len++] = ch;   /* And stuffing into name */
 } while ((ch>0) && (len<sizeof(FriendsName)));
                     /* Until end of string, timeout, or too many chars */

 FriendsName[sizeof(FriendsName)-1] = 0;   /* Add terminator if we had to truncate */

 return (!ch);        /* Only return true if EOS was reached */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void SendName(void)
/*
 * Sends our name, and goes into receive mode.
 */
{
  int i;

  for (i=0; PlayerName[i]; i++)  SendChar(PlayerName[i], FALSE);

  SendChar(0, TRUE);       /* Send terminator, and turn around */
}



/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void MakeConnection(BOOL Open)
/*
 * Based on Open, we either make (if Open=TRUE) an IR connection to another
 * 95LX, or break (if Open=FALSE) an IR connection.
 *
 * The sequence of events for making a connection follows:
 *    SENDER(1st) RECEIVER(2nd)
 *    ------      --------
 *     ENQ    -->
 *            <--   ACK
 *            <--   "Name"
 *     ACK    -->
 *     "Name" -->
 *
 * Neither side knows which one is the 1st until someone responds to an
 * ENQ.  The first to respond is the 2nd player.
 */
{
  com_settings settings;
  int free,size,loop;
  BOOL Done;

  if (!Opponent)  return;

  if (Open) {
    ComOpen(&ComIR, COM_LINE_1);  /* Get access to line 1, and return handle info in ComIR */

    settings.Dial = COM_ANS_STOP;
    settings.XonXof = COM_XON_OFF;
    settings.Duplex = COM_DUP_HALF;
    settings.Echo = COM_NOECHO;
    settings.InfraRed = COM_IR_ON;  /* Use IR, not Com port */

    settings.Baud = COM_BR_2400;
    settings.Parity = COM_PTY_NO;    /* 2400, No parity, 1 stop, 8 data */
    settings.Stop = COM_STOP_1;
    settings.Data = COM_DATA_8;
    ComSet(ComIR, &settings);   /* Set the port to our settings */

    ComReset(ComIR, COM_RESET_TXB | COM_RESET_RXB);   /* Flush both buffers for ComIR */

    Done = FALSE;

    loop = 0;
    while (!Done) {
      m_nevent(&app_event);                 /* Look for next event a keystroke */
      if (app_event.kind==E_BREAK || app_event.kind==E_KEY) {
        m_event(&app_event);                         /* Flush the keyboard event */
        ESCmsg(s(TIMEOUTs),FALSE);
        Opponent = FALSE;
        Done = TRUE;
        break;
        }

      ComQryRxQue(ComIR, &size, &free);   /* Query Recieve Buffer for free bytes and total size */
      if (size!=free) {      /* Some chars there! */

        switch (GetChar()) {
          case ACK:             /* Got an ACK response--we're 1st */
            if (GetName()) {
              SendChar(ACK, FALSE);
              SendName();
              }
            MyTurn = TRUE;            /* Start with our turn */
            Done = TRUE;              /* and terminate the loop */
            break;

          case ENQ:             /* Got an ENQ response from our friend--we're 2nd */
            SendChar(ACK,FALSE);                 /* ACK and turnaround */
            SendName();
            if (RequestACK())                   /* If completes ACK, get name too */
              GetName();
            MyTurn = FALSE;                     /* Start with our friend's turn */
            Done = TRUE;                        /* End loop */
            break;
          }
        }

      if (!Done && (loop-- <= 0)) { /* Send periodic ENQ's to wake up our opponent */
        loop = 5000;
        SendChar(ENQ, TRUE);      /* Send an ENQ, and turnaround */
        }
      }

    }

  else {          /* Terminate the connection */
    ComClose(ComIR);
  }

  ShowScore();    /* Update the title */
}





/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL SendMove(int x,int y, int side)
/*
 * Sends the indicated move across the serial port.
 */
{
  char Buffer[4];

  SendChar((char) (x+32), FALSE);       /* Move consists of x,y,side */
  SendChar((char) (y+32), FALSE);       /* 32 is added to eliminate control chars */
  SendChar((char) (side+32), TRUE);     /* Turnaround to wait for ACK */

  return RequestACK();
}

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL TransferTurn(void)
/*
 * Switches current player to other machine.
 */
{
  char Buffer[40];

  m_clear(-3,0,1,40);
  strcpy(Buffer, FriendsName);
  strcat(Buffer, s(OPPONENTSMOVEs));
  m_disp(-3,0, Buffer, strlen(Buffer), 0, 0);    /* New top line */
  m_dirty_sync();                                /* and update */

  MyTurn = FALSE;               /* No longer our turn */

  SendChar(EOT, TRUE);          /* Send EOT and turn to wait for ACK */

  return RequestACK();
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL ReceiveMove(void)
/*
 * Gets an opponent's move if one is in the IR buffer.
 * Move may be a request to switch sides, or an abort.
 * We return TRUE for any good move, FALSE for an abort command.
 */
{
  char Buffer[40];
  int size,free;
  int x,y,side;
  int i;
  int ch;

  ComQryRxQue(ComIR, &size, &free);   /* Find out if chars are waiting (size != free) */

  if (size!=free) {
    ch = GetChar();                         /* Get first char */

    switch (ch) {
      case EOF:
          strcpy(Buffer, FriendsName);
          strcat(Buffer, s(OPPONENTSURRENDERs));
          ESCmsg(Buffer,FALSE);
          return FALSE;
          break;

      case EOT:                 /* EOT = End of Turn */
          m_clear(-3,0,1,40);               /* Wipe top line */
          m_disp(-3,0, s(YOURMOVEs), strlen(s(YOURMOVEs)), 0, 0);    /* New top line */
          m_dirty_sync();                                            /* and update */

          MyTurn = TRUE;
          SendChar(ACK, FALSE);   /* For a turn transfer, don't go into receive mode */
          break;
      default:                  /* Any other is move information */
          x = ch - 32;          /* Snatch the first char */

          y = GetChar() - 32;   /* and get other move params */
          side = GetChar() - 32;

          if (x>=0 && x<NUMBER_COLUMNS &&   /* Only accept valid moves */
              y>=0 && y<NUMBER_COLUMNS &&
              side>=0 && side<RIGHT_SIDE) {
            EnterMove(x,y,side, ME_BIT);  /* An opponent's move simulates ME, ie. the computer */
            ShowScore();
            ShowCursor(TRUE);             /* Re-update display & counters */

            SendChar(ACK, TRUE);  /* Send the ACK, and turnaround for another cmd */
            }
      }
    }
  return TRUE;
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void SaveGame(char *Filename)
/*
 * Saves current game status into the file with the given name
 */
{
  FILE File;
  int err;

  int i,extension;

  /* Add an extension if one is missing */
  extension = FALSE;
  for (i=0; Filename[i]; i++)  if (Filename[i]=='.')  extension = TRUE;
  if (!extension)  strcat(Filename,".BOX");

  /* m_fcreat is just like C creat--it makes a file if one doesn't exist, */
  /* and it truncs an existing file to 0.  (m_create is avail, and it     */
  /* will only create if a file doesn't yet exist).   SYNTAX:             */
  /* m_fcreat(FILE *File, char *Name, len(Name), sys attrs, BOOL SupressBuffering) */

  err = m_fcreat(&File, Filename, strlen(Filename), 0, 0);
  if (err)  goto error;        /* Abort the save if cannot create a file */

  m_write(&File, Screen, sizeof(SCREEN));  /* m_write(&File, &Buffer, Bufferlen) */

  m_write(&File, &boxes, sizeof(boxes));
  err = m_write(&File, &score, sizeof(score));

  m_close(&File);        /* Closes the file */

  if (err)    goto error;     /* Couldn't write--close file and abort */
  return;

error:
  ESCmsg(s(BADFILESAVEs),FALSE);
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void OpenGame(char *Filename)
/*
 * Loads up the game state from the given file into the game variables
 */
{
  FILE File;
  int err,len;
  int i,extension;

  /* Add an extension if one is missing */
  extension = FALSE;
  for (i=0; Filename[i]; i++)  if (Filename[i]=='.')  extension = TRUE;
  if (!extension)  strcat(Filename,".BOX");

  err = m_open(&File, Filename, strlen(Filename), 0, 0);  /* See above m_fcreat for param description */

  if (err) goto error;

  m_read(&File, Screen, sizeof(SCREEN), &len);   /* Just like DOS, returns a len which */
                                                 /* if != passed length, indicates error */
  m_read(&File, &boxes, sizeof(boxes), &len);
  m_read(&File, &score, sizeof(score), &len);

  m_close(&File);

  if (len!=sizeof(score))  goto error;   /* Check for error on last one (assuming error will propagate) */
  return;

error:
  ESCmsg(s(BADFILEOPENs),FALSE);
}



/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void AllowUserSave(void)
/*
 * Puts program into save filemenu event loop--exiting this brings the user
 *   back into the game.
 * Game saving is done from inside the game to illustrate using the
 * event handling calls to take advantage of the program structure.
 * Although it is rather contrived (we don't need to be always active), this
 * example shows how it could be done.  In this case, create a special
 * event handling loop (like what is done with InterruptibleLoop) to handle
 * this exception instead of adding more flags to indicate complex program
 * states.
 *
 * For more active games that REQUIRE using the InterruptibleLoop construct,
 * it is much easier to segment the responsibilities this way.
 *
 * Note that app_display and app_key (the display and key handlers ) are still
 * responsible for handling the event details, even in this specialized loop.
 */
{
  app_fmenu.fm_pattern = "*.BOX";  /* Set the file menu search pattern */
  app_edit.prompt_window = TRUE;      /* We do want a prompt */
  app_edit.prompt_line_length = 0;    /* Dummy, but must be set */
  app_edit.message_line = s( FSAVEtitle );  /* The actual prompt */
  app_edit.message_line_length = strlen( s( FSAVEtitle ) );  /* Length of the prompt */
  fmenu_init( &app_fmenu, &app_edit, "*.BOX", 9, 1 );        /* Initialize the file-menu */

  state = FSAVE;
  app_display();

  enable_light_sleep();   /* Save battery power (l.s. will be disabled because */
                          /* we're entering from the GAME mode */

  while (state==FSAVE) {   /* If the state exits FSAVE, we need to abort */
    m_event( &app_event );     /* Look at next event */
    switch (app_event.kind) {
       case E_KEY:
          app_key();
          break;
       }
    }

  disable_light_sleep();   /* Return CPU to high speed mode */

  state = GAME;            /* Return to game */
  app_display();
}




/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ShowBox(int x, int y, int invertside)
{
  int sx,sy,side;

  if (x>=NUMBER_COLUMNS || y>=NUMBER_ROWS)  return;

  sx = x*3;  sy = y*2;             /* Boxes are 3 wide, 2 tall */
  side = (*Screen)[x][y];

  /* Display all sides currently filled in */

  m_disp(sy+1, sx, (side & LEFT_SIDE)  ?"º" :" ", 1, (invertside&LEFT_SIDE)?1:0,  0);
  m_disp(sy,  sx+1,(side & TOP_SIDE)   ?"ÍÍ":"  ",2, (invertside&TOP_SIDE)?1:0,   0);
  m_disp(sy+1,sx+3,(side & RIGHT_SIDE) ?"º" :" ", 1, (invertside&RIGHT_SIDE)?1:0, 0);
  m_disp(sy+2,sx+1,(side & BOTTOM_SIDE)?"ÍÍ":"  ",2, (invertside&BOTTOM_SIDE)?1:0,0);

  if (side & ME_BIT)
    m_disp(sy+1,sx+1,"",2,NORM,0);      /* Display computer "signature" */

  if (side & YOU_BIT)
    m_disp(sy+1,sx+1,"",2,NORM,0);      /* Display player "signature" */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ShowCursor(BOOL Show)
/*
 * Display selection cursor
 */
{
  ShowBox(cursorx,cursory,(Show)?cursorside:0);
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ShowBoard(void)
/*
 * Display boxes game board
 */
{
  int x,y;

  m_clear(0,0,11,40);   /* Clears screen block from (row,col), height=11, width=40 */


  for (x=0; x<=NUMBER_COLUMNS; x++)
    for (y=0; y<=NUMBER_ROWS; y++) {
      if (x<NUMBER_COLUMNS && y<NUMBER_ROWS)  ShowBox(x,y,0);
      m_disp(y*2,x*3,"Å",1,NORM,0);
      }

  ShowCursor(TRUE);
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void IncCursor(int ax,int ay)
/*
 * Move the cursor one unit in any direction.
 * Movement "waggles" so that the cursor can visit every line that is
 * not in a rectangular pattern.
 */
{
  /* Left/Right movement only */

  if (ax) {
    switch (cursorside) {
      case LEFT_SIDE:
        if (cursorx || ax>0) {
          if (ax<0)  cursorx+=ax;
          cursorside = TOP_SIDE;
          }
        break;
      case TOP_SIDE:
        if (cursorx<NUMBER_COLUMNS-1) {
          if (ax>0)  cursorx+=ax;
          cursorside = LEFT_SIDE;
          }
        else {
          if (ax>0)  cursorside = RIGHT_SIDE;
                else cursorside = LEFT_SIDE;
          }
        break;
      case BOTTOM_SIDE:
        if ((cursorx<NUMBER_COLUMNS-1 && ax>0) ||
           (cursorx && ax<0))
          cursorx+=ax;
        break;
      case RIGHT_SIDE:
        if (ax<0)  cursorside = TOP_SIDE;
        break;
      }
    }

  /* Up/Down movement only */

  if (ay) {
    switch (cursorside) {
      case TOP_SIDE:
        if (cursory || ay>0) {
          if (ay<0)  cursory+=ay;
          cursorside = LEFT_SIDE;
          }
        break;
      case LEFT_SIDE:
        if (cursory<NUMBER_ROWS-1) {
          if (ay>0)  cursory+=ay;
          cursorside = TOP_SIDE;
          }
        else {
          if (ay>0)  cursorside = BOTTOM_SIDE;
                else cursorside = TOP_SIDE;
          }
        break;
      case RIGHT_SIDE:
        if ((cursory<NUMBER_ROWS-1 && ay>0) ||
           (cursory && ay<0))
          cursory+=ay;
        break;
      case BOTTOM_SIDE:
        if (ay<0)  cursorside = LEFT_SIDE;
        break;
      }
    }

}

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL CheckFinished(int x,int y, int player)
/*
 * If the box at (x,y) is completed, places player's marker in it.
 * returns TRUE if box was finished.
 * Keeps tally of boxes left and score.
 */
{
  if ((*Screen)[x][y] == ALL_SIDES) {
    (*Screen)[x][y] |= player;
    boxes--;
    if (player==ME_BIT)  score--;
                    else score++;
    return TRUE;
    }
  else
    return FALSE;
}




/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
int EnterMove(int x,int y, int side, int player)
/*
 *  Puts in a line for box (x,y) on side for the given player.
 *  Note that if the box is finished, the appropriate player symbol is
 *  entered.
 *  Returns are:
 *      0: Bad move (position already occupied).
 *      1: Move okay, but did not finish a box.
 *      2: Move okay, finished a box.
 *      3: Game finished.
 */
{
  BOOL Finished=FALSE;

  if ((*Screen)[x][y] & side)  return 0;   /* Move already made */

  if (side==BOTTOM_SIDE && y!=NUMBER_ROWS-1) {   /* Normalize to top if possible */
    side=TOP_SIDE; y++;
    }

  if (side==RIGHT_SIDE && x!=NUMBER_COLUMNS-1) { /* Normalize to left if possible */
    side=LEFT_SIDE; x++;
    }

  (*Screen)[x][y] |= side;
  Finished |= CheckFinished(x,y, player);

  ShowBox(x,y,0);

  if (!boxes)  return 3;                     /* Game over */

  if (side!=BOTTOM_SIDE && side!=RIGHT_SIDE) {  /* If we aren't looking at an edge */

    if (side==LEFT_SIDE) {    /* Flip to opposite side of adjacent box */
      x--;
      side=RIGHT_SIDE;
      }
    else {
      y--;
      side=BOTTOM_SIDE;
      }

    if (x>=0 && y>=0) {             /* If we're still on the screen */
      (*Screen)[x][y] |= side;
      Finished |= CheckFinished(x,y, player);
      ShowBox(x,y,0);                          /* Update adjacent box's "initials" */

      if (!boxes)  return 3;               /* Game over */
      }
    }

  m_dirty_sync();                          /* Update screen to reflect changes */

  return 1+Finished;
}



/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void NewGame(void)
/*
 * Initialize variables for a new game (score, level, etc.)
 */
{
  int i,j;

  for (i=0; i<NUMBER_COLUMNS; i++)   /* Wipe out "screen" (box matrix) */
    for (j=0; j<NUMBER_ROWS; j++)
      (*Screen)[i][j] = 0;

  boxes = NUMBER_COLUMNS*NUMBER_ROWS;     /* Set score and # boxes uncompleted */
  score = 0;
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void GameLoop(void)
/*
 * Control the execution of the game--loops until game finished, then returns
 */
{
  char Buffer[40];

  cursorx = NUMBER_COLUMNS>>1;       /* Init cursor location */
  cursory = NUMBER_ROWS>>1;
  cursorside = TOP_SIDE;

  Connecting = Opponent;          /* Only set TRUE if an opponent */

  state=GAME;                     /* Init game display */
  app_display();
  m_dirty_sync();                 /* And make sure it shows */

  MakeConnection(TRUE);

  Connecting = FALSE;             /* Done connecting */
  ShowScore();                    /* so show it */

  if (!MyTurn && Opponent) {      /* If our friend's turn, display it */
     strcpy(Buffer, FriendsName);
     strcat(Buffer, s(OPPONENTSMOVEs));
     m_disp(-3,0, Buffer, strlen(Buffer), 0, 0);    /* New top line */
     }

  cursorstate = TRUE;
  cursorwait = 0;

  InterruptibleLoop(MainLoop);   /* Enter game loop */

  MakeConnection(FALSE);

  state=ATTRACT;                 /* Game loop all over, return to attract mode */
  app_display();

  NewGame();                     /* Initialize in case user comes back in */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
int ComputerMove(void)
/*
 * Handle continuous operation of game.
 * Returns same status as EnterMove--3 means game over.
 */
{
  int Move;
  int i,j;
  int temp,rank1;
  int side;
  int bx,by;                /* Best guesses for the next move */
  int score;                /* "Score" of current best guess */

  m_clear(-3,0,1,40);        /* Wipe out old top line: clears 1 row, 40 cols @ (-3,0) */
  m_disp(-3,0, s(MYMOVEs), strlen(s(MYMOVEs)), 0, 0);    /* New top line */
  m_dirty_sync();                                        /* and update */

another:
  score = 0;
  bx = NUMBER_COLUMNS>>1;
  by = NUMBER_ROWS>>1;

  for (i=0; i<NUMBER_COLUMNS; i++)            /* Rank all boxes, and pick the best */
    for (j=0; j<NUMBER_ROWS; j++) {
      rank1 = Rank[ NumberSides[ (*Screen)[i][j] & ALL_SIDES ] ];

      if (rank1>score) {
        score = rank1;
        bx = i;
        by = j;
        }
      }

  if (!score)  return 3;          /* No boxes were free, so game over! */


  /* Figure out which side on given box to move on */

  side = 0;

  for (temp=8; temp>=1; temp>>=1)                       /* Pick the first free side */
    if (!((* Screen)[bx][by] & temp))  side = temp;

  Move = EnterMove(bx,by,side, ME_BIT);  /* "Register" the move */
  if (Move==2)  goto another;            /* If computer makes a box, let it do another */

  m_disp(-3,0, s(YOURMOVEs), strlen(s(YOURMOVEs)), 0, 0);    /* New top line */
  m_dirty_sync();                                            /* and update */

  return Move;
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void ShowScore(void)
/*
 * Tell who's ahead...
 */
{
  char Buffer[41];
  int len;

  m_clear(-3,0,1,40);        /* Wipe out old top line: clears 1 row, 40 cols @ (-3,0) */

  /* Only need to decide if Your or Your friend's turn */
  /*   (computer move happens between display points)*/

  if (Connecting)
    strcpy(Buffer, s(CONNECTINGs));
  else if (!Opponent | MyTurn)
    strcpy(Buffer, s(YOURMOVEs));
  else {
    strcpy(Buffer, FriendsName);
    strcat(Buffer, s(OPPONENTSMOVEs));
    }

  m_disp(-3,0,Buffer, strlen(Buffer), 0, 0);  /* Display Buffer on top line */

  /* Decide if you're ahead, your friend's ahead, the computer's ahead, or nobody */
  if (score<0) {
    if (Opponent) {
      strcpy(Buffer, FriendsName);
      strcat(Buffer, s(OPPONENTAHEADs));
      }
    else
      strcpy(Buffer, s(IMAHEADs));
    }
  else if (score>0)
    strcpy(Buffer, s(YOURAHEADs));
  else
    Buffer[0] = 0;


  if (score)
    itoa(abs(score), Buffer+strlen(Buffer), 10);  /* Append score to string */

  len = strlen(Buffer);

  while (len<40)  Buffer[len++] = ' ';        /* Pad string out to 40 chars */

  Buffer[len] = 0;

  m_disp(-2,0,Buffer,strlen(Buffer),0,0);     /* And display on second line */
}


/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
BOOL MainLoop(int key)
/*
 * Handle continuous operation of game.
 * Returns TRUE if done.
 */
{
  int Move;

  switch (key) {
     case 0:         /* 0 indicates no keystroke, just time ebbing away */
        if (Opponent && !MyTurn) {       /* If our opponent's turn, try to get their moves*/

          if (!ReceiveMove()) {     /* Returns FALSE to remotely terminate */
            state = ATTRACT;
            app_display();
            return TRUE;
            }
          break;             /* don't flash the cursor--flash indicates current turn */
          }

        if (cursorwait++>100) {        /* Only do this every 100th time */
          cursorwait = 0;
          cursorstate = !cursorstate;  /* Blink cursor while waiting */
          ShowCursor(cursorstate);
          }
        break;

     case ENTER:       /* Enter a line--do the logic */
     case SPACE_BAR:
          if (Opponent && !MyTurn) {
            m_asound(0);    /* Make a sound pattern--values from 0 to 6 */
            break;
            }


          Move = EnterMove(cursorx,cursory,cursorside,YOU_BIT);
          if (Opponent)  {
            SendMove(cursorx,cursory,cursorside);   /* Send the turn if we've gotta */
            }

          switch (Move) {
            case 0:
                m_beep();
                break;   /* If no move possible, make a error sound */
            case 1:
                if (Opponent) {            /* Playing two player-- */
                  TransferTurn();          /*  transfer the turn */
                  }
                else {
                  Move = ComputerMove();   /* Now it's the computer's turn! */
                  }
                break;
            case 2:
                m_thud();    /* Finished a box, player can keep moving */
                ShowScore();
                m_disp(-3,0, s(CONTINUEs), strlen(s(CONTINUEs)), 0, 0);
                break;
            }

          if (Move==3) {            /* Is game over ? */
            if (score>0)
              ESCmsg(s(YOUWONs),FALSE);
            else if (score<0)
              ESCmsg(s(IWONs),FALSE);
            else
              ESCmsg(s(TIEs),FALSE);

            state = ATTRACT;
            app_display();
            return TRUE;
            }

          ShowCursor(TRUE);       /* Redisplay the cursor after the move */
          break;

     case UP_KEY:
     case '8':
          ShowCursor(FALSE);
          IncCursor(0,-1);
          ShowCursor(TRUE);
          break;
     case DOWN_KEY:
     case '2':
          ShowCursor(FALSE);
          IncCursor(0, 1);
          ShowCursor(TRUE);
          break;
     case RIGHT_KEY:
     case '6':
          ShowCursor(FALSE);
          IncCursor(1,0);
          ShowCursor(TRUE);
          break;
     case LEFT_KEY:
     case '4':
          ShowCursor(FALSE);
          IncCursor(-1,0);
          ShowCursor(TRUE);
          break;

     case F1:                     /* Save game */
          AllowUserSave();
          break;

     case F10:                    /* Abort game */
          if (ESCmsg(s(ABORTGAMEs),TRUE)=='Y')  {

            /* If playing a human, tell them we're quitting */
            if (Opponent)  SendChar(EOF,FALSE);

            state = ATTRACT;
            app_display();
            return TRUE;
            }
          break;
     }

  return FALSE;
}


