/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	stats.h
 * CREATED:	Tue Jun  7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Header file for stats.c.
 *
 */

#ifndef	XTAR_STATS_H
#define	XTAR_STATS_H

#define	_COMPRESSED_NONE	0
#define	_COMPRESSED_Z		1
#define	_COMPRESSED_GZ		2
#define	_COMPRESSED_BZ		3

typedef struct _tar_stats
{
    char *filename;

    int loaded;

    int type_normal;
    int type_hardlink;
    int type_softlink;
    int type_character;
    int type_block;
    int type_directory;
    int type_unknown;

    int nfiles;

    int size;
    int size_unpacked;

    int compressed;
} XTAR_STATS;

extern void stats_show(XTAR_STATS *);

#endif
