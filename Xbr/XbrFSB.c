/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrFSB.c
 * CREATED:	Fri Oct 29 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Routines for handling the Motif XmFileSelectionBox widget.
 *
 */

#include <Xm/Xm.h>
#include <Xm/FileSB.h>

char *XbrFSBSelection(Widget);

/* XbrFSBSelection:***********************************************************/
/* XbrFSBSelection: Get the selected file name out of the File Selection Box */
/* XbrFSBSelection:***********************************************************/

char *XbrFSBSelection(Widget w)
{
    char *filename;
    XmString MyString;

    XtVaGetValues(w, XmNdirSpec, &MyString, NULL);
    XmStringGetLtoR(MyString, XmSTRING_DEFAULT_CHARSET, &filename);

    return(filename);
}
