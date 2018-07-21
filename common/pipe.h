/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	pipe.h
 * CREATED:	Mon Nov  1 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Header file for pipe.c.
 *
 *
 */

#ifndef	COMMON_PIPE_H
#define	COMMON_PIPE_H		1

extern int pipe_open(char *);
extern int pipe_seek(int, int);
extern int pipe_read(int, char *, int);

#define	pipe_close(fp)	close(fp)

#endif
