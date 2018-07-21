/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	Xbr.h
 * CREATED:	Mon Nov  1 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Main header file for the Xbr X windows library.
 *
 */

#ifndef	XBRLIB_XBR_H
#define	XBRLIB_XBR_H

#if (XmVERSION < 1) || ((XmVERSION == 1) && (XmREVISION < 2))
#define XmFONTLIST_DEFAULT_TAG  XmSTRING_DEFAULT_CHARSET
#define XmStringCreateLocalized(string) XmStringCreateLtoR(string,XmSTRING_DEFAULT_CHARSET)
#endif

#define XbrDisplay(w)           XtDisplay(w)
#define XbrRootWindow(w)        RootWindowOfScreen(XtScreen(w))
#define XbrWindow(w)            XtWindow(w)
#define XbrScreen(w)		XtScreen(w)
#define XbrDepth(w)		DefaultDepthOfScreen(XbrScreen(w))
#define XbrColourmap(w)		DefaultColormap(XbrDisplay(w), DefaultScreen(XbrDisplay(w)))
#define XbrVisual(w)		DefaultVisualOfScreen(XbrScreen(w))

#include "XbrCB.h"
#include "XbrColour.h"
#include "XbrCreate.h"
#include "XbrCursor.h"
#include "XbrDialog.h"
#include "XbrFSB.h"
#include "XbrGfx.h"
#include "XbrHelp.h"
#include "XbrHTML.h"
#include "XbrLabel.h"
#include "XbrList.h"
#include "XbrMenu.h"
#include "XbrRes.h"
#include "XbrTooltips.h"

#endif
