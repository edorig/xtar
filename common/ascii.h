/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	ascii.h
 * CREATED:	Mon Oct 25 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for ascii.c
 *
 */

#ifndef	COMMON_ASCII_H
#define	COMMON_ASCII_H	1

#include <time.h>

extern char *ascii_size(int);	     /* Convert size into K,MBS            */
extern char *ascii_mins(float);      /* Convert seconds to hours:mins:secs */
extern char *ascii_date(time_t);     /* Convert seconds to Day Year        */

#endif
