/*
 * Module settings.h  Version 1.1  Date 06/19/91  Time 17:18:42
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/* volume level range */

#define VOLLEVOFF       0x0
#define VOLLEVHI        0x4


/* contrast level range */

#define CONTLEV0	0x0
#define CONTLEV15	0xf


/* date formats */

#define DATEFOR1	0x0		/* dd-mmm-yy */
#define DATEFOR2	0x1		/* dd-mmm */
#define DATEFOR3	0x2		/* mmm-yy */
#define DATEFOR4a	0x3		/* mm/dd/yy */
#define DATEFOR4b	0x4		/* dd/mm/yy */
#define DATEFOR4c	0x5		/* dd.mm.yy */
#define DATEFOR4d	0x6		/* yy-mm-dd */
#define DATEFOR5a	0x7		/* mm/dd */
#define DATEFOR5b	0x8		/* dd/mm */
#define DATEFOR5c	0x9		/* dd.mm */
#define DATEFOR5d	0xa		/* mm-dd */


/* week starts */

#define WEEKSTARTSUN	0x0
#define WEEKSTARTMON	0x1


/* time formats */

#define TIMEFOR1	0x0		/* HH:MM:SS am/pm */
#define TIMEFOR2	0x1		/* HH:MM am/pm */
#define TIMEFOR3a	0x2		/* HH:MM:SS */
#define TIMEFOR3b	0x3		/* HH.MM.SS */
#define TIMEFOR3c	0x4		/* HH,MM,SS */
#define TIMEFOR3d	0x5		/* HHhMMmSSs */
#define TIMEFOR4a	0x6		/* HH:MM */
#define TIMEFOR4b	0x7		/* HH.MM */
#define TIMEFOR4c	0x8		/* HH,MM */
#define TIMEFOR4d	0x9		/* HHhMMm */


/* punctuation sets (follows 1-2-3)
 *   (decimal, argument, thousands)
 */

#define PUNCFORA	0x0		/* (.,,) */
#define PUNCFORB	0x1		/* (,..) */
#define PUNCFORC	0x2		/* (.;;) */
#define PUNCFORD	0x3		/* (,;.) */
#define PUNCFORE	0x4		/* (., ) */
#define PUNCFORF	0x5		/* (,. ) */
#define PUNCFORG	0x6		/* (.; ) */
#define PUNCFORH	0x7		/* (,; ) */


/* languages -- only US is defined; it is the default.  All
 * others are found at run-time by the presense of QBTASK??.RI
 * resource images, where ?? used to indicated the language 
 */


#define LANG_US	0x5355		/*  'US', bytes swapped */



/* collating sequences */

#define NUMBERSFIRST	0x0
#define LETTERSFIRST	0x1
#define ASCII		0x2


/* character sizes */

#define CHR7		0x0
#define CHR8		0x1



/* baud rates, Max baud for non-infrared selections if 19200
 * max for IR link is 1200
 */

#define MENU_CNT_IR	3
#define MENU_CNT_NOT_IR	6

#define BAUD300 	0x0
#define BAUD1200	0x1
#define BAUD2400	0x2
#define BAUD4800	0x3
#define BAUD9600	0x4
#define BAUD19200	0x5

#define MAX_IR_BAUD	BAUD2400


/* interface options for printers */

#define INTERFCOM1	0x0
#define INTERFCOM2	0x1
#define INTERFINFR	0x2
#define INTERFLPT1	0x3


/* supported printers -- this is static because the printer
 * drivers are in ROM, no RAM drivers supported
 */

#define EP_FX80 	0x0
#define HP_LJ		0x1
#define IBM_PP		0x2



/* currency prefix/suffix selection */

#define CUR_PREF	0x0
#define CUR_SUFF	0x1



/* keyboard selections */

#define KEYB_BEL	0x0001		/* Belgium */
#define KEYB_FCA	0x0002		/* French Canadian */
#define KEYB_DEN	0x0004		/* Denmark */
#define KEYB_FIN	0x0008		/* Finland */
#define KEYB_FRA	0x0010		/* French */
#define KEYB_GER	0x0020		/* Finland */
#define KEYB_ITA	0x0040		/* Italy */
#define KEYB_NED	0x0080		/* Netherlands */


#define KEYB_NOR	0x0100		/* Norway */
#define KEYB_POR	0x0200		/* Portugal */
#define KEYB_SPA	0x0400		/* Spain */
#define KEYB_SWE	0x0800		/* Sweden */
#define KEYB_SFR	0x1000		/* Swiss French */
#define KEYB_SGR	0x2000		/* Swiss German */
#define KEYB_UNK	0x4000		/* United Kingdom */
#define KEYB_USA	0x8000		/* USA */




#define MAX_MACRO_LEN	256


/* date parsing rules */

#define DR_DMY_LIM	1
#define DR_MDY_LIM	2
#define DR_YMD_LIM	3
#define DR_DMYO_LIM     4
#define DR_MDYO_LIM     5

/* or this bit onto patterns above to get any date */
#define DR_ANY_YEAR	8


/* time parsing rules */

#define TM_H_M_S_P	1
#define TM_H_M_S_24	2
#define TM_HM_S_24	3
#define TM_H_M_S_C_24	4
#define TM_H_M_P        5
#define TM_H_M_24       6
#define TM_HM_24        7

/* Code Page Settings */
#define CDPG_850	1
#define CDPG_437  2

#define NAMEBUFLEN	30
#define TITLEBUFLEN	30
#define COMPANYBUFLEN	28

/*  settings */
typedef struct {
	int	Country;	/* */
	int	Volume; 	/* 1 speaker volume level 0ffh for off and 0-3 */
	int	Contrast;	/* 2 contrast level 1-5 */
	int	WeekStart;	/* 3 week starts SUNDAY or Monday */
	int	Punctuation;	/* 4 Punctuation format */
	int 	Language;	/* two character code for current language */
	int	DateFormat;	/* 6 current date format */
	int	TimeFormat;	/* 7 current time format */
	int	Sort;		/* 8 collating sequence */
	char	PicFile[80];	/* picture file name */
	char	Name_Buff[NAMEBUFLEN];	
	char	Title_Buff[TITLEBUFLEN]; 
	char	Company_Buff[COMPANYBUFLEN]; 
	int 	LanguageCount;
	int	LanguagesAvail[6];	/* MAX_LANG */
	char	LanguageMenu[66];	

	char	DateSeparator[2]; /* date separator  can be /-. */
	char	TimeSeparator[2]; /* time separator can be :., or h (for hms) */
	char	DateOrder;	/* */
	char	Time24;		/* use 24 hour time? */

	char	Currency[16];   /* currency string */
	int	CurrencyFix;   /* prefix or sufix */

	int	Keyboard;
	int	PrBaud; 	/* printer baud rate */
	int	PrDriver;	/* epson, hp, etc, by code */
	int 	PrInterface;	/* com 1, etc  */
	int	SystemRupt;		/* sysmgr interupt number - 60 default */
	int	CodePage;		/* for PC based - 437 or 850 based */
	int	ExitKeyCode;	/* for PC based - active exit key */
	int	MenuKeyCode;	/* for PC based - active menu key */
	int	CharKeyCode;	/* for PC based - active CHAR key toggle */
	int	Alarm[6];
} SETTINGS;


typedef struct {
	char	MacroFK;	   /* MacroFunctionKey Number 1-10 0 for not used */
        unsigned char MacroSize;   /* elements in the macro */
        char    CommentString[38]; /* MacroFunctionKey's Comment line */
	unsigned int MacroBody[MAX_MACRO_LEN];
} MACROHD;
