/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrCreate.c
 * CREATED:	Thu Nov  4 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Convenience functions for creating various widgets.
 *
 */

#include <Xm/Xm.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>

Widget XbrCreateList(Widget, unsigned char, unsigned char, unsigned char, int,
  unsigned char, int, unsigned char, int, unsigned char, int);
Widget XbrCreateText(Widget, Boolean, Boolean, Boolean, unsigned char,
  unsigned char, int, unsigned char, int, unsigned char, int, unsigned char,
  int);

/* XbrCreateList:************************************************************/
/* XbrCreateList: Convenience function for creating a scrolled XmList on an */
/* XbrCreateList: XmForm manager.                                           */
/* XbrCreateList:************************************************************/

Widget XbrCreateList(Widget parent, unsigned char sizeP, unsigned char dispP, 
  unsigned char leftAt, int leftPos, unsigned char rightAt, int rightPos,
  unsigned char topAt, int topPos, unsigned char botAt, int botPos)
{
    Widget w;
    Arg args[2];

    XtSetArg(args[0], XmNlistSizePolicy, sizeP);
    XtSetArg(args[1], XmNscrollBarDisplayPolicy, dispP);
    w = XmCreateScrolledList(parent, "Scrolled List", args, 2);
    XtVaSetValues(XtParent(w),
      XmNleftAttachment,   leftAt,     XmNleftPosition,   leftPos,
      XmNrightAttachment,  rightAt,    XmNrightPosition,  rightPos,
      XmNtopAttachment,    topAt,      XmNtopPosition,    topPos,
      XmNbottomAttachment, botAt,      XmNbottomPosition, botPos,
      NULL);
   
    XtManageChild(w);

    return(w);
}

/* XbrCreateText:************************************************************/
/* XbrCreateText: Convenience function for creating a scrolled XmText on an */
/* XbrCreateText: XmForm manager.                                           */
/* XbrCreateText:************************************************************/

Widget XbrCreateText(Widget parent, Boolean editable, Boolean scroll, Boolean
  word_wrap, unsigned char mode, unsigned char leftAt, int leftPos,
  unsigned char rightAt, int rightPos, unsigned char topAt, int topPos,
  unsigned char botAt, int botPos)
{
    Widget w;
    Arg args[5];

    XtSetArg(args[0], XmNeditable, editable);
    XtSetArg(args[1], XmNcursorPositionVisible, editable);
    XtSetArg(args[2], XmNscrollHorizontal, scroll);
    XtSetArg(args[3], XmNwordWrap, word_wrap);
    XtSetArg(args[4], XmNeditMode, mode);
    w = XmCreateScrolledText(parent, "Scrolled Text", args, 5);
    XtVaSetValues(XtParent(w),
      XmNleftAttachment,   leftAt,     XmNleftPosition,   leftPos,
      XmNrightAttachment,  rightAt,    XmNrightPosition,  rightPos,
      XmNtopAttachment,    topAt,      XmNtopPosition,    topPos,
      XmNbottomAttachment, botAt,      XmNbottomPosition, botPos,
      NULL);

    XtManageChild(w);

    return(w);
} 
