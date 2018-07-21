/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrHelp.h
 * CREATED:	Mon Oct 24 1994
 * AUTHOR:	Rik Turnbull <rik@stella-consulting.ltd.uk>
 * DESCRIPTION:	Header file for XbrHelp.c
 *
 */

#ifndef	XBR_XBRHELP_H
#define	XBR_XBRHELP_H

#include <Xm/Xm.h>

typedef struct _XbrHelpData {
    char *name;
    char *text;
} XbrHelpData;

extern void XbrHelpSet(Widget, XtEventHandler, XbrHelpData[], int);

#endif
