/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrCB.h
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for XbrCB.c
 *
 */

#ifndef XBRLIB_XBRCB_H
#define XBRLIB_XBRCB_H		1

extern void XbrCBExitApplication(Widget, XtPointer, XtPointer);
extern void XbrCBDestroyWidget(Widget, XtPointer, XtPointer);
extern void XbrCBPassword(Widget, XtPointer, XtPointer);
extern void XbrCBPopdownShell(Widget, XtPointer, XtPointer);
extern void XbrCBRefresh(Widget, XtPointer, XtPointer);
extern void XbrCBUnmanageWidget(Widget, XtPointer, XtPointer);

#endif
