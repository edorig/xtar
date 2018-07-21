/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	radnum.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION: Header file for nums.c
 *
 */

#ifndef COMMON_RANDNUM_H
#define COMMON_RANDNUM_H	1

#define num_roundup(x,y)   ((((x)+((y)-1))/(y))*(y))

extern int num_random();
extern long num_decimal(char *, int);

#endif
