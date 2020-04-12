/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	magic.h
 * CREATED:	Mon Oct 25 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for magic.c
 *
 */

#ifndef COMMON_MAGIC_H
#define COMMON_MAGIC_H	1

#define	MAGIC_JPEG		1
#define	MAGIC_GIF		2
#define	MAGIC_COMPRESS		3
#define	MAGIC_GZIP		4
#define	MAGIC_TROFF		5
#define	MAGIC_POSTSCRIPT	6
#define	MAGIC_BZIP		7
#define MAGIC_LZMA              8

extern int magic_ftype(char *, int);

#define	is_jpeg(filename)	magic_ftype(filename, MAGIC_JPEG)
#define	is_gif(filename)	magic_ftype(filename, MAGIC_GIF)
#define	is_compressed(filename)	magic_ftype(filename, MAGIC_COMPRESS)
#define	is_gzipped(filename)	magic_ftype(filename, MAGIC_GZIP)
#define	is_bzipped(filename)	magic_ftype(filename, MAGIC_BZIP)
#define is_xzipped(filename)    magic_ftype(filename, MAGIC_LZMA)
#define	is_troff(filename)	magic_ftype(filename, MAGIC_TROFF)
#define	is_postscript(filename)	magic_ftype(filename, MAGIC_POSTSCRIPT)

#endif
