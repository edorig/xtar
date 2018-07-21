/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrRes.c
 * CREATED:	Thu 19 Sep 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION:	Routines for handling Motif resources.
 *
 */

#include <Xm/Xm.h>

char *XbrResGetString(Widget, char *);

/*----------------------------------------------------------------------------
  XbrResGetString()
  Convert an XmString to a normal text string.
  
  Widget w	Widget to use as the source
  char *name	Name of resource to grab as an XmString
  ----------------------------------------------------------------------------*/
char *XbrResGetString(Widget w, char *name)
{
    XmString string;
    char *retval;
    
    XtVaGetValues(w, name, &string, NULL);
    
    XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &retval);
    
    return(retval);
}
