/*
 * Module fmenu.h  Version 1.2  Date 07/24/91  Time 14:21:09
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/* FMENU.H  - File menu, for selecting file by pointing */

/* Return values from FMENU_KEY

	RET_UNKNOWN			key was unknown by FMENU
	RET_OK				key was processed by FMENU, just call FMENU_DIS
	RET_BAD				key is known by FMENU, but was invalid (pgdn off list)
	RET_REDISPLAY		redisplay application area before calling FMENU_DIS
	RET_ACCEPT			user confirmed, the filename is in the EDIT_BUFFER
	RET_ABORT			user aborted operation
*/

                                             
/* define symbols */

#define FNAM_LEN  13 /* each file name is allocated a space of 13 chars long */
#define FILEINFO_ATTR_DIR	0x10

/* define return codes */

#define	RET_BADFILE		-5
#define	RET_BADDIR		-4
#define	RET_BADDRIVE	-3
#define	RET_UNKNOWN		-2
#define	RET_BAD			-1
#define	RET_OK			0
#define	RET_REDISPLAY	1
#define	RET_ACCEPT		2
#define	RET_ABORT		3


/* file info structures created in FM_BUFFER */
#pragma pack(1)
typedef	struct {
	char fi_attr;				/*  file attribute  */
	int fi_time;				/*  time modified  */
     	int fi_date;				/*  date modified  */
	long fi_size;  				/*  file length  */
     	char fi_name[13];			/*  file name  */
} FILEINFO;                                  
#pragma pack()


/* define the menu structure */
typedef struct {
	char far *fm_path;		/* base directory name C:\DATA\ */
	char far *fm_pattern;		/* file pattern, e.g.  *.WK1    */
	FILEINFO far *fm_buffer;	/* workspace  for file list (hold finfos) */
	int fm_buf_size;		/* size of the buffer in bytes */
     	int fm_startline,fm_startcol;	/* starting row,col */
	int fm_numlines, fm_numcols;	/* number of lines and columns */
	int fm_filesperline;		/* nuumber of files displayed across */
/* -- Initted in service -- */
	int fm_firstedit;			/* 0 if first edit char, else multi line */
	int fm_filesinbuf;		/* number of files in list */
	int fm_maxfinbuf;		/* maximum number of files buffer holds */
	int fm_topfile;			/* file at top of list */
	int fm_curselect;		/* index of file to highlight */
	int fm_oldselect;		/* index of file to un-highlight */
	int fm_focus;			/* 1 = fmenu, 2 = edit */
} FMENU;

/* System defaults for above
	fm_path				Must be supplied
	fm_pattern			Must be supplied
	fm_buffer			Give me atleast 1024 bytes
	fm_buf_size			The actual size in bytes
	fm_startline		-2
	fm_startcol			0
	fm_numlines			13
	fm_numcols			40
	fm_filesperline	3

	all others are handled by init service
*/


/* manifests for FOCUS value in FMENU */
#define	FOCUS_FMENU	1
#define	FOCUS_EDIT 2

/* Function entry points for FMENU
 *
 *	FMENU_INIT(FIRST_ARG dummy,
 *            FMENU far *fmenu,
 *            EDITDATA far *e,
 *            char far *ini_buf,
 *            int ini_len, max_len, dline, dcol);
 *
 * FMENU_DIS( FIRST_ARG dummy,
 *            FMENU far *fmenu;
 *            EDITDATA far *e);
 *
 * FMENU_KEY( FIRST_ARG dummy,
 *            FMENU far *fmenu;
 *            EDITDATA far *e
 *            int key);
 *
 * FMENU_OFF( FIRST_ARG dummy,
 *            FMENU far *fmenu;
 *            EDITDATA far *e);
 */

