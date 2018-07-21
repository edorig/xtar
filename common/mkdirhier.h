/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	mkdirhier.h
 * CREATED:	Mon Feb  7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Header file for mkdirhier.c.
 *
 */

#ifndef	COMMON_MKDIRHIER_H
#define	COMMON_MKDIRHIER_H		1

#include <sys/stat.h>

extern int mkdirhier(char *, mode_t);

#endif
