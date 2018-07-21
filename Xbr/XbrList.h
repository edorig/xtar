/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrList.h
 * CREATED:	Wed Oct 27  1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Header file for XbrList.c
 *
 */

#ifndef XBRLIB_XBRLIST_H
#define XBRLIB_XBRLIST_H	1

#define	XbrLIST_SHOWTOP		1
#define	XbrLIST_SHOWBOTTOM	0
#define	XbrLIST_SHOWNONE	-1

extern void XbrListClear(Widget);
extern void XbrListAddItem(Widget, char *, int);
extern char **XbrListGetSelection(Widget);
extern void XbrListSelectionFree(char **);
extern XbrListItemSelected(Widget, int *);
extern void XbrListTop(Widget);
extern int XbrListNumItems(Widget);

#endif
