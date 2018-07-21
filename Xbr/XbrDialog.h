/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrDialog.h
 * CREATED:	Thu Oct 28 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Header file for XbrDialog.c
 *
 */

#ifndef XBRLIB_XBRDIALOG_H
#define XBRLIB_XBRDIALOG_H		1

#define	XbrDialogFSB(w)	XbrDialogFSBox(w, NULL, NULL)

extern void XbrDialogError(Widget, ...);
extern int XbrDialogQuestion(Widget, char *, char *, ...);
extern char *XbrDialogFSBox(Widget, char *, char *);
extern char *XbrDialogPrompt(Widget, ...);

#endif
