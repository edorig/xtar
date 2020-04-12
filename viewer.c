/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	viewer.c
 * CREATED:	Tue 26 Jan 1999
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Implementation of internal text file viewer.sity logo and my
 *
 */
#include <stdlib.h> 
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>

#include "Xbr.h"
#include "xtarint.h"
#include "common.h"

#define XTAR_LAUNCH_TEXT        "Press \'Launch External...\' to view this file with the command:\n\n"

extern Widget TopLevel;

typedef struct tar_show {
    Widget ViewerShell;
    Widget ViewerForm;
    Widget ViewerClosePB;
    Widget ViewerSavePB;
    Widget ViewerText;
    Widget ViewerNameLabel;
    Widget ViewerPrevPB;
    Widget ViewerNextPB;
    Widget ViewerLaunchExternalPB;
    Widget ViewerMenuBar;
    Widget ViewerWindow;

    Widget *file_menu;

    TAR_FILES *tfs;
    TAR_FILES *displaying;

    char *tmpdir;

    struct tar_show *next;
} TAR_SHOW;

static TAR_SHOW *first_ts = NULL;
static TAR_SHOW *current_ts = NULL;

/*****************************************************************************/
/* Viewer File->Viewer menu - actions for operations on single files.      */
/*****************************************************************************/
#define	VIEWER_Pulldown		0
#define	VIEWER_Cascade		1
#define	VIEWER_Previous		2
#define	VIEWER_Next		3
#define	VIEWER_Separator1	4
#define	VIEWER_Launch		5
#define	VIEWER_SaveAs		6
#define	VIEWER_Separator2	7
#define	VIEWER_Close		8

static XbrMenuData viewer_menu_data[] = {
    { XbrPB,        "Previous",        NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrPB,        "Next",            NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrSEPARATOR, NULL,              NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrPB,        "Launch External...", NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrPB,        "Save As...",      NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrSEPARATOR, NULL,              NULL,  NULL, 0, NULL, NULL, 0, 0 },
    { XbrPB,        "Close",           NULL,  NULL, 0, NULL, NULL, 0, 0 },
};

/*************************** FUNCTION PROTOTYPES *****************************/
int viewer_open(TAR_FILES *, char *);
void viewer_close();

static void viewer_show(TAR_SHOW *, TAR_FILES *); /* Display file contents */
static void viewer_launch(char *, char *);        /* Run external program    */
static void viewer_freets(TAR_SHOW *, int);       /* Free TAR_SHOW structure */

/*************************** CALLBACK PROTOTYPES *****************************/
static void viewer_saveCB(Widget, XtPointer, XtPointer);
static void viewer_nextCB(Widget, XtPointer, XtPointer);
static void viewer_prevCB(Widget, XtPointer, XtPointer);
static void viewer_launchCB(Widget, XtPointer, XtPointer);
static void viewer_destroyCB(Widget, XtPointer, XtPointer);

int viewer_open(TAR_FILES *tar_files, char *tmpdir)
{
    TAR_SHOW *ts;
    TAR_FILES *tfs;
    char buffer[256], *data;
    Cardinal n;
    Arg args[10];
    XmString string;
    int size, count = 0;

    /* Found out how many files are selected - bit of a naff way though */
    for(tfs = tar_files, count = 0; tfs != NULL; tfs = tfs->next, count++);
    if(count == 1 && mappings_execute(tar_files->tar_file->name,NULL,0)!=NULL) {
        viewer_launch(tmpdir, tar_files->tar_file->name);
        return(1);
    }

    /* Allocate memory for a new viewer data structure */
    ts = (TAR_SHOW *) mem_calloc(sizeof(TAR_SHOW));
    ts->displaying = NULL;
    ts->tmpdir = tmpdir;
    ts->tfs = tar_files;

    /* We maintain a linked list of viewers */
    if(first_ts == NULL) {
        first_ts = ts;
        current_ts = first_ts;
    } else {
        current_ts->next = ts;
        current_ts = current_ts->next;
    }

    /* ViewerShell. XtTOPLEVELSHELL. */
    ts->ViewerShell =  XtCreatePopupShell("Viewer", topLevelShellWidgetClass,
      TopLevel, NULL, 0);
    XtAddCallback(ts->ViewerShell, XmNpopupCallback, XbrCursorAddCB, XbrNOGRAB);
    XtAddCallback(ts->ViewerShell, XmNdestroyCallback, XbrCursorDeleteCB, NULL);

    /* ViewerWindow. XmMAINWINDOW. */
    n = 0;
    XtSetArg(args[n], XmNshowSeparator, True); n++;
    ts->ViewerWindow = XmCreateMainWindow(ts->ViewerShell, "ViewerWindow",
      args, n);
    XtManageChild(ts->ViewerWindow);

    /* ViewerMenuBar. XmMENUBAR. */
    ts->ViewerMenuBar = XmCreateMenuBar(ts->ViewerWindow, "ViewerMenuBar",
      NULL, 0);
    XtManageChild(ts->ViewerMenuBar);

    /* ViewerForm. XmFORM. */
    n = 0;
    XtSetArg(args[n], XmNmarginWidth, 5); n++;
    XtSetArg(args[n], XmNmarginHeight, 5); n++;
    ts->ViewerForm = XmCreateForm(ts->ViewerWindow, "ViewerForm", args, n);
    XtManageChild(ts->ViewerForm);

    /* Set menu bar and work area - set remainder to NULL */
    XmMainWindowSetAreas(ts->ViewerWindow, ts->ViewerMenuBar, NULL, NULL, NULL,
      ts->ViewerForm);

    ts->file_menu = XbrMenu(ts->ViewerMenuBar, "XTar Viewer", XbrMENU_PULLDOWN,
      viewer_menu_data, XtNumber(viewer_menu_data));

    /* ViewerPrevPB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("Previous");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    ts->ViewerPrevPB = XmCreatePushButton(ts->ViewerForm, "ViewerPrevPB",
      args, n);
    XtAddCallback(ts->ViewerPrevPB, XmNactivateCallback, viewer_prevCB, ts);
    XtAddCallback(ts->file_menu[VIEWER_Previous], XmNactivateCallback,
      viewer_prevCB, ts);
    XbrTooltipsRegister(TopLevel, ts->ViewerPrevPB, "Display previous file");
    XmStringFree(string);
    XtManageChild(ts->ViewerPrevPB);

    /* ViewerNextPB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("Next");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, ts->ViewerPrevPB); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    ts->ViewerNextPB = XmCreatePushButton(ts->ViewerForm, "ViewerNextPB",
      args, n);
    XtAddCallback(ts->ViewerNextPB,XmNactivateCallback,viewer_nextCB,ts);
    XtAddCallback(ts->file_menu[VIEWER_Next], XmNactivateCallback,
      viewer_nextCB, ts);
    XbrTooltipsRegister(TopLevel, ts->ViewerNextPB, "Display next file");
    XmStringFree(string);
    XtManageChild(ts->ViewerNextPB);

    /* ViewerLaunchExternalPB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("Launch External...");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, ts->ViewerNextPB); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    XtSetArg(args[n], XmNsensitive, False); n++;
    ts->ViewerLaunchExternalPB = XmCreatePushButton(ts->ViewerForm,
      "ViewerLaunchExternalPB", args, n);
    XmStringFree(string);
    XtAddCallback(ts->ViewerLaunchExternalPB, XmNactivateCallback,
      viewer_launchCB, ts);
    XtAddCallback(ts->file_menu[VIEWER_Launch], XmNactivateCallback,
      viewer_launchCB, ts);
    XbrTooltipsRegister(TopLevel, ts->ViewerLaunchExternalPB,
      "Display file contents in external viewer");
    XtManageChild(ts->ViewerLaunchExternalPB);

    /* ViewerClosePB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("Close");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    ts->ViewerClosePB = XmCreatePushButton(ts->ViewerForm, "ViewerClosePB",
      args, n);
    XmStringFree(string);
    XtAddCallback(ts->ViewerClosePB, XmNactivateCallback, viewer_destroyCB, ts);
    XtAddCallback(ts->file_menu[VIEWER_Close], XmNactivateCallback,
      viewer_destroyCB, ts);
    XbrTooltipsRegister(TopLevel, ts->ViewerClosePB, "Close viewer window");
    XtManageChild(ts->ViewerClosePB);

    /* ViewerSavePB. XmPUSHBUTTON. */
    n = 0;
    string = XmStringCreateLocalized("Save As...");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNrightWidget, ts->ViewerClosePB); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    ts->ViewerSavePB = XmCreatePushButton(ts->ViewerForm, "ViewerSavePB",
      args, n);
    XbrTooltipsRegister(TopLevel, ts->ViewerSavePB,
      "Save current file contents");
    XmStringFree(string);
    XtManageChild(ts->ViewerSavePB);

    /* ViewerNameLabel. XmLABEL. */
    n = 0;
    string = XmStringCreateLocalized("Filename:");
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
    XtSetArg(args[n], XmNlabelString, string); n++;
    ts->ViewerNameLabel = XmCreateLabel(ts->ViewerForm, "ViewerName", args, n);
    XmStringFree(string);
    XtManageChild(ts->ViewerNameLabel);

    /* ViewerText. XmSCROLLEDTEXT. */
    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNtopWidget, ts->ViewerSavePB); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNbottomWidget, ts->ViewerNameLabel); n++;
    XtSetArg(args[n], XmNbottomOffset, 5); n++;
    XtSetArg(args[n], XmNeditable, False); n++;
    XtSetArg(args[n], XmNcursorPositionVisible, True); n++;
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
    ts->ViewerText = XmCreateScrolledText(ts->ViewerForm,"ViewerText",args,n);
    XtAddCallback(ts->ViewerSavePB, XmNactivateCallback, viewer_saveCB, ts);
    XtAddCallback(ts->file_menu[VIEWER_SaveAs], XmNactivateCallback,
      viewer_saveCB, ts);

    /* Display the first file */
    viewer_show(ts,ts->tfs);

    /* Display text */
    XtManageChild(ts->ViewerText);

    /* Show it */
    XtPopup(ts->ViewerShell, XtGrabNone);

    /* OK. */
    return(1);
}

/*----------------------------------------------------------------------------
  viewer_close()
  Close down all the currently open viewers and free the associated memory.
  ---------------------------------------------------------------------------*/
void viewer_close()
{
    TAR_SHOW *ts;

    /* Free items in list one at a time. */
    ts = first_ts;
    while(ts != NULL) {
        /* Close the window */
        if(ts->ViewerShell) {
            XtPopdown(ts->ViewerShell);
            XtDestroyWidget(ts->ViewerShell);
        }

        /* Remove any temporary files */
        file_remove(ts->tmpdir);

        /* Free the structure */
        current_ts = ts->next;
        viewer_freets(ts,1);
        ts = current_ts;
    }
    first_ts = NULL;
    current_ts = NULL;
}

/* viewer_show:*************************************************************/
/* viewer_show: Display a particular file, in the internal file viewer.    */
/* viewer_show:*************************************************************/
static void viewer_show(TAR_SHOW *ts, TAR_FILES *tfs)
{
    int size;
    TAR_FILE *tf;
    char *data, filename[256], *mapcommand, command[512];

    if(tfs != NULL) {
        tf = tfs->tar_file;

        sprintf(filename, "%s/%s", ts->tmpdir, tf->name);

        /* Load the file */
        if(!stat_isdir(filename)) {
            /* Do we require an external loader */
            mapcommand = mappings_execute(tf->name, NULL, 0);
            if(mapcommand) {
                /* Set the filename label. */
                XbrLabelSet(ts->ViewerNameLabel, "Filename: %s (%s)",
                  tf->name, ascii_size(tf->size));

                sprintf(command, mapcommand, fname_basename(tf->name));
                data = (char *) XtMalloc(strlen(XTAR_LAUNCH_TEXT)+strlen(command)+20);
                sprintf(data, "%s %s", XTAR_LAUNCH_TEXT, command);

                /* Load text into widget */
                XmTextSetString(ts->ViewerText, data);
                XmTextShowPosition(ts->ViewerText,0);

                XtFree(data);

                XtSetSensitive(ts->ViewerLaunchExternalPB, True);
                XtSetSensitive(ts->file_menu[VIEWER_Launch], True);
            } else {
                data = file_load(filename, &size);
                if(data != NULL) {
                    /* Set the filename label. */
                    XbrLabelSet(ts->ViewerNameLabel, "Filename: %s (%s)",
                      tf->name, ascii_size(size));

                    /* Load text into widget */
                    XmTextSetString(ts->ViewerText, data);
                    XmTextShowPosition(ts->ViewerText,0);

                    /* Free data */
                    XtFree(data);
                } else {
                    /* Set the filename label. */
                    XbrLabelSet(ts->ViewerNameLabel, "Filename: LOAD FAILED");

                    /* Reset text in widget */
                    XmTextSetString(ts->ViewerText, "");
                }
                XtSetSensitive(ts->ViewerLaunchExternalPB, False);
                XtSetSensitive(ts->file_menu[VIEWER_Launch], False);
            }
            ts->displaying = tfs;
        }
 
        if(tfs->prev == NULL) {
            XtSetSensitive(ts->ViewerPrevPB, False);
            XtSetSensitive(ts->file_menu[VIEWER_Previous], False);
        } else {
            XtSetSensitive(ts->ViewerPrevPB, True);
            XtSetSensitive(ts->file_menu[VIEWER_Previous], True);
        }

        if(tfs->next == NULL) {
            XtSetSensitive(ts->ViewerNextPB, False);
            XtSetSensitive(ts->file_menu[VIEWER_Next], False);
        } else {
            XtSetSensitive(ts->ViewerNextPB, True);
            XtSetSensitive(ts->file_menu[VIEWER_Next], True);
        }

    }
}

/* viewer_launch:*************************************************************/
/* viewer_launch: Launch an external program to view a particular file.      */
/* viewer_launch:*************************************************************/
static void viewer_launch(char *tmpdir, char *filename)
{
    char fname[256];

    sprintf(fname,"%s/%s", tmpdir, filename);
    mappings_execute(filename, fname, 1);
}

/* viewer_freets:*************************************************************/
/* viewer_freets: Free all memory allocated to a TAR_SHOW structure.         */
/* viewer_freets:*************************************************************/
static void viewer_freets(TAR_SHOW *ts, int all)
{
    ts->ViewerShell = NULL;

    if(ts->file_menu) {
        free(ts->file_menu);
        ts->file_menu = NULL;
    }

    if(ts->tfs) {
        xtar_freetfs(ts->tfs);
        ts->tfs = NULL;
    }

    if(all)
        free(ts);
}



/********************************* CALLBACKS *********************************/

/* viewer_saveCB:*************************************************************/
/* viewer_saveCB: Dump the contents of the text widget to a file.            */
/* viewer_saveCB:*************************************************************/
static void viewer_saveCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    TAR_SHOW *ts = (TAR_SHOW *) client_data;
    TAR_FILE *tf;
    char *outname, inname[512], buffer[512];
    int status;

    if(ts) {
        outname = XbrDialogFSB(ts->ViewerShell);
        if(outname == NULL)
            return;

        tf = ts->displaying->tar_file;
        sprintf(inname, "%s/%s", ts->tmpdir, tf->name);
        printf("INPUT FILE: %s\n", inname);
        printf("OUTPUT FILE: %s\n", outname);

        if(file_exists(outname) && stat_isdir(outname)) {
            sprintf(buffer, "%s/%s", outname, fname_basename(inname));
            XtFree(outname);
            outname = mem_strdup(buffer);
        }

        if(file_exists(outname) && !stat_isdir(outname)) {
            if(XbrDialogQuestion(ts->ViewerShell, "Yes", "No",
              "File \"%s\" already exists! \nOverwrite?", outname) == 0) {
                XtFree(outname);
                return;
            }
        }

        XbrCursorBusy(TopLevel);
        status = file_copy(inname, outname);
        XbrCursorReset(TopLevel);

        if(status == 0)
            XbrDialogError(ts->ViewerShell, "Error copying file!");

        printf("INPUT FILE: %s\n", inname);
        printf("OUTPUT FILE: %s\n", outname);

        XtFree(outname);
    }
}
            
/* viewer_nextCB:***********************************************************/
/* viewer_nextCB: In the file display window - scroll to the next file.    */
/* viewer_nextCB:***********************************************************/
static void viewer_nextCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    TAR_SHOW *ts = (TAR_SHOW *) client_data;

    if(ts->displaying && ts->displaying->next)
        viewer_show(ts, ts->displaying->next);
}

/* viewer_prevCB:*************************************************************/
/* viewer_prevCB: In the file display window - scroll to the previous file.  */
/* viewer_prevCB:*************************************************************/
static void viewer_prevCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    TAR_SHOW *ts = (TAR_SHOW *) client_data;

    if(ts->displaying && ts->displaying->prev)
        viewer_show(ts, ts->displaying->prev);
}

/* viewer_launchCB:***********************************************************/
/* viewer_launchCB: Launch an external programm when button pressed.         */
/* viewer_launchCB:**********************************************************/
static void viewer_launchCB(Widget w,XtPointer client_data,XtPointer call_data)
{
    TAR_SHOW *ts = (TAR_SHOW *) client_data;

    viewer_launch(ts->tmpdir, ts->displaying->tar_file->name);
}

/* viewer_destroyCB:**********************************************************/
/* viewer_destroyCB: When view is closed - free up associated memory.        */
/* viewer_destroyCB:**********************************************************/
static void viewer_destroyCB(Widget w,XtPointer client_data,XtPointer call_data)
{
    TAR_SHOW *ts = (TAR_SHOW *) client_data;

    XtPopdown(ts->ViewerShell);
    XtDestroyWidget(ts->ViewerShell);
    file_remove(ts->tmpdir);
    viewer_freets(ts,0);
}
