/*
 * Module edit.h  Version 1.2  Date 06/21/91  Time 11:53:54
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/* EDIT.H  - edit manager for SINGLE line editing */

/* ASSUME: the maximum number of cols in an edit area is 80 */


#ifndef EDITDATADEF

#ifndef MDITDATADEF

#ifndef EOL
/*
 *  multi-line edit data structure
 *
 */

typedef struct {
	char    far *m_buffer;    /*  user-supplied edit buffer  */
	int     m_length;         /*  length of buffer  */
	int     m_pos;            /*  current cursor position  */
	int     m_row, m_col;     /*  location of edit area  */
	int     m_nrows, m_ncols; /*  dimensions of edit area  */
	int	m_yoff,m_disprows;/*  current top row, number of rows displayable */
	char    m_ccol;           /*  cursor column  */
	char    m_modified;       /*  1 -> buffer has been modified  */
        char    m_xoff;           /*  1st disp. col (for ticker fields only)*/
        char    wrapflag;         /*  1 -> wordwrap enabled,-1 ticker field */
	int     far *m_line;      /*  pointer to array used for line starts */
				  /*     must be at least m_nrows+1 long */
	char 	markon;		  /* currenty marking? */
	char	spec_flag;
	int	markst;		  /* offset of start of mark, inclusive */
	int	markend;	  /* offset of end of mark, inclusive */
        int     m_dispcols;       /*  displayable columns */
} MDITDATA;


#define EOL       0
#endif

#define MDITDATADEF
#endif

/* EDIT_KEY returns "1" if user has accepted the edit buffer contents
     and returns "0" otherwise */

typedef struct {
	int edit_length; 	/* current length of the edit buffer */
	char first_time;      	/* flag for special processing on first char */
	char spec_flags;	/* bit 0 is tab handling */
     	int prompt_window;      /* whether this belongs to the prompt window */
	char far *message_line; /* the top line message for prompt */
	int message_line_length;/* length of same */
	char far *prompt_line;  /* second line of prompt window */
	int prompt_line_length; /* length of same */
	char edit_buffer[80];   /* work space for the buffer */
	int  line_array[2];	/* line array needed by mdit */
	MDITDATA mdit;          /* multi-line edit structure */
	int e_dispcols;
} EDITDATA;



#define EDITDATADEF 0
#endif
