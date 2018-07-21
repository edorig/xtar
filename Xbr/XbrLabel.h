/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrLabel.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for XbrLabel.c
 *
 */

#ifndef	XBRLIB_XBRLABEL_H
#define	XBRLIB_XBRLABEL_H		1

extern char *XbrLabelGet(Widget);
extern void XbrLabelSet(Widget, ...);
extern Pixmap XbrLabelSetPixmap(Widget, char *, int, int, char *, int, int);

#endif
