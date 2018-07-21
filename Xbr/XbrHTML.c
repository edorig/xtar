/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTML.c
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Source code for XbrHTML widget.
 *
 */

#include <assert.h>
#include <stdio.h>

#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>

#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>

#include "XbrHTML.h"
#include "XbrHTMLP.h"
#include "XbrHTMLFormat.h"
#include "XbrHTMLToken.h"

/*****************************************************************************/
/******************************** DEFINES ************************************/
/*****************************************************************************/
#define	_XbrHTML_MIN_WIDTH	640
#define	_XbrHTML_MIN_HEIGHT	600

#define	_XbrHTMLInRegion(w,r,e)	\
	((region == NULL)||(XRectInRegion(region, e->x0+w->html.x_offset-1, \
		e->y0+w->html.y_offset-1, e->width+2, e->height+2) \
			!= RectangleOut))

#define	XbrCHANGED(x) (((XbrHTMLWidget)new)->x!=((XbrHTMLWidget)old)->x)

/*****************************************************************************/
/*************************** FUNCTIONS DECLARATIONS **************************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
  Widget methods.
 ----------------------------------------------------------------------------*/
static void Initialize(Widget, Widget, ArgList, Cardinal *);
static void Redisplay(Widget, XEvent *, Region);
static void Resize(Widget);
static void Destroy(Widget);
static XtGeometryResult GeometryManager(Widget, XtWidgetGeometry *,
  XtWidgetGeometry *);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);

/*----------------------------------------------------------------------------
  Internal functions.
 ----------------------------------------------------------------------------*/
static void XbrHTML_GetGCs(XbrHTMLWidget);
static void XbrHTML_Update(XbrHTMLWidget, int);
static void XbrHTML_Mark(XbrHTMLWidget, int);
static void XbrHTML_FreeGCs(XbrHTMLWidget);
static void _XbrHTMLDraw(XbrHTMLWidget, Region, XbrHTMLElement *, Boolean);
static void _XbrHTMLDrawElement(XbrHTMLWidget, XbrHTMLElement *, Boolean);
static void _XbrHTMLCreateDrawingArea(XbrHTMLWidget);
static void _XbrHTMLCreateScrollBars(XbrHTMLWidget);
static void _XbrHTMLDAExpose(Widget, XtPointer, XtPointer);
static void _XbrHTMLSizeWidgets(XbrHTMLWidget);
static void _XbrHTMLSetScrollBars(XbrHTMLWidget);
static void _XbrHTMLVDrag(Widget, XtPointer, XtPointer);
static void _XbrHTMLHDrag(Widget, XtPointer, XtPointer);
static void _XbrHTMLMotion(Widget, XtPointer, XEvent *, Boolean *);
static void _XbrHTMLClick(Widget, XtPointer, XtPointer);
static void _XbrHTMLHighlightAnchor(XbrHTMLWidget, XbrHTMLElement *, Boolean);
static void _XbrHTMLSetText(XbrHTMLWidget, char *);
static void _XbrHTMLRetestVisited(XbrHTMLWidget);

/*----------------------------------------------------------------------------
  External functions.
 ----------------------------------------------------------------------------*/
Widget XbrCreateHTML(Widget, char *, ArgList, Cardinal);
void XbrHTMLSetText(Widget, char *);
void XbrHTMLRetestVisited(Widget);

/*****************************************************************************/
/****************************** WIDGET RESOURCES  ****************************/
/*****************************************************************************/

static XtResource resources[] = {

    { XmNnormalFont1, XtCNormalFont1, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[0]),
      XtRString, "-adobe-times-medium-r-normal-*-24-*-*-*-*-*-iso8859-1" },

    { XmNboldFont1, XtCBoldFont1, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[1]),
      XtRString, "-adobe-times-bold-r-normal-*-24-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont1, XtCItalicFont1, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[2]),
      XtRString, "-adobe-times-medium-i-normal-*-24-*-*-*-*-*-iso8859-1" },

    { XmNboldItalicFont1,XtCBoldItalicFont1,XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[3]),
      XtRString, "-adobe-times-bold-i-normal-*-24-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont1,XtCFixedNormalFont1,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[4]),
      XtRString, "-adobe-courier-medium-r-normal-*-24-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont1, XtCFixedBoldFont1, XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[5]),
      XtRString, "-adobe-courier-bold-r-normal-*-24-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont1,XtCFixedItalicFont1,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[6]),
      XtRString, "-adobe-courier-medium-o-normal-*-24-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont1, XtCFixedBoldItalicFont1, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[7]),
      XtRString, "-adobe-courier-bold-o-normal-*-24-*-*-*-*-*-iso8859-1" },


    { XmNnormalFont2, XtCNormalFont2, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[8]),
      XtRString, "-adobe-times-medium-r-normal-*-18-*-*-*-*-*-iso8859-1" },

    { XmNboldFont2, XtCBoldFont2, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[9]),
      XtRString, "-adobe-times-bold-r-normal-*-18-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont2, XtCItalicFont2, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[10]),
      XtRString, "-adobe-times-medium-i-normal-*-18-*-*-*-*-*-iso8859-1" },

    { XmNboldItalicFont2,XtCBoldItalicFont2,XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[11]),
      XtRString, "-adobe-times-bold-i-normal-*-18-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont2,XtCFixedNormalFont2,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[12]),
      XtRString, "-adobe-courier-medium-r-normal-*-18-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont2, XtCFixedBoldFont2, XtRFontStruct, sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[13]),
      XtRString, "-adobe-courier-bold-r-normal-*-18-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont2,XtCFixedItalicFont2,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[14]),
      XtRString, "-adobe-courier-medium-o-normal-*-18-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont2, XtCFixedBoldItalicFont2, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[15]),
      XtRString, "-adobe-courier-bold-o-normal-*-18-*-*-*-*-*-iso8859-1" },


    { XmNnormalFont3, XtCNormalFont3, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[16]),
      XtRString, "-adobe-times-medium-r-normal-*-17-*-*-*-*-*-iso8859-1" },

    { XmNboldFont3, XtCBoldFont3, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[17]),
      XtRString, "-adobe-times-bold-r-normal-*-17-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont3, XtCItalicFont3, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[18]),
      XtRString, "-adobe-times-medium-i-normal-*-17-*-*-*-*-*-iso8859-1" },

    { XmNboldItalicFont3,XtCBoldItalicFont3,XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[19]),
      XtRString, "-adobe-times-bold-i-normal-*-17-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont3,XtCFixedNormalFont3,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[20]),
      XtRString, "-adobe-courier-medium-r-normal-*-17-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont3,XtCFixedBoldFont3, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[21]),
      XtRString, "-adobe-courier-bold-r-normal-*-17-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont3,XtCFixedItalicFont3,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[22]),
      XtRString, "-adobe-courier-medium-o-normal-*-17-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont3, XtCFixedBoldItalicFont3, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[23]),
      XtRString, "-adobe-courier-bold-o-normal-*-17-*-*-*-*-*-iso8859-1" },


    { XmNnormalFont4, XtCNormalFont4, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[24]),
      XtRString, "-adobe-times-medium-r-normal-*-14-*-*-*-*-*-iso8859-1" },

    { XmNboldFont4, XtCBoldFont4, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[25]),
      XtRString, "-adobe-times-bold-r-normal-*-14-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont4, XtCItalicFont4, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[26]),
      XtRString, "-adobe-times-medium-i-normal-*-14-*-*-*-*-*-iso8859-1" },

    {XmNboldItalicFont4,XtCBoldItalicFont4,XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[27]),
      XtRString, "-adobe-times-bold-i-normal-*-14-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont4,XtCFixedNormalFont4,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[28]),
      XtRString, "-adobe-courier-medium-r-normal-*-14-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont4, XtCFixedBoldFont4, XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[29]),
      XtRString, "-adobe-courier-bold-r-normal-*-14-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont4,XtCFixedItalicFont4,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[30]),
      XtRString, "-adobe-courier-medium-o-normal-*-14-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont4, XtCFixedBoldItalicFont4, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[31]),
      XtRString, "-adobe-courier-bold-o-normal-*-14-*-*-*-*-*-iso8859-1" },


    { XmNnormalFont5, XtCNormalFont5, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[32]),
      XtRString, "-adobe-times-medium-r-normal-*-12-*-*-*-*-*-iso8859-1" },

    { XmNboldFont5, XtCBoldFont5, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[33]),
      XtRString, "-adobe-times-bold-r-normal-*-12-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont5, XtCItalicFont5, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[34]),
      XtRString, "-adobe-times-medium-i-normal-*-12-*-*-*-*-*-iso8859-1" },

    { XmNboldItalicFont5,XtCBoldItalicFont5,XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[35]),
      XtRString, "-adobe-times-bold-i-normal-*-12-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont5,XtCFixedNormalFont5,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[36]),
      XtRString, "-adobe-courier-medium-r-normal-*-12-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont5,XtCFixedBoldFont5, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[37]),
      XtRString, "-adobe-courier-bold-r-normal-*-12-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont5,XtCFixedItalicFont5,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[38]),
      XtRString, "-adobe-courier-medium-o-normal-*-12-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont5, XtCFixedBoldItalicFont5, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[39]),
      XtRString, "-adobe-courier-bold-o-normal-*-12-*-*-*-*-*-iso8859-1" },


    { XmNnormalFont6, XtCNormalFont6, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[40]),
      XtRString, "-adobe-times-medium-r-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNboldFont6, XtCBoldFont6, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[41]),
      XtRString, "-adobe-times-bold-r-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNitalicFont6, XtCItalicFont6, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[42]),
      XtRString, "-adobe-times-medium-i-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNboldItalicFont6,XtCBoldItalicFont6,XtRFontStruct,sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[43]),
      XtRString, "-adobe-times-bold-i-normal-*-10-*-*-*-*-*-iso8859-1" },

    {XmNfixedNormalFont6,XtCFixedNormalFont6,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[44]),
      XtRString, "-adobe-courier-medium-r-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldFont6,XtCFixedBoldFont6, XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(XbrHTMLWidget, html.fonts[45]),
      XtRString, "-adobe-courier-bold-r-normal-*-10-*-*-*-*-*-iso8859-1" },

    {XmNfixedItalicFont6,XtCFixedItalicFont6,XtRFontStruct,sizeof(XFontStruct*),
      XtOffset(XbrHTMLWidget, html.fonts[46]),
      XtRString, "-adobe-courier-medium-o-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNfixedBoldItalicFont6, XtCFixedBoldItalicFont6, XtRFontStruct,
      sizeof(XFontStruct *), XtOffset(XbrHTMLWidget, html.fonts[47]),
      XtRString, "-adobe-courier-bold-o-normal-*-10-*-*-*-*-*-iso8859-1" },

    { XmNanchorColour, XtCAnchorColour, XtRPixel, sizeof(Pixel),
      XtOffset(XbrHTMLWidget, html.foreground[XbrHTML_FOREGROUND_ANCHOR]),
      XtRString, "Blue" },

    { XmNvisitedColour, XtCVisitedColour, XtRPixel, sizeof(Pixel),
      XtOffset(XbrHTMLWidget, html.foreground[XbrHTML_FOREGROUND_VISITED]),
      XtRString, "Blue4" },

    { XmNselectColour, XtCSelectColour, XtRPixel, sizeof(Pixel),
      XtOffset(XbrHTMLWidget, html.foreground[XbrHTML_FOREGROUND_SELECT]),
      XtRString, "Red" },

    { XmNtitle, XtCTitle, XtRString, sizeof(String),
      XtOffset(XbrHTMLWidget, html.title),
      XtRString, NULL },

    { XmNimageResolver, XmCImageResolver, XtRPointer, sizeof(XtPointer),
      XtOffset(XbrHTMLWidget, html.image_resolver),
      XtRImmediate, NULL },

    { XmNvisitedTest, XmCVisitedTest, XtRPointer, sizeof(XtPointer),
      XtOffset(XbrHTMLWidget, html.visited_test),
      XtRImmediate, NULL },

    { XmNanchorCallback, XmCAnchorCallback, XtRCallback, sizeof(XtCallbackList),
      XtOffset(XbrHTMLWidget, html.anchor_callback),
      XtRImmediate, NULL },

};

/*****************************************************************************/
/**************************** WIDGET CLASS RECORD ****************************/
/*****************************************************************************/
XbrHTMLClassRec xbrHTMLClassRec = {
  {
    (WidgetClass) &xmManagerClassRec,		/* superclass */
    "XbrHTML",					/* class_name */
    sizeof(XbrHTMLRec),				/* widget_size */
    NULL,					/* class_initialize */
    NULL,					/* class_part_initialize */
    False,					/* class_inited */
    Initialize,					/* initialize */
    NULL,					/* initialize_hook */
    XtInheritRealize,				/* realize */
    NULL,					/* actions */
    0,						/* num_actions */
    resources,					/* resources */
    XtNumber(resources),			/* num_resources */
    NULLQUARK,					/* xrm_class */ 
    True,					/* compress_motion */
    True,					/* compress_exposure */
    True,					/* compress_enterleave */
    True,					/* visible_interest */
    Destroy,					/* destroy */
    Resize,					/* resize */
    Redisplay,					/* expose */
    SetValues,					/* set_values */
    NULL,					/* set_values_hook */
    XtInheritSetValuesAlmost,			/* set_values_almost */
    NULL,					/* get_values_hook */
    NULL,					/* accept_focus */
    XtVersion,					/* version */
    NULL,					/* callback private */
    NULL,					/* tm_table */
    XtInheritQueryGeometry,			/* query_geometry */
    NULL,					/* display accelerator */
    NULL,					/* extension */
  },
  {
    (XtGeometryHandler) GeometryManager,	/* geometry_manager   */
    NULL,					/* change_managed     */
    XtInheritInsertChild,			/* insert_child       */
    XtInheritDeleteChild,			/* delete_child       */
    NULL,					/* extension          */
   },
   {
    NULL,					/* resource list        */
    0,						/* num resources        */
    0,						/* constraint size      */
    NULL,					/* init proc            */
    NULL,					/* destroy proc         */
    NULL,					/* set values proc      */
    NULL,					/* extension            */
  },
  {
    XtInheritTranslations,			/* translations           */
    NULL,					/* syn_resources          */
    0,						/* num_syn_resources      */
    NULL,					/* syn_cont_resources     */
    0,						/* num_syn_cont_resources */
    XmInheritParentProcess,			/* parent_process         */
    NULL,					/* extension              */
  },
  {
    0,						/* ignore */
  }
};

WidgetClass xbrHTMLWidgetClass = (WidgetClass) &xbrHTMLClassRec;

/*****************************************************************************/
/***************************** WIDGET METHODS ********************************/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  Initialize()
  This function is called when the widget is created. We use it to set a
  minimum size for the widget and initialized our structure.
  ----------------------------------------------------------------------------*/
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *n)
{
    XbrHTMLWidget req = (XbrHTMLWidget) request;
    XbrHTMLWidget noo = (XbrHTMLWidget) new;
    Arg noo_args[10];
    XGCValues values;
    Cardinal noo_n = 0;
 
    /* Make sure the widget is a decent size. 
    if(req->core.width == 0)
        noo->core.width = 10;
    if(req->core.height == 0)
        noo->core.height = 5;
    */

    /* Initialize structure. */
    noo->html.selected = NULL;    
    noo->html.elements = NULL;    
    noo->html.tokens = NULL;    
    noo->html.x_value = 0;
    noo->html.y_value = 0;
    noo->html.x_offset = 0;
    noo->html.y_offset = 0;
    noo->html.da_width = noo->core.width;
    noo->html.da_height = noo->core.height;
    noo->html.doc_width = noo->core.width;
    noo->html.doc_height = noo->core.height;
    noo->html.foreground[XbrHTML_FOREGROUND_DEFAULT] = noo->manager.foreground;

    /* Allocate memory for anchors - start with 20? */
    noo->html.anchors = (char **) malloc(sizeof(char *) * 20);
    noo->html.nanchors = 1;
    noo->html.maxanchor = 20;

    /* Get normal GC. */
    noo->html.gc = XCreateGC(XbrDisplay(noo), XbrRootWindow(noo), 0, NULL);

    /* Get GC for clearing screen. */
    values.foreground = noo->core.background_pixel;
    values.background = noo->manager.foreground;
    noo->html.clear_gc = XCreateGC(XbrDisplay(noo), XbrRootWindow(noo),
      GCBackground|GCForeground, &values);

    /* Create drawing area widget */
    _XbrHTMLCreateDrawingArea(noo);

    /* Create scroll bars. */
    _XbrHTMLCreateScrollBars(noo);

    /* Set correct sizes */
    _XbrHTMLSizeWidgets(noo);
}

/*-----------------------------------------------------------------------------
  Destroy()
  This function is called when the widget is destroyed. We must free up all
  the resources we have allocated.
  ----------------------------------------------------------------------------*/
static void Destroy(Widget w)
{
    XbrHTMLWidget widget = (XbrHTMLWidget) w;

    /* Free the pixmap from the server. */

    /* Free the GCs we allocated. */
}

/*-----------------------------------------------------------------------------
  Resize()
  This function is called when the widget is resized (am I going too fast?). We
  have to destroy the old pixmap because it's now the wrong size. We create
  a new one and redraw the bar.
  ----------------------------------------------------------------------------*/
static void Resize(Widget w)
{
    XbrHTMLWidget widget = (XbrHTMLWidget) w;

    _XbrHTMLSizeWidgets(widget);

    XbrHTMLReformat(widget);

    _XbrHTMLDraw(widget, NULL, NULL, False);
}

/*-----------------------------------------------------------------------------
  Redisplay()
  This function is called when a part of the widget needs to be redisplayed.
  ----------------------------------------------------------------------------*/
static void Redisplay(Widget w, XEvent *event, Region region)
{
    _XbrHTMLDraw((XbrHTMLWidget)w, NULL, NULL, False);
}

/*-----------------------------------------------------------------------------
  SetValues()
  This function is called when the various versions of XtSetValues() are
  called from within the user's program. If any of the resources referring to
  our GCs are changed then we must free and recreate the GCs. If any of the
  values are changed we just need to redraw the bar.
  ----------------------------------------------------------------------------*/
static Boolean SetValues(Widget old, Widget request, Widget new, ArgList args,
Cardinal *n)
{
    Boolean redraw = False;

    /* If any of the values which are used for the GCs have changed then
       we must free them and create them again.
    if(XbrCHANGED(core.background_pixel) || XbrCHANGED(html.bar_colour) ||
       XbrCHANGED(manager.foreground)  || XbrCHANGED(html.font)) {
        XbrHTML_FreeGCs((XbrHTMLWidget)new);
        XbrHTML_GetGCs((XbrHTMLWidget)new);
        redraw = True;
    }
    */

    return(redraw);
}

/*-----------------------------------------------------------------------------
  GeometryManager()
  Just accept the requested structure.
  ----------------------------------------------------------------------------*/
static XtGeometryResult GeometryManager(
 Widget w,			/* Widget data */
 XtWidgetGeometry *request,	/* Requested geometry specifications */
 XtWidgetGeometry *reply	/* Accepted specifications returned */ )
{
    reply->x = request->x;
    reply->y = request->y;
    reply->width = request->width;
    reply->height = request->height;
    reply->border_width = request->border_width;
    reply->request_mode = request->request_mode;

    return (XtGeometryYes);
}



/*****************************************************************************/
/***************************** INTERNAL FUNCTIONS ****************************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
  XbrHTML_Update()
  This function is the main function for drawing the html bar. It is
  called from the resize function (in which case it recreates the pixmap)
  and from the SetValues() function (in which case the bar changes).
  ----------------------------------------------------------------------------*/
static void XbrHTML_Update(XbrHTMLWidget w, int force)
{
}


/*----------------------------------------------------------------------------
  XbrHTML_GetGCs()
  This function is used to create the various GCs used for drawing operations.
  ----------------------------------------------------------------------------*/
static void XbrHTML_GetGCs(XbrHTMLWidget w)
{
}

/*----------------------------------------------------------------------------
  XbrHTML_FreeGCs()
  This function is used to free up the GCs allocated with XbrHTML_GetGCs().
  ----------------------------------------------------------------------------*/
static void XbrHTML_FreeGCs(XbrHTMLWidget widget)
{
}

/*----------------------------------------------------------------------------
  XbrHTML_Mark()
  This function is used to draw a marker on the bar.
  ----------------------------------------------------------------------------*/
static void XbrHTML_Mark(XbrHTMLWidget w, int marker)
{
}

/*-----------------------------------------------------------------------------
  _XbrHTMLSetText
 Main function called to set the text in the widget.

 XbrHTMLWidget w	Widget to use
 char *data		Data to set
 -----------------------------------------------------------------------------*/
static void _XbrHTMLSetText(XbrHTMLWidget w, char *data)
{
    if(data == NULL||*data == 0)
        return;

    XbrHTMLElementsFree(w, w->html.elements);
    w->html.elements = NULL;

    w->html.tokens = XbrHTMLTokenize(w, data);
    w->html.elements = XbrHTMLFormat(w);

    _XbrHTMLSetScrollBars(w);

    _XbrHTMLDraw(w, NULL, NULL, False);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLRetestVisited()
  Function so that application programmer can call their visited test function
  again. This may be because external events may have caused an anchor to
  have been visited.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLRetestVisited(
 XbrHTMLWidget w	/* Widget data */ )
{
    XbrHTMLElement *element;
    Boolean changed = False;
    int fg;

    /* If they haven't got a visited function then tough titty! */
    if(w->html.visited_test) {
        for(element = w->html.elements; element; element = element->next) {
            if(element->anchor) {
                if(w->html.visited_test(w->html.anchors[element->anchor-1]))
                    fg = XbrHTML_FOREGROUND_VISITED;
                else
                    fg = XbrHTML_FOREGROUND_ANCHOR;
                if(fg != element->foreground) {
                    element->foreground = fg;
                    changed = True;
                }
            }
        }
    }

    /* If we changed the colour of one of the anchors, then we better redraw */
    if(changed)
        _XbrHTMLDraw(w, NULL, NULL, False);
}

/*****************************************************************************/
/***************************** EXTERNAL FUNCTIONS ****************************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
  XbrCreateHTML()
  This is just a wrap around function to make life easier for the user and
  make the widget have a similar front end to other widgets.
  ----------------------------------------------------------------------------*/
Widget XbrCreateHTML(Widget w, char *name, ArgList args, Cardinal n)
{
    return(XtCreateWidget(name, xbrHTMLWidgetClass, w, args, n));
}

void XbrHTMLSetText(Widget w, char *data)
{
    _XbrHTMLSetText((XbrHTMLWidget)w, data);
}

void XbrHTMLRetestVisited(Widget w)
{
    _XbrHTMLRetestVisited((XbrHTMLWidget)w);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLDraw()
  This function actually performs the drawing of the elements previously
  parsed by XbrHTMLTokize() and formatted by XbrHTMLFormat(). At this
  point elements should already have x,y coordinates and a known width and
  height. If region is not NULL then we only draw the elements which are
  present in that region. If anchor is not NULL then we assume that this is
  an anchor that needs un/highlighting.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLDraw(
 XbrHTMLWidget w,	/* Pointer to widget data */
 Region region,		/* Region to redraw or NULL */
 XbrHTMLElement *anchor,/* Start of anchor that needs to be un/highlighted */
 Boolean highlight	/* Highlight this anchor or not? */ )
{
    XbrHTMLElement *element;
    int n;

    if(XtIsRealized(w)) {
        if(anchor) {
            n = anchor->anchor;
            for(element = anchor; element && element->anchor == n;
              element = element->next) {
                if(highlight) {
                    XSetForeground(XbrDisplay(w->html.da), w->html.gc,
                      w->html.foreground[XbrHTML_FOREGROUND_SELECT]);
                } else {
                    XSetForeground(XbrDisplay(w->html.da), w->html.gc,
                      w->html.foreground[element->foreground]);
                }
                XSetFont(XbrDisplay(w->html.da), w->html.gc,
                  w->html.fonts[element->font]->fid);
                _XbrHTMLDrawElement(w, element, highlight);
            }
        } else {
            if(region == NULL)
                XClearArea(XbrDisplay(w->html.da), XbrWindow(w->html.da), 0, 0,
                  w->html.da_width, w->html.da_height, False);
            for(element = w->html.elements; element; element = element->next) {
                if(_XbrHTMLInRegion(w, region, element)) {
                    XSetForeground(XbrDisplay(w->html.da), w->html.gc,
                      w->html.foreground[element->foreground]);
                    XSetFont(XbrDisplay(w->html.da), w->html.gc,
                      w->html.fonts[element->font]->fid);
                    _XbrHTMLDrawElement(w, element, False);
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------------
  _XbrHTMLDrawElement()
  Function to draw a single element.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLDrawElement(
 XbrHTMLWidget w,		/* Widget data */
 XbrHTMLElement *element,	/* Element to draw! */
 Boolean selected		/* Is this a selected element? */ )
{

    switch(element->type) {
        case XbrHTML_TYPE_TEXT:
        case XbrHTML_TYPE_NUMBER:
            XDrawString(XbrDisplay(w->html.da), XbrWindow(w->html.da),
              w->html.gc, element->x+w->html.x_offset,
              element->y+w->html.y_offset, element->text,
              element->length);
            if(element->anchor) {
                _XbrHTMLHighlightAnchor(w, element, selected);
            }
            break;
        case XbrHTML_TYPE_LINE:
            XSetForeground(XbrDisplay(w->html.da), w->html.gc,
              w->manager.top_shadow_color);
            XDrawLine(XbrDisplay(w->html.da), XbrWindow(w->html.da),
              w->html.gc, element->x+w->html.x_offset,
              element->y+w->html.y_offset,
              element->x+w->html.x_offset+element->width,
              element->y+w->html.y_offset);
            XSetForeground(XbrDisplay(w->html.da), w->html.gc,
              w->manager.bottom_shadow_color);
            XDrawLine(XbrDisplay(w->html.da), XbrWindow(w->html.da),
              w->html.gc, element->x+w->html.x_offset,
              element->y+w->html.y_offset+1,
              element->x+w->html.x_offset+element->width,
              element->y+w->html.y_offset+1);
            break;
        case XbrHTML_TYPE_IMAGE:
            if(element->pixmap)
                XCopyArea(XbrDisplay(w->html.da), element->pixmap,
                  XbrWindow(w->html.da), w->html.gc, 0, 0,
                  element->width, element->height,
                  element->x+w->html.x_offset,
                  element->y+w->html.y_offset);
            else
                XDrawRectangle(XbrDisplay(w->html.da), XbrWindow(w->html.da),
                  w->html.gc, element->x+w->html.x_offset,
                  element->y+w->html.y_offset, element->width,
                  element->height);
            break;
        case XbrHTML_TYPE_LIST_ITEM:
            if(element->subtype == XbrHTML_BULLET_DOT)
                XFillArc(XbrDisplay(w->html.da), XbrWindow(w->html.da),
                  w->html.gc, element->x+w->html.x_offset,
                  element->y+w->html.y_offset, element->width, element->height,
                    0, 23040);
            else if(element->subtype == XbrHTML_BULLET_CIRCLE)
                XDrawArc(XbrDisplay(w->html.da), XbrWindow(w->html.da),
                  w->html.gc, element->x+w->html.x_offset,
                  element->y+w->html.y_offset,
                  element->width, element->height, 0, 23040);
            else if(element->subtype == XbrHTML_BULLET_SQUARE)
                XDrawRectangle(XbrDisplay(w->html.da), XbrWindow(w->html.da),
                  w->html.gc, element->x+w->html.x_offset,
                  element->y+w->html.y_offset, element->width,
                  element->height);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------
  _XbrHTMLCreateDrawingArea()
  Build and create the actual drawing area itself.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLCreateDrawingArea(
 XbrHTMLWidget w	/* Widget data */ )
{
    Arg args[10];
    Cardinal n = 0;

    n = 0;
    XtSetArg(args[n], XmNwidth, w->core.width); n++;
    XtSetArg(args[n], XmNheight, w->core.height); n++;
    XtSetArg(args[n], XmNbackground, w->core.background_pixel); n++;
    w->html.da = XmCreateDrawingArea((Widget)w, "DrawingArea", args, n);
    XtAddCallback(w->html.da, XmNexposeCallback, _XbrHTMLDAExpose, w);
    XtAddEventHandler(w->html.da, PointerMotionMask, False, _XbrHTMLMotion, w);
    XtAddEventHandler(w->html.da, LeaveWindowMask, False, _XbrHTMLMotion, w);
    XtAddCallback(w->html.da, XmNinputCallback, _XbrHTMLClick, w);
    XtManageChild(w->html.da);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLCreateScrollBars()
  Build the vertical and horizontal scroll bars.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLCreateScrollBars(
 XbrHTMLWidget w	/* Widget data */ )
{
    Arg args[10];
    Cardinal n = 0;
    Dimension width;

    n = 0;
    XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
    XtSetArg(args[n], XmNheight, w->core.height); n++;
    XtSetArg(args[n], XmNbackground, w->core.background_pixel); n++;
    w->html.vscrollbar = XmCreateScrollBar((Widget)w, "VScrollBar", args, n);
    XtAddCallback(w->html.vscrollbar, XmNdragCallback, _XbrHTMLVDrag, w);
    XtAddCallback(w->html.vscrollbar, XmNdecrementCallback, _XbrHTMLVDrag, w);
    XtAddCallback(w->html.vscrollbar, XmNincrementCallback, _XbrHTMLVDrag, w);
    XtAddCallback(w->html.vscrollbar, XmNpageDecrementCallback,_XbrHTMLVDrag,w);
    XtAddCallback(w->html.vscrollbar, XmNpageIncrementCallback,_XbrHTMLVDrag,w);
    XtManageChild(w->html.vscrollbar);

    n = 0;
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
    XtSetArg(args[n], XmNwidth, w->core.width); n++;
    XtSetArg(args[n], XmNbackground, w->core.background_pixel); n++;
    w->html.hscrollbar = XmCreateScrollBar((Widget)w, "HScrollBar", args, n);
    XtAddCallback(w->html.hscrollbar, XmNdragCallback, _XbrHTMLHDrag, w);
    XtAddCallback(w->html.hscrollbar, XmNdecrementCallback, _XbrHTMLHDrag, w);
    XtAddCallback(w->html.hscrollbar, XmNincrementCallback, _XbrHTMLHDrag, w);
    XtAddCallback(w->html.hscrollbar, XmNpageDecrementCallback,_XbrHTMLHDrag,w);
    XtAddCallback(w->html.hscrollbar, XmNpageIncrementCallback,_XbrHTMLHDrag,w);
    XtManageChild(w->html.hscrollbar);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLDAExpose()
  Expose event called when drawing area widget is exposed and needs redrawing.
  Note that the drawing area completely covers the widget which means the
  Redisplay() method is never called. I think.
  ---------------------------------------------------------------------------*/
static void _XbrHTMLDAExpose(
 Widget w,		/* Widget data */
 XtPointer data,	/* Client data - full HTML widget pointer */
 XtPointer cb		/* Callback data including event details */ )
{
    _XbrHTMLDraw((XbrHTMLWidget)data, NULL, NULL, False);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLSizeWidgets()
  We have to make all the children of the HTML widget fit on the screen nicely.
  Therefore we position them all allowing for default sizes etc.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLSizeWidgets(
 XbrHTMLWidget w	/* Widget data */ )
{
    Dimension width, height;

    if(w->html.vscrollbar) {
        width = w->html.vscrollbar->core.width +
          (2*w->html.vscrollbar->core.border_width);
        XtResizeWidget(w->html.vscrollbar, w->html.vscrollbar->core.width,
          w->core.height-w->html.vscrollbar->core.border_width,
          w->html.vscrollbar->core.border_width);
    } else {
        width = 0;
    }

    if(w->html.hscrollbar) {
        height = w->html.hscrollbar->core.height +
          (2*w->html.hscrollbar->core.border_width);
        XtResizeWidget(w->html.hscrollbar,
          w->core.width-w->html.hscrollbar->core.border_width-width,
          w->html.hscrollbar->core.height,
          w->html.hscrollbar->core.border_width);
    } else {
        height = 0;
    }

    XtResizeWidget(w->html.da,
      w->core.width-width-w->html.da->core.border_width,
      w->core.height-height-w->html.da->core.border_width,
      w->html.da->core.border_width);


    if(w->html.vscrollbar)
        XtMoveWidget(w->html.vscrollbar,
          w->html.da->core.width+w->html.da->core.border_width, 0);

    if(w->html.hscrollbar) {
        XtMoveWidget(w->html.hscrollbar, 0,
          w->html.da->core.height+w->html.da->core.border_width+
          w->html.hscrollbar->core.border_width);
    }

    w->html.da_width = w->html.da->core.width;
    w->html.da_height = w->html.da->core.height;
    w->html.doc_width = w->html.da_width;
    w->html.doc_height = w->html.da_height;
}

/*-----------------------------------------------------------------------------
  _XbrHTMLSetScrollBars()
  We may need to manage/unmanage the vertical and horizontal scroll bars
  or set their value based on the size of the visible document.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLSetScrollBars(
 XbrHTMLWidget w	/* Widget data */ )
{
    Arg args[5];
    Cardinal n;

    /* Deal with vertical scrollbar */
    n = 0;
    if(w->html.doc_height > w->html.da_height) {
        XtSetArg(args[n], XmNmaximum, w->html.doc_height); n++;
    } else {
        XtSetArg(args[n], XmNmaximum, w->html.da_height); n++;
    }
    XtSetArg(args[n], XmNvalue, 0); n++;
    XtSetArg(args[n], XmNsliderSize, w->html.da_height); n++;
    XtSetArg(args[n], XmNpageIncrement, w->html.da_height); n++;
    XtSetValues(w->html.vscrollbar, args, n);

    w->html.x_offset = 0;
    w->html.y_offset = 0;
    w->html.x_value = 0;
    w->html.y_value = 0;

    /* Deal with horizontal scrollbar */
    n = 0;
    if(w->html.doc_width > w->html.da_width) {
        XtSetArg(args[n], XmNmaximum, w->html.doc_width); n++;
    } else {
        XtSetArg(args[n], XmNmaximum, w->html.da_width); n++;
    }
    XtSetArg(args[n], XmNvalue, 0); n++;
    XtSetArg(args[n], XmNsliderSize, w->html.da_width); n++;
    XtSetArg(args[n], XmNpageIncrement, w->html.da_width); n++;
    XtSetValues(w->html.hscrollbar, args, n);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLVDrag()
  Vertical scroll bar has been moved up or down. Move the document.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLVDrag(
 Widget w,		/* Widget data */
 XtPointer data,	/* Client data - full HTML widget pointer */
 XtPointer cb		/* Callback data including event details */ )
{
    XmScrollBarCallbackStruct *sbcb = (XmScrollBarCallbackStruct *) cb;
    XbrHTMLWidget widget = (XbrHTMLWidget) data;
    Region region;
    XPoint points[5];
    int difference;

    widget->html.y_offset = sbcb->value * -1;

    /* Have we scrolled down? */
    if(sbcb->value > widget->html.y_value) {
        /* Found out how much we have scrolled down by. */
        difference = sbcb->value - widget->html.y_value;

        /* If it is greater than a page then we have to redraw the entire
           window. Clear it and set region to NULL (which means draw all)
        */
        if(difference > widget->html.da_height) {
            region = NULL;
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, widget->html.da_width, widget->html.da_height, False);
        } else {
            /* If we have scrolled less then a page, then copy the bottom
               portion to the top and redraw the bottom bit. We build an
               Xlib Region structure of the area to redraw to make it
               faster.
            */
            points[0].x = points[4].x = 0;
            points[0].y = points[4].y = widget->html.da_height-difference;
            points[1].x = widget->html.da_width;
            points[1].y = widget->html.da_height-difference;
            points[2].x = widget->html.da_width;
            points[2].y = widget->html.da_height;
            points[3].x = 0;
            points[3].y = widget->html.da_height;
            region = XPolygonRegion(points, 5, WindingRule);

            /* Copy the area up. */
            XCopyArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              XbrWindow(widget->html.da),widget->html.gc, 0, difference,
              widget->html.da_width, widget->html.da_height-difference, 0, 0);

            /* Clear the area we intend to redraw. */
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, widget->html.da_height-difference, widget->html.da_width,
              difference, False);
        }
    } else {
        /* Looks like we have scrolled up. */
        difference = widget->html.y_value - sbcb->value;

        /* If it is greater than a page then we have to redraw the entire
           window. Clear it and set region to NULL (which means draw all)
        */
        if(difference > widget->html.da_height) {
            region = NULL;
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, widget->html.da_width, widget->html.da_height, False);
        } else {
            /* If we have scrolled less then a page, then copy the top
               portion to the bottom and redraw the top bit. We build an
               Xlib Region structure of the area to make it faster to redraw.
            */
            points[0].x = points[4].x = 0;
            points[0].y = points[4].y = 0;
            points[1].x = widget->html.da_width;
            points[1].y = 0;
            points[2].x = widget->html.da_width;
            points[2].y = difference;
            points[3].x = 0;
            points[3].y = difference;
            region = XPolygonRegion(points, 5, WindingRule);

            /* Copy the area down. */
            XCopyArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              XbrWindow(widget->html.da),widget->html.gc, 0, 0,
              widget->html.da_width, widget->html.da_height-difference, 0,
              difference);

            /* Clear the area we intend to redraw. */
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, widget->html.da_width, difference, False);
        }
    }
    widget->html.y_value = sbcb->value;

    _XbrHTMLDraw(widget, region, NULL, False);

    if(region)
        XDestroyRegion(region);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLHDrag()
  Horizontal scroll bar has been moved left or right. Move the document.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLHDrag(
 Widget w,		/* Widget data */
 XtPointer data,	/* Client data - full HTML widget pointer */
 XtPointer cb		/* Callback data including event details */ )
{
    XmScrollBarCallbackStruct *sbcb = (XmScrollBarCallbackStruct *) cb;
    XbrHTMLWidget widget = (XbrHTMLWidget) data;
    Region region;
    XPoint points[5];
    int difference;

    widget->html.x_offset = sbcb->value * -1;

    /* Have we scrolled right? */
    if(sbcb->value > widget->html.x_value) {
        /* Found out how much we have scrolled right by. */
        difference = sbcb->value - widget->html.x_value;

        /* If it is greater than a page then we have to redraw the entire
           window. Clear it and set region to NULL (which means draw all)
        */
        if(difference > widget->html.da_width) {
            region = NULL;
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, widget->html.da_width, widget->html.da_height, False);
        } else {
            /* If we have scrolled less then a page, then copy the right
               portion to the left and redraw the right bit. We build an
               Xlib Region structure of the area to redraw to make it
               faster.
            */
            points[0].x = points[4].x = widget->html.da_width-difference;
            points[0].y = points[4].y = 0;
            points[1].x = widget->html.da_width;
            points[1].y = 0;
            points[2].x = widget->html.da_width;
            points[2].y = widget->html.da_height;
            points[3].x = widget->html.da_width-difference;
            points[3].y = widget->html.da_height;
            region = XPolygonRegion(points, 5, WindingRule);

            /* Copy the area left. */
            XCopyArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              XbrWindow(widget->html.da),widget->html.gc, difference, 0,
              widget->html.da_width-difference, widget->html.da_height, 0, 0);

            /* Clear the area we intend to redraw. */
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              widget->html.da_width-difference, 0, difference,
              widget->html.da_height, False);
        }
    } else {
        /* Looks like we have scrolled left. */
        difference = widget->html.x_value - sbcb->value;

        /* If it is greater than a page then we have to redraw the entire
           window. Clear it and set region to NULL (which means draw all)
        */
        if(difference > widget->html.da_height) {
            region = NULL;
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, widget->html.da_width, widget->html.da_height, False);
        } else {
            /* If we have scrolled less then a page, then copy the left
               portion to the right and redraw the left bit. We build an
               Xlib Region structure of the area to make it faster to redraw.
            */
            points[0].x = points[4].x = 0;
            points[0].y = points[4].y = 0;
            points[1].x = difference;
            points[1].y = 0;
            points[2].x = difference;
            points[2].y = widget->html.da_height;
            points[3].x = 0;
            points[3].y = widget->html.da_height;
            region = XPolygonRegion(points, 5, WindingRule);

            /* Copy the area right. */
            XCopyArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              XbrWindow(widget->html.da),widget->html.gc, 0, 0,
              widget->html.da_width-difference, widget->html.da_height,
              difference, 0);

            /* Clear the area we intend to redraw. */
            XClearArea(XbrDisplay(widget->html.da), XbrWindow(widget->html.da),
              0, 0, difference, widget->html.da_height, False);
        }
    }
    widget->html.x_value = sbcb->value;

    _XbrHTMLDraw(widget, region, NULL, False);

    if(region)
        XDestroyRegion(region);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLMotion()
 User is moving the pointer and because Monsier Andreesen chose to change the
 cursor to a hand when moving over links we have to do the same.
 -----------------------------------------------------------------------------*/
static void _XbrHTMLMotion(
 Widget w, 		/* Drawing area widget */
 XtPointer data,	/* Widget data */
 XEvent *event,		/* Event that caused this handler */
 Boolean *cont		/* Can't remember but it's boring */ )
{
    Boolean hand = False;
    XMotionEvent *me;
    XbrHTMLElement *element;
    XbrHTMLWidget widget = (XbrHTMLWidget) data;

    /* Was an anchor crossed? */
    if(event->type == MotionNotify) {
        me = (XMotionEvent *) event;
        for(element = widget->html.elements; element; element = element->next) {
            if(element->anchor && (me->x-widget->html.x_offset>element->x0) \
              && (me->x-widget->html.x_offset < element->width+element->x0) \
              && (me->y-widget->html.y_offset > element->y0) \
              && (me->y-widget->html.y_offset < element->height+element->y0)) {
                hand = True;
                break;
            }
        }
    }

    /* Turn hand cursor on/off */
    XbrCursorHand((Widget)widget, hand?1:0);

    /* If the user moved the mouse pointer away from an anchor without
       releasing the mouse button we deselect that anchor.
    */
    if(hand == False && widget->html.selected) {
        _XbrHTMLDraw(widget, NULL, widget->html.selected, False);
        widget->html.selected = False;
    }
}

/*-----------------------------------------------------------------------------
  _XbrHTMLClick()
  User has clicked or released the mouse button inside the drawing area.
  If they have pressed the button on a link then highlight that link. If
  they have released the button on the link then put it back to normal and
  activate the link.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLClick(
 Widget w,		/* Drawing area widget */
 XtPointer data,	/* Full widget data */
 XtPointer cb		/* Callback data */ )
{
    XmDrawingAreaCallbackStruct *dacb = (XmDrawingAreaCallbackStruct *) cb;
    XButtonEvent event = dacb->event->xbutton;
    XbrHTMLWidget widget = (XbrHTMLWidget) data;
    XbrHTMLElement *selected = NULL, *element;
    XbrHTMLCallbackStruct callback;
    int n;

    /* Only interested in the left mouse button */
    if(event.button == 1) {
        /* If they released the button and there is an anchor selected, then
           draw the anchor normally.
        */
        if(event.type == ButtonRelease && widget->html.selected) {
            _XbrHTMLDraw(widget, NULL, widget->html.selected, False);
            callback.reason = XmCR_ANCHOR;
            callback.event = dacb->event;
            callback.window = XtWindow(w);
            callback.href=widget->html.anchors[widget->html.selected->anchor-1];
            callback.text = widget->html.selected->text;
            XtCallCallbackList((Widget)widget, widget->html.anchor_callback,
              &callback);
            widget->html.selected = NULL;
        } else if(event.type == ButtonPress) {
            /* If they pressed the button then we need to find the start of the
               anchor they clicked on - if they clicked on an anchor that is.
            */
            for(element=widget->html.elements;element;element=element->next) {
                /* New anchor */
                if(element->anchor != n) {
                    selected = element;
                    n = element->anchor;
                }

                /* Has this element been clicked on? */
                if(element->anchor
                 &&(event.x-widget->html.x_offset>element->x0)
                 &&(event.x-widget->html.x_offset<element->width+element->x0)
                 &&(event.y-widget->html.y_offset>element->y0)
                 &&(event.y-widget->html.y_offset<element->height+element->y0)){
                    widget->html.selected = selected;
                    break;
                }
            }

            /* If there is something selected, then redraw it */
            if(widget->html.selected)
                _XbrHTMLDraw(widget, NULL, widget->html.selected, True);
        }
    }
}

/*-----------------------------------------------------------------------------
  XbrHTMLHighlightAnchor()
  This is still really expiremental - 3D borders look quite snazzy but or
  a little bit difficult to do. Also, underlining is a bit naff because
  there's not a lot you can do about the <U> underline tag.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLHighlightAnchor(
 XbrHTMLWidget w,		/* Widget data */
 XbrHTMLElement *element,	/* Element we are highlighting */
 Boolean selected		/* Is this selected or deselected */ )
{
    if(selected)
        XSetForeground(XbrDisplay(w),w->html.gc,w->manager.top_shadow_color);
    else
        XSetForeground(XbrDisplay(w),w->html.gc,w->manager.bottom_shadow_color);
    XDrawLine(XbrDisplay(w), XbrWindow(w->html.da), w->html.gc,
      element->x0+w->html.x_offset,
      element->y0+w->html.y_offset+element->height,
      element->x0+w->html.x_offset+element->width,
      element->y0+w->html.y_offset+element->height);
    XDrawLine(XbrDisplay(w), XbrWindow(w->html.da), w->html.gc,
     element->x0+w->html.x_offset+element->width,
     element->y0+w->html.y_offset,
     element->x0+w->html.x_offset+element->width,
     element->y0+w->html.y_offset+element->height);
    if(selected)
        XSetForeground(XbrDisplay(w),w->html.gc,w->manager.bottom_shadow_color);
    else
        XSetForeground(XbrDisplay(w),w->html.gc,w->manager.top_shadow_color);
    XDrawLine(XbrDisplay(w), XbrWindow(w->html.da), w->html.gc,
      element->x0+w->html.x_offset+element->width,
      element->y0+w->html.y_offset,
      element->x0+w->html.x_offset,
      element->y0+w->html.y_offset);
    XDrawLine(XbrDisplay(w), XbrWindow(w->html.da), w->html.gc,
      element->x0+w->html.x_offset,
      element->y0+w->html.y_offset,
      element->x0+w->html.x_offset,
      element->y0+w->html.y_offset+element->height);
}
