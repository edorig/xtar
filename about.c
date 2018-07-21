/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	about.c
 * CREATED:	Thu Nov 18 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Simply displays an about box with the University logo and my
 *              name in it.
 *
 */

#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>

#include "Xbr.h"
#include "common.h"

#include "sc.h"

#include <stdio.h>

#define ABOUT_TEXT	"XTAR V1.5b2\n\n(c) Rik Turnbull 1999\n(rik@stella-consulting.ltd.uk)\n\nStella Consulting Ltd\nhttp://www.stella-consulting.ltd.uk/"

extern Widget TopLevel;

/*****************************************************************************/
/* General function declarations.                                            */
/*****************************************************************************/
void about_callback(Widget, XtPointer, XtPointer);
static void about_show();
void about_loadPixmap(Widget);

/*****************************************************************************/
/* Widget declarations.                                                      */
/*****************************************************************************/

/* Top level widgets */
static Widget AboutShell = NULL,	/* Shell to hold everyfink           */
              AboutForm,		/* Main widget is a form             */
              AboutClosePB,		/* Push Button to close window       */
              AboutLabel,		/* Label to display text             */
              AboutPixmap,		/* Label to display bitmap           */
              AboutSeparator;		/* Separator to split display        */

/* about_show:*************************************************************/
/* about_show: Create widgets for ABOUT box if necessary and display it.  */
/* about_show:*************************************************************/

static void about_show()
{
    Arg args[10];
    Cardinal n;
    XmString string;

    /* Only create the interface once - popup it up from then on */
    if(AboutShell != NULL) {
        XtPopup(AboutShell, XtGrabExclusive);
        return;
    }

    /* Top level shell */
    n = 0; 
    XtSetArg(args[n], XmNminWidth, 450); n++;
    XtSetArg(args[n], XmNminHeight, 200); n++;
    AboutShell = XtCreatePopupShell("AboutShell", xmDialogShellWidgetClass,
      TopLevel, args, n);
    XtAddCallback(AboutShell, XmNpopupCallback, XbrCursorAddCB,
     (XtPointer)XbrGRAB);
    XtAddCallback(AboutShell, XmNpopdownCallback, XbrCursorDeleteCB, NULL);

    /* AboutForm. XmFORM. */
    n = 0;
    XtSetArg(args[n], XmNmarginHeight, 5); n++;
    XtSetArg(args[n], XmNmarginWidth, 5); n++;
    XtSetArg(args[n], XmNautoUnmanage, False); n++;
    XtSetArg(args[n], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL); n++;
    AboutForm = XmCreateForm(AboutShell, "AboutForm", args, n);
    XtManageChild(AboutForm);

    /* AboutClosePB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("OK");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    AboutClosePB = XmCreatePushButton(AboutForm, "AboutClosePB", args, n);
    XtAddCallback(AboutClosePB, XmNactivateCallback, XbrCBPopdownShell, AboutShell);
    XmStringFree(string);
    XtManageChild(AboutClosePB);

    /* AboutSeparator. XmSEPARATOR. */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, AboutClosePB); n++;
    AboutSeparator = XmCreateSeparator(AboutForm, "AboutSeparator", args, n);
    XtManageChild(AboutSeparator);

    /* AboutLabel. XmLABEL. */
    n = 0;
    string = XmStringCreateLocalized(ABOUT_TEXT);
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, AboutSeparator); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    XtSetArg(args[n], XmNalignment, XmALIGNMENT_CENTER); n++;
    AboutLabel = XmCreateLabel(AboutForm, "AboutLabel", args, n);
    XmStringFree(string);
    XtManageChild(AboutLabel);

    /* AboutPixmap. XmLABEL. */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, AboutLabel); n++;
    XtSetArg(args[n], XmNalignment, XmALIGNMENT_CENTER); n++;
    AboutPixmap = XmCreateLabel(AboutForm, "AboutPixmap", args, n);
    XtManageChild(AboutPixmap);

    about_loadPixmap(AboutPixmap);

    /* Display it */
    XtPopup(AboutShell, XtGrabExclusive);
}

/* about_loadPixmap:*******************************************************/
/* about_loadPixmap: Takes the included logo and creates a pixmap and     */
/* about_loadPixmap: then loads it into the label.                        */
/* about_loadPixmap:*******************************************************/

void about_loadPixmap(Widget w)
{
    Display *display = XtDisplay(w);
    Screen *screen = XtScreen(w);
    Colormap colourmap = DefaultColormap(display, DefaultScreen(display));
    XBR_IMAGE *xbrimage;
    XColor colour;
    int size = sizeof(sc);
    Pixel background;
char *errmsg = NULL;
 
    XtVaGetValues(w, XmNbackground, &background, NULL);

    colour.pixel = background;
    XQueryColor(display, colourmap, &colour);

    xbrimage = XbrGfxGIF(display, sc, size, 0, &colour, &errmsg);
if(xbrimage == NULL) {
fprintf(stderr, "Failed to load GIF: %s\n", errmsg);
}
    XtVaSetValues(w,XmNlabelType,XmPIXMAP,XmNlabelPixmap,xbrimage->pixmap,NULL);
}

/* about_callback:*********************************************************/
/* about_callback: A wrapper function to display the about box.           */
/* about_callback:*********************************************************/

void about_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
    XbrCursorBusy(TopLevel);
    about_show();
    XbrCursorReset(TopLevel);
}
