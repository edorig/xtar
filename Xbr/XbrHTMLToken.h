/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLToken.h
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Header file for XbrHTMLToken.c.
 *
 */

#ifndef	_XBR_XBRHTMLTOKEN_H
#define	_XBR_XBRHTMLTOKEN_H

#include "XbrHTML.h"

typedef struct _XbrHTMLToken
{
    unsigned int tag;
    char *text;
    struct _XbrHTMLToken *next;
} XbrHTMLToken;

extern XbrHTMLToken *XbrHTMLTokenize(XbrHTMLWidget, char *);
extern void XbrHTMLTokenFree(XbrHTMLToken *);

#endif
