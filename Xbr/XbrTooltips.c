/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrTooltips.c
 * CREATED:	Tue 16 Feb 1999 (happy birthday to me!)
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Routines for implementing PC style tooltips.
 *
 */


#include <Xm/Xm.h>
#include <Xm/Label.h>

#include <time.h>

#include "XbrTooltips.h"

static Widget XbrTooltipsShell = NULL;
static Widget XbrTooltipsLabel = NULL;
static Widget XbrTooltipsTopLevel = NULL;
static Widget XbrTooltipsWidget = NULL;
static XtIntervalId XbrTooltipsIntervalId = 0;
static time_t XbrTooltipsTime;

void XbrTooltipsRegister(Widget, Widget, char *);

static void XbrTooltipsEnter(Widget, XtPointer, XEvent *, Boolean *);
static void XbrTooltipsTimer(XtPointer, XtIntervalId *);
static void XbrTooltipsPopup(Widget, char *);
static void XbrTooltipsLeave(Widget, XtPointer, XEvent *, Boolean *);

/*----------------------------------------------------------------------------
  XbrTooltipsRegister()
  For the given Widget add event handlers to catch the focus events, to
  popup the appropriate text near the widget.
  ----------------------------------------------------------------------------*/
void XbrTooltipsRegister(Widget toplevel, Widget w, char *message)
{
    XbrTooltipsTopLevel = toplevel;

    XtAddEventHandler(w, EnterWindowMask, False, XbrTooltipsEnter, message);
    XtAddEventHandler(w, LeaveWindowMask, False, XbrTooltipsLeave, NULL);
    XtAddEventHandler(w, ButtonPressMask, False, XbrTooltipsLeave, NULL);
}

/*----------------------------------------------------------------------------
  XbrTooltipsEnter()
  An enter window event has occurred - register a timeout, to display the 
  tooltip message after a delay.
  ----------------------------------------------------------------------------*/
static void XbrTooltipsEnter(Widget w, XtPointer client_data,
  XEvent *event, Boolean *continue_to_dispatch_return)
{
    int delay = 1500;
    time_t time_now;

    if(XbrTooltipsIntervalId != 0) {
        XtRemoveTimeOut(XbrTooltipsIntervalId);
    }

    time(&time_now);
    if((time_now - XbrTooltipsTime) < 1)
        delay = 0;

    XbrTooltipsIntervalId = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
      delay, XbrTooltipsTimer, client_data);

    XbrTooltipsWidget = w;
}

static void XbrTooltipsTimer(XtPointer client_data, XtIntervalId *id)
{
    XbrTooltipsPopup(XbrTooltipsWidget, (char *) client_data);
    XtRemoveTimeOut(XbrTooltipsIntervalId);
    XbrTooltipsIntervalId = 0;
}


/*----------------------------------------------------------------------------
  XbrTooltipsPopup()
  Display the tooltip message window.
  ----------------------------------------------------------------------------*/
static void XbrTooltipsPopup(Widget w, char *message)
{
    Window root, qroot, window, parent, *children;
    XWindowAttributes attributes;
    unsigned int nchildren;
    Cardinal n;
    Arg args[10];
    Position rx = 0, ry = 0;
    Dimension width, height;
    XmString string;

    root = DefaultRootWindow(XtDisplay(w));

    window = XtWindow(w);
    while(window != root) {
        XGetWindowAttributes(XtDisplay(w), window, &attributes);
        rx += attributes.x; ry += attributes.y;

        /* What about freeing this up again??? */
        XQueryTree(XtDisplay(w), window, &qroot, &window, &children,
          &nchildren);
    }

    n = 0;
    XtSetArg(args[n], XmNwidth, &width); n++;
    XtSetArg(args[n], XmNheight, &height); n++;
    XtGetValues(w, args, n);
    rx += width; ry += height;

    if(XbrTooltipsShell == NULL) {
        n = 0;
        XtSetArg(args[n], XtNallowShellResize, True); n++;
        XbrTooltipsShell = XtCreatePopupShell("XbrTooltipsShell",
          overrideShellWidgetClass, XbrTooltipsTopLevel, args, n);

        XbrTooltipsLabel = XmCreateLabel(XbrTooltipsShell, message, NULL, 0);
        XtManageChild(XbrTooltipsLabel);
    }

    n = 0;
    XtSetArg(args[n], XmNx, rx); n++;
    XtSetArg(args[n], XmNy, ry); n++;
    XtSetValues(XbrTooltipsShell, args, n);

    n = 0;
    string = XmStringCreateLocalized(message);
    XtSetArg(args[n], XmNlabelString, string); n++;
    XtSetValues(XbrTooltipsLabel, args, n);
    XmStringFree(string);

    XtPopup(XbrTooltipsShell, XtGrabNone);
}

/*----------------------------------------------------------------------------
  XbrTooltipsLeave()
  A leave window event has occurred - popdown the tooltip message.
  ----------------------------------------------------------------------------*/
static void XbrTooltipsLeave(Widget w, XtPointer client_data,
  XEvent *event, Boolean *continue_to_dispatch_return)
{
    *continue_to_dispatch_return = True;

    if(XbrTooltipsShell != NULL) {
        XtPopdown(XbrTooltipsShell);
        time(&XbrTooltipsTime);
    }

    if(XbrTooltipsIntervalId != 0) {
        XtRemoveTimeOut(XbrTooltipsIntervalId);
    }
}
