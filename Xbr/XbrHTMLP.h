/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLP.h
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Internal header file for XbrHTML widget.
 *
 */

#ifndef XBR_XBRHTMLP_H
#define XBR_XBRHTMLP_H

#include <Xm/XmP.h>
#include <Xm/ManagerP.h>

#include "XbrHTML.h"
#include "XbrHTMLFormat.h"
#include "XbrHTMLToken.h"

#define XbrDisplay(w)           XtDisplay(w)
#define XbrRootWindow(w)        RootWindowOfScreen(XtScreen(w))
#define XbrWindow(w)            XtWindow(w)

#define XbrHTMLAddToList(f,c,t) \
	if(f) \
		c->next = t; \
	else \
		f = t; \
	c = t;

#define XbrHTML_FONT_NORMAL		0
#define XbrHTML_FONT_BOLD		1
#define XbrHTML_FONT_ITALIC 		(1<<1)
#define XbrHTML_FONT_FIXED		(1<<2)
#define XbrHTML_FONT_BOLD_ITALIC	(XbrHTML_FONT_BOLD|XbrHTML_FONT_ITALIC)
#define XbrHTML_FONT_FIXED_ITALIC	(XbrHTML_FONT_FIXED|XbrHTML_FONT_ITALIC)
#define XbrHTML_FONT_FIXED_BOLD		(XbrHTML_FONT_FIXED|XbrHTML_FONT_BOLD)
#define XbrHTML_FONT_FIXED_BOLD_ITALIC	(XbrHTML_FONT_FIXED|XbrHTML_FONT_BOLD|\
					                    XbrHTML_FONT_ITALIC)

#define	XbrHTML_FOREGROUND_DEFAULT	0
#define	XbrHTML_FOREGROUND_ANCHOR	1
#define	XbrHTML_FOREGROUND_SELECT	2
#define	XbrHTML_FOREGROUND_VISITED	3

/*typedef Pixmap (*XbrHTMLImageResolver)(Widget, char *, int *, int *);*/

typedef struct _XbrHTMLClassPart {
    int ignore;
} XbrHTMLClassPart;

typedef struct _XbrHTMLClassRec {
    CoreClassPart            core_class;
    CompositeClassPart       composite_class;
    ConstraintClassPart      constraint_class;
    XmManagerClassPart       manager_class;
    XbrHTMLClassPart         html_class;
} XbrHTMLClassRec;

typedef struct _XbrHTMLPart {
    /* PUBLIC VALUES */
    XFontStruct *fonts[48];	/* Pointer to all 48 possible fonts */
    Pixel foreground[4];	/* Default, anchor, visited & select colours */

    char *title;		/* Title of document as specified by <TITLE> */

    XbrHTMLImageResolver image_resolver;
    XbrHTMLVisitedTest visited_test;

    XtCallbackList anchor_callback;

    /* PRIVATE VALUES */
    XbrHTMLElement *selected;	/* Currently selected/highlighted element */
    XbrHTMLElement *elements;	/* List of formatted elements */
    XbrHTMLToken *tokens;	/* List of parsed tokens */

    int x_value, y_value;	/* X and Y values of scrollbars */
    int x_offset, y_offset;	/* Offset due to scroll bar positions */

    int da_width,  da_height;	/* Width and height of drawing area */
    int doc_width, doc_height;	/* Width and height of actual document */

    Widget da;			/* Drawing area */
    Widget vscrollbar;		/* Vertical scroll bar widget */
    Widget hscrollbar;		/* Horizontal scroll bar widget */

    char **anchors;		/* List of anchors */
    int nanchors;		/* Number of current anchors */
    int maxanchor;		/* Size of the anchor array */

    GC clear_gc;		/* GC for clearing drawing area */
    GC gc;			/* GC for drawing operations */
} XbrHTMLPart;

typedef struct _XbrHTMLRec {
    CorePart           core;
    CompositePart      composite;
    ConstraintPart     constraint;
    XmManagerPart      manager;
    XbrHTMLPart        html;
} XbrHTMLRec;

#endif
