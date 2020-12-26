/*
 * Module menu.h  Version 1.1  Date 06/19/91  Time 17:18:32
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/* MENU.H  - menu definitions file for users of the menu system */
#ifndef	M_MENU
                                              
/* All menu operation calls return an error code (0=OK). 
     Any return values are via reference parameters. */
                 
/* ASSUME there is a maximum number of keywords. */
#define   MAX_KWDS    20

/* ASSUME there is a maximum number of lines in a menu */
/*	two menu lines, plus long prompts */
#define   MAX_MENU     3

/* ASSUME there is a maximum window width of 80 */
#define   MAX_MWID     80

/* MENU_KEY returns the index of the selected keyword.  It returns
     a negative number if selection is not complete. */


/* define the menu structure */
typedef struct {
     /* define the menu display storage area */
     /* the intent is to have one string per display line to make display 
          management easier */
     char menu_text [MAX_MENU][MAX_MWID];
     int menu_count;       /* as well as the number of keywords */

     /* define currently highlighted keyword */
     int menu_highlight;                /* index of currently hightlighted */
                                        /* special value of -1 for no highlight */

     /* flag indicating special mode with single prompt on top line */
     int menu_tprompt;                  /* 0 ==> no */

     /* define the menu information table */
     char menu_line   [MAX_KWDS];    /* which line of menu this word is on */
     char menu_offset [MAX_KWDS];    /* offset of this keyword in the string */
     char menu_length [MAX_KWDS];    /* length of this keyword */
     char menu_letter [MAX_KWDS];    /* first letter of this keyword */

    /* store the pointers to the long prompts */
    unsigned menu_prompt[MAX_KWDS];	/* DS-relative offsets... */

     } MENUDATA;


#define	M_MENU
#endif
