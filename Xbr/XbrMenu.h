/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrMenu.h
 * CREATED:	Mon Nov  1 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Header file for XbrMenu.c
 *
 */

#ifndef XBRLIB_XBRMENU_H
#define XBRLIB_XBRMENU_H

#define	XbrMENU_POPUP		1
#define	XbrMENU_PULLDOWN	2
#define XbrMENU_PULLDOWN_RADIO	3
#define XbrMENU_OPTION		4

#define	XbrLABEL		1
#define	XbrPB			2
#define	XbrSEPARATOR		3
#define XbrSUBMENU		4
#define XbrTOGGLE		5

#define	XbrSET			1
#define	XbrUNSET		0
#define	XbrNOSET		-1

typedef struct menu_data {
    int type;			/* Type of widget 1 from 5 above */
    char *label;		/* Label for widget              */
    XtCallbackProc callback;	/* Callback to be added          */
    XtPointer data;		/* Data for callback             */
    int set;			/* Set this (if toggle button)   */
    struct menu_data *submenu;	/* Possible submenu              */
    char *title;		/* Title for submenu             */
    int stype;			/* Type of submenu               */
    int n;			/* Number of items in submenu    */
} XbrMenuData;

extern Widget *XbrMenu(Widget, char *, int, XbrMenuData[], int);

#endif
