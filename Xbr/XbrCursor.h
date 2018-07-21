/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrCursor.c
 * CREATED:	Thu Oct 28 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: Cursor handling routines. This is not very good at all and
 *              I'm sure there is a better way of doing this, but as the FAQ
 *              does not suggest many easier ways, here goes...
 *
 */

#ifndef	XBRLIB_XBRCURSOR_H
#define	XBRLIB_XBRCURSOR_H		1

extern void XbrCursorAdd(Widget, int);
extern void XbrCursorDelete(Widget);
extern void XbrCursorAddCB(Widget, XtPointer, XtPointer);
extern void XbrCursorDeleteCB(Widget, XtPointer, XtPointer);
extern void XbrCursorWatch(Widget, int);
extern void XbrCursorSecure(Widget, int);
extern void XbrCursorHand(Widget, int);

#define	XbrCursorBusy(w)		XbrCursorWatch(w, 1)
#define	XbrCursorBusySecure(w)		XbrCursorSecure(w, 1)
#define	XbrCursorReset(w)		XbrCursorWatch(w, 0)

#define	XbrGRAB		1
#define	XbrNOGRAB	0

#endif
