/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 * FILE NAME:	stats.c
 * CREATED:	Tue Jun  7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Quick stats window for those too lazy to read the man page.
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

#include "stats.h"

extern Widget TopLevel;

/*****************************************************************************/
/* General function declarations.                                            */
/*****************************************************************************/
void stats_show(XTAR_STATS *);
static char *stats_text(XTAR_STATS *);

/*****************************************************************************/
/* Widget declarations.                                                      */
/*****************************************************************************/

/* Top level widgets */
static Widget StatsShell = NULL,	/* Shell to hold everyfink          */
              StatsForm,		/* Main widget is a form            */
              StatsClose,               /* Push Button to close window      */
              StatsText,		/* Label to display text            */
              StatsSeparator;           /* Separator to split display       */

/* stats_show:***************************************************************/
/* stats_show: Create widgets for ABOUT box if necessary and display it.    */
/* stats_show:***************************************************************/

void stats_show(XTAR_STATS *stats)
{
    Arg args[10];
    Cardinal n;
    XmString string;
    char *data = stats_text(stats);
    
    /* Only create the interface once - popup it up from then on */
    if(StatsShell != NULL) {
        XmTextSetString(StatsText, data);
        XmTextShowPosition(StatsText, 0);
        XtPopup(StatsShell, XtGrabExclusive);
        return;
    }

    /* Top level shell */
    n = 0; 
    XtSetArg(args[n], XmNminWidth, 350); n++;
    XtSetArg(args[n], XmNminHeight, 300); n++;
    StatsShell = XtCreatePopupShell("StatsShell", xmDialogShellWidgetClass,
      TopLevel, args, n);
    XtAddCallback(StatsShell, XmNpopupCallback, XbrCursorAddCB,
     (XtPointer)XbrGRAB);
    XtAddCallback(StatsShell, XmNpopdownCallback, XbrCursorDeleteCB, NULL);

    /* Form widget - as usual */
    n = 0;
    XtSetArg(args[n], XmNmarginHeight, 5); n++;
    XtSetArg(args[n], XmNmarginWidth, 5); n++;
    XtSetArg(args[n], XmNautoUnmanage, False); n++;
    XtSetArg(args[n], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL); n++;
    StatsForm = XmCreateForm(StatsShell, "StatsForm", args, n);
    XtManageChild(StatsForm);

    /* Push button to close window */
    n = 0;
    string = XmStringCreateLocalized("Close");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
    XtSetArg(args[n], XmNleftPosition, 35); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    StatsClose = XmCreatePushButton(StatsForm, "StatsClose", args, n);
    XmStringFree(string);
    XtAddCallback(StatsClose, XmNactivateCallback, XbrCBPopdownShell, StatsShell);
    XtManageChild(StatsClose);

    /* Separator to make screen look nice */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, StatsClose); n++;
    StatsSeparator = XmCreateSeparator(StatsForm, "StatsSeparator", args, n);
    XtManageChild(StatsSeparator);

    /* Text Widget to hold details */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, StatsSeparator); n++;
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
    XtSetArg(args[n], XmNeditable, False); n++;
    XtSetArg(args[n], XmNcursorPositionVisible, False); n++;
    StatsText = XmCreateText(StatsForm, "StatsText", args, n);
    XtManageChild(StatsText);

    XmTextSetString(StatsText, data);
    XmTextShowPosition(StatsText, 0);

    /* Display it */
    XtPopup(StatsShell, XtGrabExclusive);
}

/*----------------------------------------------------------------------------
  stats_text()
  Convert the data in the XTAR_STATS * structure into some text to place into
  the text widget.
  ----------------------------------------------------------------------------*/
static char *stats_text(XTAR_STATS *stats)
{
    static char buffer[1024];
    int n = 0;

    if(stats->loaded) {
        n += sprintf(&buffer[n], "Archive Name: %s\n\n",
          fname_basename(stats->filename));

        n += sprintf(&buffer[n], "Archive Size:  %s\n",
          ascii_size(stats->size));
        n += sprintf(&buffer[n], "Size Unpacked: %s\n\n",
          ascii_size(stats->size_unpacked));

        n += sprintf(&buffer[n], "#Normal Files: %4d\n",stats->type_normal);
        n += sprintf(&buffer[n], "#Directories:  %4d\n",stats->type_directory);
        n += sprintf(&buffer[n], "#Soft Links:   %4d\n",stats->type_softlink);
        if(stats->type_hardlink)
            n += sprintf(&buffer[n], "#Hard Links: %d\n",
              stats->type_hardlink);
        if(stats->type_character)
            n += sprintf(&buffer[n], "#Character Special Files: %d\n",
              stats->type_character);
        if(stats->type_block)
            n += sprintf(&buffer[n], "#Block Special Files: %d\n",
              stats->type_block);
        if(stats->type_unknown)
            n += sprintf(&buffer[n], "#Unknown File Types: %d\n",
              stats->type_block);
        n += sprintf(&buffer[n], "Total #Files:  %4d\n", stats->nfiles);

        switch(stats->compressed) {
            case _COMPRESSED_NONE:
                n += sprintf(&buffer[n], "\nCompression: None\n");
                break; 
            case _COMPRESSED_Z:
                n += sprintf(&buffer[n],"\nCompression: UNIX .Z Compression\n");
                break; 
            case _COMPRESSED_GZ:
                n += sprintf(&buffer[n],"\nCompression: GZIPPED\n");
                break; 
            case _COMPRESSED_BZ:
                n += sprintf(&buffer[n],"\nCompression: BZIP2\n");
                break; 
        }
    } else {
        strcpy(buffer,"ERROR: No file loaded!\n");
    }

    return(buffer);
}
