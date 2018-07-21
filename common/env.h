/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	env.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for env.c
 *
 */

#ifndef COMMON_ENV_H
#define COMMON_ENV_H		1

#include <limits.h>

extern int env_osrelease();
extern int env_inpath(char *);
extern char *env_realname();
extern char *env_hostname();
extern char *env_username();
extern char *env_email();

#define	MAXUSERNAMELEN	10
#define	MAXGCOSLEN	256
#define	MAXEMAILLEN	MAXUSERNAMELEN+MAXHOSTNAMELEN+1

#define	running9()	(env_osrelease() == 9)
#define	env_isroot()	(!strcmp("root", env_username()))

#ifdef	_USE_GETWD
#define	env_cwd(buffer, length)	getwd(buffer)
#else
#define	env_cwd(buffer, length)	getcwd(buffer, length)
#endif

#endif
