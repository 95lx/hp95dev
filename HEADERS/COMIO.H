/*
 * Module comio.h  Version 1.2  Date 06/21/91  Time 11:38:42
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

#ifndef _COMIO_H_
#define _COMIO_H_

typedef int com_handle;

#define E_GOOD       0
#define E_REOPEN    -1
#define E_IVCHN     -2
#define E_IVOPR     -3
#define E_UNSUP     -4
#define E_NOFIT     -5
#define E_BUFOV     -6
#define E_NOMEM     -7
#define E_UNOPEN    -8
#define E_CONECT    -9
#define E_EMPTY    -10
#define E_OVERN    -11
#define E_PARITY   -12
#define E_FRAME    -13
#define E_TIMEOUT  -14
#define E_BUSY     -15

#define         COM_QUERY    0

#define COM_LINE_1      1
#define COM_LINE_2      2
#define COM_LINE_3      3
#define COM_LINE_4      4

#define COM_MDM_NONE            1
#define COM_MDM_HAYES   2

#define COM_ANS_WAIT            1       /* Wait till phone answered          */
#define COM_ANS_NOWAIT  2       /* Put phone in answer mode          */
#define COM_ANS_STOP            3       /* Turn off answer mode (default) */

#define COM_RESET_LINE  0x01    /* Reset line */
#define COM_RESET_TXB   0x02    /* Flush transmit buffer */
#define COM_RESET_RXB   0x04    /* Flush receive buffer */
#define COM_RESET_MODEM 0x08    /* Reset modem */
#define COM_RESET_RXFLOW 0x10   /* Reset receiver's ^S state, if set. */
#define COM_RESET_TXFLOW 0x20   /* Reset transmitter's ^S state, if set. */

#define         COM_CTL_WHOLE   0x01    /* Send partial buffer */
#define         COM_CTL_SETRCV  0x02    /* Turn rcvr on after sending */

#pragma pack(1)
typedef struct {
        char    Dial;
        int     Baud;
        char    Parity;
        char    Stop;
        char    Data;
        char    XonXof;
        char    InfraRed;
        char    Duplex;
        char    Echo;
} com_settings;
#pragma pack()

/* Values for Dial field in com_settings struct */
#define COM_DIAL_TONE   'T'     /* Go into TONE dial mode */
#define COM_DIAL_PULSE  'P'     /* Go into PULSE dial mode */

/* Values for Baud field in com_settings struct */
#define COM_BR_110      0x417
#define COM_BR_150      0x300
#define COM_BR_300      0x180
#define COM_BR_600      0x0C0
#define COM_BR_1200     0x060
#define COM_BR_2400     0x030
#define COM_BR_4800     0x018
#define COM_BR_9600     0x00C
#define COM_BR_19200    0x006
#define COM_BR_38400    0x003
#define COM_BR_57600    0x002
/* Values for Parity field in com_settings struct.  */
#define COM_PTY_NO              0x00    /* No parity     */
#define COM_PTY_ODD             0x08    /* Odd parity    */
#define COM_PTY_EVEN            0x18    /* Even parity */
#define COM_PTY_MARK            0x28    /* High parity */
#define COM_PTY_SPACE   0x38    /* Low parity  */

/* Values for Stop field in com_settings struct. */
#define COM_STOP_1      0x00    /* One stop bit  */
#define COM_STOP_2      0x04    /* Two stop bits */

/* Values for Data field in com_settings struct. */
#define COM_DATA_7              0x02    /* Seven data bits */
#define COM_DATA_8              0x03    /* Eight data bits */

/* Values for XonXof field in com_settings struct. */
#define COM_XON_OFF     1       /* Disable XOF/XON */
#define COM_XON_ON      2       /* Turn on XOF/XON */
#define COM_XON_ANY     4       /* Turn on XOF/ANY */
#define COM_ENQACK      8       /* HP's ENQ/ACK protocol for old equipment */
                /* Note that both XON/XOF and ENQ/ACK may */
                /* be specified.  These two, COM_ENQACK */
                /* and COM_XON_ON, are the only two bits that */
                /* may be on simultaneously.  If any others */
                /* are on simultaneously, then results are not */
                /* guaranteed. */


/* Values for InfraRed field in com_settings struct */
#define COM_IR_OFF      1       /* Turn off infra-red/on modem/rs-232 */
#define COM_IR_ON               2       /* Turn on infra-red/off modem/rs-232 */

/* Values for Duplex field in com_settings struct. */
#define COM_DUP_HALF    1
#define COM_DUP_FULL    2

/* Values for Echo field in com_settings struct. */
#define COM_ECHO                1
#define COM_NOECHO      2

/* Values for Compression field in com_settings struct. */
#define COM_CMP_OFF     1       /* Turn compression off */
#define COM_CMP_G31D    2       /* Group 3, 1 dimensional (FAX uses this) */

#endif
