/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrHelp.c
 * CREATED:	Mon Oct 24 1994
 * AUTHOR:	Rik Turnbull <rik@stella-consulting.ltd.uk>
 * DESCRIPTION:	Some routines useful for help functions.
 *
 */

/******************************************************************************/
/******************************* INCLUDES *************************************/
/******************************************************************************/
#include <Xm/Xm.h>

#include "XbrHelp.h"



/******************************************************************************/
/****************************** DEFINITIONS ***********************************/
/******************************************************************************/



/******************************************************************************/
/********************** EXTERNAL VARIABLE DECLARATIONS ************************/
/******************************************************************************/



/******************************************************************************/
/************************ EXPORTED FUNCTION PROTOTYPES ************************/
/******************************************************************************/
void XbrHelpSet(Widget, XtEventHandler, XbrHelpData[], int);



/******************************************************************************/
/********************** INTERNAL STRUCTURES & VARIABLES ***********************/
/******************************************************************************/



/******************************************************************************/
/************************ INTERNAL FUNCTION PROTOTYPES ************************/
/******************************************************************************/



/******************************************************************************/
/*************************** EXPORTED FUNCTIONS *******************************/
/******************************************************************************/

/*----------------------------------------------------------------------------
  XbrHelpSet()
  Set the help text for the set of widgets.
  
  Widget w			Widget to begin searching from, for names
  XtEventHandler handler	Event handler to install
  XbrHelpData help_data[]	Array of data to use
  int n				Numberof help items.
  ----------------------------------------------------------------------------*/
void XbrHelpSet(Widget w, XtEventHandler handler, XbrHelpData help_data[], int n)
{
    int i;
    Widget widget;
    
    /* Loop round widgets. */
    for(i = 0; i < n; i++) {
	if(widget = XtNameToWidget(w, help_data[i].name)) {
	    XtAddEventHandler(widget, EnterWindowMask, True, handler,
			      (XtPointer) help_data[i].text);
	    XtAddEventHandler(widget, LeaveWindowMask, True, handler,
			      (XtPointer) "");
	}
    }
    
}
/******************************************************************************/
/*************************** INTERNAL FUNCTIONS *******************************/
/******************************************************************************/
