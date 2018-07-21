/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	stat.h
 * CREATED:	Mon Oct 25 11:16:39 GMT 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION: Header file for file.c
 *
 */

#ifndef	COMMON_FILE_H
#define	COMMON_FILE_H

#include <time.h>          /* For time_t */

extern int stat_size(char *);		/* File size in bytes     */
extern time_t stat_atime(char *);	/* Last access time       */
extern time_t stat_mtime(char *);	/* Last modification time */
extern time_t stat_ctime(char *);	/* Last status change     */
extern ushort stat_mode(char *);	/* Mode of file           */
extern uid_t stat_uid(char *);		/* UID of owner           */
extern gid_t stat_gid(char *);		/* GID of owner           */
extern int stat_isdir(char *);          /* Is file a directory?   */
extern int stat_islink(char *);         /* Is file a link?        */

#endif
