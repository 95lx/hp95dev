/*
 * Module cbcodes.h  Version 1.1  Date 07/24/91  Time 16:16:59
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
cbcodes.h

Clipboard status codes; negative values used because the "get clipboard
data" routine needs to use positive values for partial completion status.

****************************************************************/

#ifndef	CBCODES

#define	CB_OK		0		/* All's well with the world */

#define	CB_LOCKED	-1		/* Clipboard access denied */
#define CB_NOT_OPEN	-2		/* Clipboard not formally open */
#define CB_ALLOC	   -3		/* Heap allocation failure */
#define CB_EXISTS	   -4		/* Representation already exists */
#define	CB_REP_OPEN	-5		/* A Representation is open */
#define	CB_NO_REP	-6		/* No representation open */
#define	CB_NO_SUCH	-7		/* No such representation */
#define	CB_BOUNDS	-8		/* Transfer request out of bounds */

/****************************************************************
Maximum size of a clipboard author or representation name
****************************************************************/
#define	MAX_CB_NAME	48

/****************************************************************
Declarations, of the strings only, of certain representation type names
****************************************************************/
#define	CB_REP_TEXT		"TEXT"
#define	CB_REP_KEYSTROKE	"KEYSTROKE"

/****************************************************************
Declare a macro which, when invoked, will define a string constant
identifying the standard TEXT data type.
****************************************************************/
#define DECLARE_TEXT	char TEXTTYPE[] = "TEXT"

#define	CBCODES
#endif
