/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 * FILE NAME:	fname.h
 * CREATED:	Mon Oct 25 GMT 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for fname.c
 *
 */

#ifndef	COMMON_FNAME_H
#define	COMMON_FNAME_H	1

extern void fname_change(char *, char *, char *);
extern char *fname_basename(char *);
extern char *fname_dirname(char *);
extern char *fname_resolve(char *, ...);

#define	fname_c2g(filename)	fname_change(filename, ".Z", ".gz")
#define	fname_g2c(filename)	fname_change(filename, ".gz", ".Z")

#define	MAXFNAMELEN		256

#endif
