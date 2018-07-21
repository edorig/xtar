/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	cdefs.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Common defs.h file. Contains some #defines that are sometimes
 *              quite useful. Mebee.
 *
 */

#ifndef COMMON_CDEFS_H
#define COMMON_CDEFS_H		1

/* Change case of character */
#define locase(c)		(((c)<='Z')&&((c)>='A')?((c)+32):(c))
#define upcase(c)		(((c)<='z')&&((c)>='a')?((c)-32):(c))

/* Strip last character of string if it matches */
#define	strip_char(string, c)	if(strlen(string) > 0 && \
				string[strlen(string)-1] == c) \
				string[strlen(string)-1] = 0

/* Strip of a new line */
#define strip_nl(string)	strip_char(string, '\n')

/* Number of seconds in a day/week/month */
#define	SECS_DAY	604800
#define	SECS_WEEK	4233600
#define	SECS_MONTH	18748800

#endif
