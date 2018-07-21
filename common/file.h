/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	file.h
 * CREATED:	Tue Oct 26 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for file.c.
 *
 */

#ifndef	COMMON_FILE_H
#define	COMMON_FILE_H		1

#include <time.h>

extern int file_exists(char *);        /* Does file exist?             */  
extern int file_copy(char *, char *);  /* UNIX cp command              */
extern char *file_load(char *, int *); /* Load whole file into memory  */
extern char *file_uncompress(char *);  /* Uncompress a file            */
extern char *file_stdin();             /* Load stdin to memory         */
extern int file_stamp(char *, time_t); /* Stamp a file with data       */
extern int file_remove(char *);        /* Remove a file or directory   */

#endif
