/*
 * Module fileio.h  Version 1.1  Date 06/19/91  Time 17:17:44
*/

/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/* fileio.h */

#ifndef	M_FILEIO

/* file attribute settings */

#define FA_NORMAL	0
#define FA_READONLY	1
#define FUN_OPEN      0x3d
#define FUN_CREATE    0x3c
#define MODE_NO_TRUNC 0x0
#define MODE_TRUNC   0x1
#define MODE_RO        0
#define MODE_RW        2


/*
 *  open file control block
 *
 *  The caller must not modify this block except by Mosaic system calls.
 *
 *  The "physical" seek address is our copy of the actual DOS file offset.
 *  If we aren't sure of this value, it is -1.
 *
 *  The "logical" seek address is the file offset of the block currently
 *  in the buffer (if the buffer contents are valid).
 *
 *  The "virtual" seek address is the effective file offset as implemented
 *  by this buffered I/O package.
 */

#define FBUFSIZ   512

typedef struct {
      int         f_handle;         /*  DOS file handle  */
      int         f_flag;           /*  flag word -- see below  */
      long        f_pseek;          /*  physical seek address  */
      long        f_lseek;          /*  logical seek address  */
      long        f_vseek;          /*  virtual seek address  */
      int         f_len;            /*  length of valid portion of buffer  */
      char        f_buf[FBUFSIZ];   /*  I/O buffer  */
} FILE;



typedef struct {
      int         f_handle;         /*  DOS file handle  */
      int         f_flag;           /*  flag word -- see below  */
      long        f_pseek;          /*  physical seek address  */
      long        f_lseek;          /*  logical seek address  */
      long        f_vseek;          /*  virtual seek address  */
      int         f_len;            /*  length of valid portion of buffer  */
} NBFILE;


/*  flags  */
#define VALID     01                /*  buffer contents are valid  */
#define DIRTY     02                /*  buffer must be written  */
#define NOBUF     04                /*  force unbuffered I/O  */
#define DEVICE    08                /*  file is a device  */

/*  maximum file name length  */
#define MAXPATH   63
#define MAXNAM    (3+MAXPATH+1+8+1+3+1)     /*  d:\path\name.ext<nul>  */

/*
 *  pattern-match control block
 *
 */

typedef struct {
      char        m_reserved[21];   /*  data transfer area  */
      char        m_attr;                 /*  file attribute  */
      int         m_time;                 /*  time modified  */
      int         m_date;                 /*  date modified  */
      long        m_size;                 /*  file length  */
      char        m_name[13];             /*  file name  */
      char        m_path[MAXNAM];   /*  full path name  */
      char        m_ofs;            /*  offset of last portion of path name  */
      char        m_fn;             /*  DOS call number to use  */
} MATCH;


/*
 *  codes returned by m_ident
 *
 */

typedef
enum m_ident_type {ident_none=0, 
		   ident_file=1, 
		   ident_directory=2, 
		   ident_device=3}
     m_ident_type;

/* Define the file system calls 			*/


typedef
enum m_seek_type {seek_beginning=0, seek_current=1, seek_end=2} m_seek_type;


#define SYSDIR_LEN	16


#define	M_FILEIO
#endif
