/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	str.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for str.c
 *
 */

#ifndef COMMON_STR_H
#define COMMON_STR_H		1

extern int str_numdelims(char *, char);
extern int str_numchars(char *, char);
extern char *str_token(char **, char);
extern char *str_lower(char *);
extern char *str_ending(char *, char *);
extern char *str_casestr(char *, char *);
extern char *str_word(char **);
extern char *str_error(int);

#endif
