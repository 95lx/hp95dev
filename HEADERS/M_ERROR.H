/*
 * Module m_error.h  Version 1.1  Date 07/24/91  Time 15:05:57
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
TradeWind/DOS Error Codes

The new error codes are exactly the DOS 3.0/3.1 Extended Error Codes.
TradeWind extends the codes for DOS 2, thus:

    0	No error
01-18	DOS 2.00/2.10 INT 21H errors
19-31	Error mappings for DOS 2.00/2.10 INT 24H (critical) errors
32-88	Errors for DOS 3.00/3.10
****************************************************************/

#ifndef	M_ERROR

/****************	TradeWind's mappings	****************/
#define	TWMinErr	200

#define	TWerrBadName	200		/* invalid file/path name */
#define	TWerrSysDir	201		/* can't find TradeWind system dir */
#define	TWerrAlarm	202		/* too many alarms set */
#define	TWerrFull	203		/* disk full */
#define	TWerrPrint	204		/* print queuing error */
#define	TWerrNetwork	205		/* network error */
#define	TWerrPhysical	206		/* physical I/O error */
#define	TWerrInternal	207		/* internal software error */
#define	TWerrNoFile	208		/* file not found */
#define	TWerrNoPath	209		/* path not found */
#define	TWerrTooMany	210		/* too many open files */
#define	TWerrDenied	211		/* access denied */
#define	TWerrCurDir	212		/* attempt to delete current dir */
#define	TWerrNotSame	213		/* not same device */
#define	TWerrWriteProt	214		/* device is write-protected */
#define	TWerrNotReady	215		/* device not ready */
#define	TWerrNoPaper	216		/* printer out of paper */
#define	TWerrFileExists	217		/* file exists */
#define	TWerrDirEntry	218		/* cannot make directory entry */
#define	TWerrCritError	219		/* Critical error - failure */
#define	TWerrTooManyRed	220		/* too many redirections */
#define	TWerrDupRed	221		/* duplicate redirection */
#define TWerrNoMemory	222		/* out of memory */
#define TWerrDrivLoad	223		/* Error loading driver */
#define TWerrDrivVers	224		/* Bad version of driver */

#define TWerrSwapWrite	225		/* Error swapping to disk */
#define TWerrSwapRead	226		/* Error swapping from disk */

#define	TWMaxErr	226
#define TWMulApps	513		/* more than 1 app open for spawn */

/*
 *  Old Mosaic error codes
 *
 */

#define eMINERR         TWMinErr
#define eMAXERR         TWMaxErr

#define eNOFILE         TWerrNoFile
#define eNOPATH         TWerrNoPath
#define eWPDISK         TWerrWriteProt
#define eDRVNRDY        TWerrNotReady
#define eDEVNRDY        TWerrNoPaper
#define ePHYSIO         TWerrPhysical
#define eACCESS         TWerrDenied
#define eCURDIR         TWerrCurDir
#define eNOTSAME        TWerrNotSame
#define eFULL           TWerrFull
#define eOUTMEM         TWerrNoMemory
#define eALARM          TWerrAlarm
#define eSYSDIR         TWerrSysDir
#define eEXISTS         TWerrFileExists
#define eBADNAME        TWerrBadName


#define	M_ERROR
#endif
