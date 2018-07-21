/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrColour.h
 * CREATED:	Thu Jun  1 1995
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: Header file for XbrColour.c.
 *
 */

#ifndef	_XBR_XBR_COLOUR_H
#define	_XBR_XBR_COLOUR_H

extern void XbrColourAllocate(Display *, Colormap, XColor *);
extern void XbrColoursFree(Display *, Colormap, unsigned long [], int);

#endif
