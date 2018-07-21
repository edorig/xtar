/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLFormat.h
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Header file for XbrHTMLFormat.c.
 *
 */

#ifndef _XBR_XBRHTMLFORMAT_H
#define _XBR_XBRHTMLFORMAT_H

#include "XbrHTML.h"

#define	XbrHTML_TYPE_LIST_ITEM	1	/* Bullet list item */
#define	XbrHTML_TYPE_TEXT	2	/* Text rendered in any fashion */
#define	XbrHTML_TYPE_LINE	3	/* Horizontal line */
#define	XbrHTML_TYPE_IMAGE	4	/* Inlined image */
#define	XbrHTML_TYPE_NEWLINE	5	/* Forced newline - for reformatting */
#define	XbrHTML_TYPE_NUMBER	6	/* List item as a number */

#define	XbrHTML_BULLET_SQUARE	-1
#define	XbrHTML_BULLET_DOT	-2
#define	XbrHTML_BULLET_CIRCLE	-3

typedef struct _XbrHTMLElement 
{
    int type;		/* Type of element as #defined above */
    int subtype;	/* Sub type, such as square bullet */
    int font;		/* Index into font table for type of font used */
    int foreground;	/* Default, anchor, visited or select colour */
    int x, y;		/* X,Y coordinates where element is drawn */
    int x0, y0;		/* X,Y coordinates of upper left corner of element */
    int width, height;	/* Width and height of element */

    int left_margin;	/* Where the left margin starts */

    int anchor;		/* Index into anchor table or 0 */

    Boolean preformatted;	/* Is this preformatted text */

    char *text;		/* Pointer to text, of a text element */
    int length;		/* Length of text string in characters */
    
    Pixmap pixmap;	/* Pointer to pixmap, if an image element */
    unsigned long *colours;	/* Pointer to allocated colours */
    int ncolours;	/* Number of allocated colours */

    struct _XbrHTMLElement *next;
} XbrHTMLElement;

extern XbrHTMLElement *XbrHTMLFormat(XbrHTMLWidget);
extern void XbrHTMLElementsFree(XbrHTMLWidget, XbrHTMLElement *);
extern void XbrHTMLReformat(XbrHTMLWidget);

#endif
