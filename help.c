/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	help.c
 * CREATED:	Tue Jun  7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Hypertext help system.
 *
 */

#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>

#include "Xbr.h"
#include "common.h"

extern Widget TopLevel;

static char *homepage = NULL;
static char *help_dir = NULL;
static Stack *history = NULL;

/*****************************************************************************/
/* General function declarations.                                            */
/*****************************************************************************/
void help_callback(Widget, XtPointer, XtPointer);
static Pixmap help_resolve(Widget,char *,int *,int *,unsigned long **,int *);
static void help_show(char *);
static void help_create();
static char *help_filename(char *);
static void help_load(char *);
static void help_setPBs();
static void help_anchorCB(Widget, XtPointer, XtPointer);
static void help_homeCB(Widget, XtPointer, XtPointer);
static void help_backCB(Widget, XtPointer, XtPointer);
static void help_forwardCB(Widget, XtPointer, XtPointer);

/*****************************************************************************/
/* Widget declarations.                                                      */
/*****************************************************************************/

/* Top level widgets */
static Widget HelpShell = NULL,	/* Shell to hold everyfink           */
              HelpForm,		/* Main widget is a form             */
              HelpHomePB,       /* Load main help page               */
              HelpBackPB,       /* Scroll back through history       */
              HelpForwardPB,    /* Scroll forward through history    */
              HelpClosePB,	/* Push Button to close window       */
              HelpText,		/* Label to display text             */
              HelpSeparator;	/* Separator to split display        */

/* help_show:*************************************************************/
/* help_show: Create widgets for ABOUT box if necessary and display it.  */
/* help_show:*************************************************************/
static void help_show(char *filename)
{
    char *htmlname;

    /* Only create the interface once - popup it up from then on */
    if(HelpShell == NULL) {
        help_create();
        htmlname = help_filename(filename);
        help_load(htmlname);
        homepage = htmlname;
        StackPush(history, htmlname);
        help_setPBs();
    }

    XtPopup(HelpShell, XtGrabExclusive);
}

/* help_create:*************************************************************/
/* help_create: Create widgets for ABOUT box if necessary and display it.  */
/* help_create:*************************************************************/

static void help_create()
{
    Arg args[10];
    Cardinal n;
    XmString string;

    /* Top level shell */
    n = 0; 
    /*XtSetArg(args[n], XmNminWidth, 520); n++;
    XtSetArg(args[n], XmNminHeight, 400); n++;*/
    HelpShell = XtCreatePopupShell("HelpShell", xmDialogShellWidgetClass,
      TopLevel, args, n);
    XtAddCallback(HelpShell, XmNpopupCallback, XbrCursorAddCB,
     (XtPointer)XbrGRAB);
    XtAddCallback(HelpShell, XmNpopdownCallback, XbrCursorDeleteCB, NULL);

    /* Form widget - as usual */
    n = 0;
    XtSetArg(args[n], XmNmarginHeight, 5); n++;
    XtSetArg(args[n], XmNmarginWidth, 5); n++;
    XtSetArg(args[n], XmNautoUnmanage, False); n++;
    XtSetArg(args[n], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL); n++;
    HelpForm = XmCreateForm(HelpShell, "HelpForm", args, n);
    XtManageChild(HelpForm);

    /* HelpHomePB. Push button to display original page. */
    n = 0;
    string = XmStringCreateLocalized("Index");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    HelpHomePB = XmCreatePushButton(HelpForm, "HelpHomePB", args, n);
    XtAddCallback(HelpHomePB, XmNactivateCallback, help_homeCB, NULL);
    XbrTooltipsRegister(TopLevel, HelpHomePB, "Display help index");
    XmStringFree(string);
    XtManageChild(HelpHomePB);

    /* HelpBackPB. Push button to display original page. */
    n = 0;
    string = XmStringCreateLocalized("Back");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, HelpHomePB); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    HelpBackPB = XmCreatePushButton(HelpForm, "HelpBackPB", args, n);
    XtAddCallback(HelpBackPB, XmNactivateCallback, help_backCB, NULL);
    XbrTooltipsRegister(TopLevel, HelpBackPB, "Go back in document history");
    XmStringFree(string);
    XtManageChild(HelpBackPB);

    /* HelpForwardPB. Push button to display original page. */
    n = 0;
    string = XmStringCreateLocalized("Forward");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, HelpBackPB); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    HelpForwardPB = XmCreatePushButton(HelpForm, "HelpForwardPB", args, n);
    XtAddCallback(HelpForwardPB, XmNactivateCallback, help_forwardCB, NULL);
    XbrTooltipsRegister(TopLevel, HelpForwardPB,
      "Go forward in document history");
    XmStringFree(string);
    XtManageChild(HelpForwardPB);

    /* HelpClosePB. Push button to close window. */
    n = 0;
    string = XmStringCreateLocalized("Close");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    HelpClosePB = XmCreatePushButton(HelpForm, "HelpClosePB", args, n);
    XmStringFree(string);
    XtAddCallback(HelpClosePB,XmNactivateCallback,XbrCBPopdownShell,HelpShell);
    XbrTooltipsRegister(TopLevel, HelpClosePB, "Close the help window.");
    XtManageChild(HelpClosePB);

    /* HelpSeparator. Push button to close window. */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNtopWidget, HelpClosePB); n++;
    XtSetArg(args[n], XmNtopOffset, 2); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    HelpSeparator = XmCreateSeparator(HelpForm, "HelpSeparator", args, n);
    XtManageChild(HelpSeparator);

    /* Text Widget to hold details */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNtopWidget, HelpSeparator); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNimageResolver, help_resolve); n++;
    HelpText = XbrCreateHTML(HelpForm, "HelpText", args, n);
    XtAddCallback(HelpText, XmNanchorCallback, help_anchorCB, NULL);
    XtManageChild(HelpText);

    /* Create a history list */
    history = StackCreate(10);
}

/*----------------------------------------------------------------------------
  help_resolve()
  Load the image link and resolve it to a Pixmap.
  ---------------------------------------------------------------------------*/
static Pixmap help_resolve(Widget w, char *src, int *width, int *height,
  unsigned long **colours, int *ncolours)
{
    Pixmap pixmap = 0;
    XBR_IMAGE *xbrimage;
    char filename[256];
    unsigned char *data;
    unsigned long *rcolours;

    Display *display = XtDisplay(w);
    Colormap colourmap = DefaultColormap(display, DefaultScreen(display));
    XColor colour;
    Pixel background;

    int size, i;
    char *errmsg = NULL;

    sprintf(filename, "%s/%s", help_dir, src);
    data = (unsigned char *) file_load(filename, &size);
    if(data == NULL) {
        fprintf(stderr, "ERROR LOADING: %s\n", filename);
    }

    XtVaGetValues(w, XmNbackground, &background, NULL);
    colour.pixel = background;
    XQueryColor(display, colourmap, &colour);

    xbrimage = XbrGfxGIF(display, data, size, 0, &colour, &errmsg);
    *width = xbrimage->width;
    *height = xbrimage->height;
    *ncolours = xbrimage->npixels;
    pixmap = xbrimage->pixmap;

    if(xbrimage->npixels > 0) {
        rcolours = (unsigned long *) mem_calloc(xbrimage->npixels);
        for(i = 0; i < xbrimage->npixels; i++) {
            rcolours[i] = xbrimage->pixels[i];
        }
        *colours = rcolours;
    } else {
        *colours = NULL;
    }

    if(errmsg != NULL) {
        fprintf(stderr, "Error: %s\n", errmsg);
    }

    free(xbrimage->data);
    free(xbrimage);

    return(pixmap);
}

/*----------------------------------------------------------------------------
  help_filename()
  Determine the full name of a file, given its HTML link.
  ---------------------------------------------------------------------------*/
static char *help_filename(char *filename)
{
    char html[256];

    if(filename) {
        if(*filename == '/' || help_dir == NULL)
            strcpy(html,filename);
        else
            sprintf(html,"%s/%s", help_dir, filename);

        if(stat_isdir(html)) {
            if(strlen(html)>1 && html[strlen(html)-1] != '/')
                strcat(html,"/");
            strcat(html,"index.html");
        }
    } else {
        *html = 0;
    }

    return(mem_strdup(html));
}

/*----------------------------------------------------------------------------
  help_load()
  Load the selected file into the help window.
  ---------------------------------------------------------------------------*/
static void help_load(char *filename)
{
    char *data;

    XbrCursorBusy(TopLevel);

    if(help_dir)
        free(help_dir);
    help_dir = strdup(fname_dirname(filename));

    data = file_load(filename, NULL);
    if(data == NULL)
        data = strdup("<B>Unable to load document!</B>");

    XbrHTMLSetText(HelpText, data);

    free(data);

    XbrCursorReset(TopLevel);
}

/*----------------------------------------------------------------------------
  help_setPBs()
  Set the back, forward and home push buttons appropriately.
  ---------------------------------------------------------------------------*/ 
static void help_setPBs()
{
    if(StackCanPop(history))
        XtSetSensitive(HelpBackPB, True);
    else
        XtSetSensitive(HelpBackPB, False);

    if(StackCanAdvance(history))
        XtSetSensitive(HelpForwardPB, True);
    else
        XtSetSensitive(HelpForwardPB, False);
}

/* help_callback:*********************************************************/
/* help_callback: A wrapper function to display the help window.         */
/* help_callback:*********************************************************/
void help_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
    char **doc = (char **)client_data;

    XbrCursorBusy(TopLevel);
    help_show(*doc);
    XbrCursorReset(TopLevel);
}

/*----------------------------------------------------------------------------
  help_anchorCB()
  A link has been clicked on in the hypertext help window.
  --------------------------------------------------------------------------*/
static void help_anchorCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XbrHTMLCallbackStruct *cb = (XbrHTMLCallbackStruct *) call_data;
    char *filename;

    filename = help_filename(cb->href);
    help_load(filename);
    StackPush(history, filename);
    help_setPBs();
}

/*----------------------------------------------------------------------------
  help_homeCB()
  The HOME push button has been pressed.
  --------------------------------------------------------------------------*/
static void help_homeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XbrHTMLCallbackStruct *cb = (XbrHTMLCallbackStruct *) call_data;
    char *filename;

    filename = mem_strdup(homepage);
    help_load(filename);
    StackPush(history, filename);
    help_setPBs();
}

/*----------------------------------------------------------------------------
  help_backCB()
  The BACK push button has been pressed.
  --------------------------------------------------------------------------*/
static void help_backCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XbrHTMLCallbackStruct *cb = (XbrHTMLCallbackStruct *) call_data;
    char *filename;

    filename = StackPop(history);
    filename = StackPeek(history);
    help_load(filename);
    help_setPBs();
}

/*----------------------------------------------------------------------------
  help_forwardCB()
  The FORWARD push button has been pressed.                    
  --------------------------------------------------------------------------*/
static void help_forwardCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XbrHTMLCallbackStruct *cb = (XbrHTMLCallbackStruct *) call_data;
    char *filename;

    if(StackCanAdvance(history)) {
        filename = StackAdvance(history);
        help_load(filename);
        help_setPBs();
    }
}
