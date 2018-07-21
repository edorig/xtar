/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	resources.h
 * CREATED:	Tue Jun 7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: Header file for xtar resources.
 *
 */

/*
 * Fri Aug 12 1994
 * Added resource to handle mappings.
 *
 */

#ifndef XTAR_RESOURCE_H
#define XTAR_RESOURCE_H		1

#include <X11/StringDefs.h>

static char *fallback_resources[] = {
"*fontList:	-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1",
"*FileList.fontList:	fixed",
"*XmText.fontList:	fixed",
"*title:		XTar V1.5b2",
"*iconName:		XTar V1.5b2",
"*background:		Grey75",
"*foreground:		Black",
/*
"*TarWindow.width:      770",
"*TarWindow.height:     600",
"*FileForm.width:       770",
"*FileForm.height:      640",
"*HelpForm.width:       770",
"*HelpForm.height:      640",
*/
"*TarWindow.width:      360",
"*TarWindow.height:     300",
"*FileForm.width:       360",
"*FileForm.height:      320",
"*HelpForm.width:       360",
"*HelpForm.height:      320",
"*XbrTooltipsShell*background: #FFFFFFFFAAAA",
"*XmRowColumn.background:      #9F009B00BF00",
"*XmCascadeButton.background:  #9F009B00BF00",
"*XmToggleButton*visibleWhenOff:True",
"*XmToggleButton*indicatorSize:12",
"*XmToggleButton*selectColor:Red",
"*FileList*Translations:     #override \n<Key>n:   next()\n\
                                             <Key>/:   search()",
NULL
};

#define	XTAR_MAPPINGS	".gif display %s\n.jpg display %s"

struct XtarResources {
    char *mappings;
    char *helpDoc;
};

static struct XtarResources xtar_resources;

static XtResource resources[] = {
    { "mappings", "Mappings", XtRString,  sizeof(char *),
      XtOffsetOf(struct XtarResources, mappings),
      XtRString, XTAR_MAPPINGS },
    { "helpDoc", "HelpDoc", XtRString,  sizeof(char *),
      XtOffsetOf(struct XtarResources, helpDoc),
      XtRString, "/opt/xtar-1.5b/docs/index.html" }
};

#endif
