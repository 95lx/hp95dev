/*
 * Module smtime.h  Version 1.1  Date 06/19/91  Time 17:18:54
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

#ifndef M_TIME
#define M_TIME

typedef struct {
	char	dt_order;		/* month-day-year order, see below */
	char	dt_dsep;		/* date separator */
	char	dt_tsep;		/* time separator */
	char	dt_24hr;		/* non-zero => 24-hour time */
} DTINFO;

#define DATE_MDY		0
#define DATE_DMY		1
#define DATE_YMD		2

typedef struct {
	int	dt_year;
	char	dt_month;
	char	dt_date;
	char	dt_day;
	char	dt_hour;
	char	dt_minute;
	char	dt_second;
	char	dt_hundreth;
} DTM;


typedef struct {
	char	t_hundreth;		/* 0 - 99 */
	char	t_second;		/* 0 - 59 */
	char	t_minute;		/* 0 - 59 */
	char	t_hour;			/* 0 - 23 */
} TIME;


#define ALARM_MSG_LEN		40
#define ALARM_EXTRA_LEN		4

typedef struct {
	char	a_hour;			/* time of alarm, hours */
	char	a_minute;		/* time of alarm, minutes */
	char 	a_second;		/* time of alarm, seconds */
	char	a_pad;			/* supplied by caller */
	int 	a_interval;		/* reschedule interval (seconds) */
	char	a_use_seconds;		/* are seconds signficant */
	char	a_sound;		/* alarm sound */
	char	message[ALARM_MSG_LEN];	/* message displayed when alarm goes off */
	char	owner;			/* task id of the owner */
	char	special;		/* app's own use for sub-class */
	char	extra[ALARM_EXTRA_LEN];	/* app's own use for spcific data */
} ALARM;

#define ALARM_FORCE	 	1
#define ALARM_IF_EARLY	 	2
#define ALARM_IF_CLEAR	 	4

#define DATE_ONLY	0
#define TIME_ONLY	1
#define DATE_N_TIME	2
#define DAY_N_DATE	3
#define SUPP_AMPM	0x10
#define SUPP_SECOND	0x20
#define SHOW_YEAR	0x40
#define FOUR_DIGIT_YEAR	0x80



typedef struct {
        DTM     oldtime;
        DTM     newtime;
} TIMECHANGE;

#endif
