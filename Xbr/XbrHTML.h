/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTML.h
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: External header file for XbrHTML widget.
 *
 */

#ifndef XBR_XBRHTML_H
#define XBR_XBRHTML_H

extern WidgetClass htmlWidgetClass;

typedef struct _XbrHTMLClassRec *XbrHTMLClass;
typedef struct _XbrHTMLRec *XbrHTMLWidget;

typedef Pixmap (*XbrHTMLImageResolver)(Widget, char *, int *, int *,
  unsigned long **, int *);
typedef Boolean (*XbrHTMLVisitedTest)(char *);

#define	XtNnormalFont1		"normalFont1"
#define	XtCNormalFont1		"NormalFont1"
#define	XtNnormalFont2		"normalFont2"
#define	XtCNormalFont2		"NormalFont2"
#define	XtNnormalFont3		"normalFont3"
#define	XtCNormalFont3		"NormalFont3"
#define	XtNnormalFont4		"normalFont4"
#define	XtCNormalFont4		"NormalFont4"
#define	XtNnormalFont5		"normalFont5"
#define	XtCNormalFont5		"NormalFont5"
#define	XtNnormalFont6		"normalFont6"
#define	XtCNormalFont6		"NormalFont6"

#define	XtNboldFont1		"boldFont1"
#define	XtCBoldFont1		"BoldFont1"
#define	XtNboldFont2		"boldFont2"
#define	XtCBoldFont2		"BoldFont2"
#define	XtNboldFont3		"boldFont3"
#define	XtCBoldFont3		"BoldFont3"
#define	XtNboldFont4		"boldFont4"
#define	XtCBoldFont4		"BoldFont4"
#define	XtNboldFont5		"boldFont5"
#define	XtCBoldFont5		"BoldFont5"
#define	XtNboldFont6		"boldFont6"
#define	XtCBoldFont6		"BoldFont6"

#define	XtNitalicFont1		"italicFont1"
#define	XtCItalicFont1		"ItalicFont1"
#define	XtNitalicFont2		"italicFont2"
#define	XtCItalicFont2		"ItalicFont2"
#define	XtNitalicFont3		"italicFont3"
#define	XtCItalicFont3		"ItalicFont3"
#define	XtNitalicFont4		"italicFont4"
#define	XtCItalicFont4		"ItalicFont4"
#define	XtNitalicFont5		"italicFont5"
#define	XtCItalicFont5		"ItalicFont5"
#define	XtNitalicFont6		"italicFont6"
#define	XtCItalicFont6		"ItalicFont6"

#define	XtNboldItalicFont1		"boldItalicFont1"
#define	XtCBoldItalicFont1		"BoldItalicFont1"
#define	XtNboldItalicFont2		"boldItalicFont2"
#define	XtCBoldItalicFont2		"BoldItalicFont2"
#define	XtNboldItalicFont3		"boldItalicFont3"
#define	XtCBoldItalicFont3		"BoldItalicFont3"
#define	XtNboldItalicFont4		"boldItalicFont4"
#define	XtCBoldItalicFont4		"BoldItalicFont4"
#define	XtNboldItalicFont5		"boldItalicFont5"
#define	XtCBoldItalicFont5		"BoldItalicFont5"
#define	XtNboldItalicFont6		"boldItalicFont6"
#define	XtCBoldItalicFont6		"BoldItalicFont6"

#define	XtNfixedNormalFont1		"fixedNormalFont1"
#define	XtCFixedNormalFont1		"FixedNormalFont1"
#define	XtNfixedNormalFont2		"fixedNormalFont2"
#define	XtCFixedNormalFont2		"FixedNormalFont2"
#define	XtNfixedNormalFont3		"fixedNormalFont3"
#define	XtCFixedNormalFont3		"FixedNormalFont3"
#define	XtNfixedNormalFont4		"fixedNormalFont4"
#define	XtCFixedNormalFont4		"FixedNormalFont4"
#define	XtNfixedNormalFont5		"fixedNormalFont5"
#define	XtCFixedNormalFont5		"FixedNormalFont5"
#define	XtNfixedNormalFont6		"fixedNormalFont6"
#define	XtCFixedNormalFont6		"FixedNormalFont6"

#define	XtNfixedBoldFont1		"fixedBoldFont1"
#define	XtCFixedBoldFont1		"FixedBoldFont1"
#define	XtNfixedBoldFont2		"fixedBoldFont2"
#define	XtCFixedBoldFont2		"FixedBoldFont2"
#define	XtNfixedBoldFont3		"fixedBoldFont3"
#define	XtCFixedBoldFont3		"FixedBoldFont3"
#define	XtNfixedBoldFont4		"fixedBoldFont4"
#define	XtCFixedBoldFont4		"FixedBoldFont4"
#define	XtNfixedBoldFont5		"fixedBoldFont5"
#define	XtCFixedBoldFont5		"FixedBoldFont5"
#define	XtNfixedBoldFont6		"fixedBoldFont6"
#define	XtCFixedBoldFont6		"FixedBoldFont6"

#define	XtNfixedItalicFont1		"fixedItalicFont1"
#define	XtCFixedItalicFont1		"FixedItalicFont1"
#define	XtNfixedItalicFont2		"fixedItalicFont2"
#define	XtCFixedItalicFont2		"FixedItalicFont2"
#define	XtNfixedItalicFont3		"fixedItalicFont3"
#define	XtCFixedItalicFont3		"FixedItalicFont3"
#define	XtNfixedItalicFont4		"fixedItalicFont4"
#define	XtCFixedItalicFont4		"FixedItalicFont4"
#define	XtNfixedItalicFont5		"fixedItalicFont5"
#define	XtCFixedItalicFont5		"FixedItalicFont5"
#define	XtNfixedItalicFont6		"fixedItalicFont6"
#define	XtCFixedItalicFont6		"FixedItalicFont6"

#define	XtNfixedBoldItalicFont1		"fixedBoldItalicFont1"
#define	XtCFixedBoldItalicFont1		"FixedBoldItalicFont1"
#define	XtNfixedBoldItalicFont2		"fixedBoldItalicFont2"
#define	XtCFixedBoldItalicFont2		"FixedBoldItalicFont2"
#define	XtNfixedBoldItalicFont3		"fixedBoldItalicFont3"
#define	XtCFixedBoldItalicFont3		"FixedBoldItalicFont3"
#define	XtNfixedBoldItalicFont4		"fixedBoldItalicFont4"
#define	XtCFixedBoldItalicFont4		"FixedBoldItalicFont4"
#define	XtNfixedBoldItalicFont5		"fixedBoldItalicFont5"
#define	XtCFixedBoldItalicFont5		"FixedBoldItalicFont5"
#define	XtNfixedBoldItalicFont6		"fixedBoldItalicFont6"
#define	XtCFixedBoldItalicFont6		"FixedBoldItalicFont6"

#define	XtNanchorColour			"anchorColour"
#define	XtCAnchorColour			"AnchorColour"
#define	XtNvisitedColour		"visitedColour"
#define	XtCVisitedColour		"VisitedColour"
#define	XtNselectColour			"selectColour"
#define	XtCSelectColour			"SelectColour"

#define	XtNimageResolver		"imageResolver"
#define	XtCImageResolver		"ImageResolver"
#define	XtNvisitedTest			"visitedTest"
#define	XtCVisitedTest			"VisitedTest"

#define	XtNanchorCallback		"anchorCallback"
#define	XtCAnchorCallback		"AnchorCallback"

#define	XmNnormalFont1		"normalFont1"
#define	XmCNormalFont1		"NormalFont1"
#define	XmNnormalFont2		"normalFont2"
#define	XmCNormalFont2		"NormalFont2"
#define	XmNnormalFont3		"normalFont3"
#define	XmCNormalFont3		"NormalFont3"
#define	XmNnormalFont4		"normalFont4"
#define	XmCNormalFont4		"NormalFont4"
#define	XmNnormalFont5		"normalFont5"
#define	XmCNormalFont5		"NormalFont5"
#define	XmNnormalFont6		"normalFont6"
#define	XmCNormalFont6		"NormalFont6"

#define	XmNboldFont1		"boldFont1"
#define	XmCBoldFont1		"BoldFont1"
#define	XmNboldFont2		"boldFont2"
#define	XmCBoldFont2		"BoldFont2"
#define	XmNboldFont3		"boldFont3"
#define	XmCBoldFont3		"BoldFont3"
#define	XmNboldFont4		"boldFont4"
#define	XmCBoldFont4		"BoldFont4"
#define	XmNboldFont5		"boldFont5"
#define	XmCBoldFont5		"BoldFont5"
#define	XmNboldFont6		"boldFont6"
#define	XmCBoldFont6		"BoldFont6"

#define	XmNitalicFont1		"italicFont1"
#define	XmCItalicFont1		"ItalicFont1"
#define	XmNitalicFont2		"italicFont2"
#define	XmCItalicFont2		"ItalicFont2"
#define	XmNitalicFont3		"italicFont3"
#define	XmCItalicFont3		"ItalicFont3"
#define	XmNitalicFont4		"italicFont4"
#define	XmCItalicFont4		"ItalicFont4"
#define	XmNitalicFont5		"italicFont5"
#define	XmCItalicFont5		"ItalicFont5"
#define	XmNitalicFont6		"italicFont6"
#define	XmCItalicFont6		"ItalicFont6"

#define	XmNboldItalicFont1		"boldItalicFont1"
#define	XmCBoldItalicFont1		"BoldItalicFont1"
#define	XmNboldItalicFont2		"boldItalicFont2"
#define	XmCBoldItalicFont2		"BoldItalicFont2"
#define	XmNboldItalicFont3		"boldItalicFont3"
#define	XmCBoldItalicFont3		"BoldItalicFont3"
#define	XmNboldItalicFont4		"boldItalicFont4"
#define	XmCBoldItalicFont4		"BoldItalicFont4"
#define	XmNboldItalicFont5		"boldItalicFont5"
#define	XmCBoldItalicFont5		"BoldItalicFont5"
#define	XmNboldItalicFont6		"boldItalicFont6"
#define	XmCBoldItalicFont6		"BoldItalicFont6"

#define	XmNfixedNormalFont1		"fixedNormalFont1"
#define	XmCFixedNormalFont1		"FixedNormalFont1"
#define	XmNfixedNormalFont2		"fixedNormalFont2"
#define	XmCFixedNormalFont2		"FixedNormalFont2"
#define	XmNfixedNormalFont3		"fixedNormalFont3"
#define	XmCFixedNormalFont3		"FixedNormalFont3"
#define	XmNfixedNormalFont4		"fixedNormalFont4"
#define	XmCFixedNormalFont4		"FixedNormalFont4"
#define	XmNfixedNormalFont5		"fixedNormalFont5"
#define	XmCFixedNormalFont5		"FixedNormalFont5"
#define	XmNfixedNormalFont6		"fixedNormalFont6"
#define	XmCFixedNormalFont6		"FixedNormalFont6"

#define	XmNfixedBoldFont1		"fixedBoldFont1"
#define	XmCFixedBoldFont1		"FixedBoldFont1"
#define	XmNfixedBoldFont2		"fixedBoldFont2"
#define	XmCFixedBoldFont2		"FixedBoldFont2"
#define	XmNfixedBoldFont3		"fixedBoldFont3"
#define	XmCFixedBoldFont3		"FixedBoldFont3"
#define	XmNfixedBoldFont4		"fixedBoldFont4"
#define	XmCFixedBoldFont4		"FixedBoldFont4"
#define	XmNfixedBoldFont5		"fixedBoldFont5"
#define	XmCFixedBoldFont5		"FixedBoldFont5"
#define	XmNfixedBoldFont6		"fixedBoldFont6"
#define	XmCFixedBoldFont6		"FixedBoldFont6"

#define	XmNfixedItalicFont1		"fixedItalicFont1"
#define	XmCFixedItalicFont1		"FixedItalicFont1"
#define	XmNfixedItalicFont2		"fixedItalicFont2"
#define	XmCFixedItalicFont2		"FixedItalicFont2"
#define	XmNfixedItalicFont3		"fixedItalicFont3"
#define	XmCFixedItalicFont3		"FixedItalicFont3"
#define	XmNfixedItalicFont4		"fixedItalicFont4"
#define	XmCFixedItalicFont4		"FixedItalicFont4"
#define	XmNfixedItalicFont5		"fixedItalicFont5"
#define	XmCFixedItalicFont5		"FixedItalicFont5"
#define	XmNfixedItalicFont6		"fixedItalicFont6"
#define	XmCFixedItalicFont6		"FixedItalicFont6"

#define	XmNfixedBoldItalicFont1		"fixedBoldItalicFont1"
#define	XmCFixedBoldItalicFont1		"FixedBoldItalicFont1"
#define	XmNfixedBoldItalicFont2		"fixedBoldItalicFont2"
#define	XmCFixedBoldItalicFont2		"FixedBoldItalicFont2"
#define	XmNfixedBoldItalicFont3		"fixedBoldItalicFont3"
#define	XmCFixedBoldItalicFont3		"FixedBoldItalicFont3"
#define	XmNfixedBoldItalicFont4		"fixedBoldItalicFont4"
#define	XmCFixedBoldItalicFont4		"FixedBoldItalicFont4"
#define	XmNfixedBoldItalicFont5		"fixedBoldItalicFont5"
#define	XmCFixedBoldItalicFont5		"FixedBoldItalicFont5"
#define	XmNfixedBoldItalicFont6		"fixedBoldItalicFont6"
#define	XmCFixedBoldItalicFont6		"FixedBoldItalicFont6"

#define	XmNanchorColour			"anchorColour"
#define	XmCAnchorColour			"AnchorColour"
#define	XmNvisitedColour		"visitedColour"
#define	XmCVisitedColour		"VisitedColour"
#define	XmNselectColour			"selectColour"
#define	XmCSelectColour			"SelectColour"

#define	XmNimageResolver		"imageResolver"
#define	XmCImageResolver		"ImageResolver"
#define	XmNvisitedTest			"visitedTest"
#define	XmCVisitedTest			"VisitedTest"

#define	XmNanchorCallback		"anchorCallback"
#define	XmCAnchorCallback		"AnchorCallback"

extern Widget XbrCreateHTML(Widget, char *, ArgList, Cardinal);
extern void XbrHTMLSetText(Widget, char *);

#define	XmCR_ANCHOR		1

typedef struct _XbrHTMLCallbackStruct {
    int reason;
    XEvent *event;
    Window window;
    char *href;
    char *text;
} XbrHTMLCallbackStruct;

#endif
