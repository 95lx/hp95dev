/*
 * Module bridge.h  Version 1.2  Date 06/21/91  Time 09:01:44
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

#define BRIDGE_TEST 	0
#define BRIDGE_GETRANGE 1
#define BRIDGE_GETRANGE_ADDR 2
#define BRIDGE_SETRANGE_ADDR 3
#define BRIDGE_GETRANGE_DATA 4
#define BRIDGE_SETRANGE_DATA 5
#define BRIDGE_RECALC 6
#define BRIDGE_GET_CURSOR 7
#define BRIDGE_SET_CURSOR 8
#define BRIDGE_REDISPLAY 9
#define BRIDGE_CELLTYPE 10
#define BRIDGE_CALCTYPE 11


typedef struct BRIDGE_BP {
	int bpb_funcode;
	int bpb_retcode;
	char bpb_rangename[16];
	int bpb_startcol;
	int bpb_startrow;
	int bpb_endcol;
	int bpb_endrow;
	int bpb_order;
	int bpb_bufsize;
	char near * bpb_buffer;
} BRIDGE_BP;
