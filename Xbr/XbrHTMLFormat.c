/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLFormat.c
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Code for taking lexical tokens and creating actual elements
 *              which can be draw quickly later.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>

#include <Xm/Xm.h>

#include "Xbr.h"
#include "XbrHTML.h"
#include "XbrHTMLP.h"
#include "XbrHTMLTags.h"
#include "XbrHTMLToken.h"

#include "common.h"

/*****************************************************************************/
/******************************** DEFINES ************************************/
/*****************************************************************************/
#define	XbrHTMLFORMAT_LINE_HEIGHT 	15
#define	XbrHTMLFORMAT_IMAGE_SIZE	20
#define	XbrHTMLFORMAT_LEFT_MARGIN	10
#define	XbrHTMLFORMAT_INDENTATION	60
#define	XbrHTMLFORMAT_BULLET_SIZE	10
#define	XbrHTML_BULLET_BOTTOM_OFFSET	0
#define	XbrHTML_BULLET_RIGHT_OFFSET	10
#define	_XbrHTMLDefaultFontSize		2
#define _XbrHTMLLowercase(c)             ((c>64&&c<91)?c+32:c)
#define _XbrHTMLFormatAddToList(a,e)	\
	if(e) { \
		if(e->type == XbrHTML_TYPE_NEWLINE) \
			a->newlines++; \
		else \
			a->newlines = 0; \
		if(a->first) \
			a->current->next = e; \
		else \
			a->first = e; \
		a->current = e; \
	} 0

#define _XbrHTMLFormatAdjustBaseline(attributes, element, base, b, bs) \
	if(element->type == XbrHTML_TYPE_TEXT) { \
		if((bs)<(attributes->bullet_size)) \
			attributes->bullet_size = bs; \
	} \
	if((base)>(attributes->baseline)) \
		attributes->baseline = base; \
	if((b)>(attributes->bottom)) \
		attributes->bottom = b; \
	if(attributes->start == NULL) \
		attributes->start = element

#define	XbrHTMLFormat_STRING_BUFFERED		1
#define	XbrHTMLFormat_NO_STRING_BUFFERED	2
#define	XbrHTMLFormat_START_OF_LINE		3

#define	XbrHTMLFormat_X_UNDEFINED		-1
#define	XbrHTMLFormat_Y_UNDEFINED		-1
#define	XbrHTMLFormat_WIDTH_UNDEFINED		-1
#define	XbrHTMLFormat_HEIGHT_UNDEFINED		-1

/*****************************************************************************/
/******************************** STRUCTURES *********************************/
/*****************************************************************************/
typedef struct _XbrHTMLFormatData
{
    int x, y;			/* Current x,y coordinates */
    int width;			/* Current width of document */
    int left_margin;		/* Current location of left margin */

    int font;			/* Index into font table for current font */
    int foreground;		/* Index into colours */

    int newlines;		/* Current number of blank lines */

    int baseline;		/* Suggested baseline for current line */
    int bottom;			/* Suggested descent for current line */
    int bullet_size;		/* Suggested size of bullet points */

    int li_type;		/* Type of bullet - solid, circle or square */
    int li_count;		/* Number of bullet */

    int anchor;			/* Number of current anchor or zero */

    Boolean preformatted;	/* Is this currently preformatted text? */

    XbrHTMLElement *start;	/* Element which is first on current line */
    XbrHTMLElement *first;	/* Pointer to first element in list */
    XbrHTMLElement *current;	/* Pointer to current element in list */
} _XbrHTMLFormatData;



/*****************************************************************************/
/*************************** FUNCTIONS DECLARATIONS **************************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
  Internal functions.
 ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeText(XbrHTMLWidget, XbrHTMLToken *,
  _XbrHTMLFormatData *);
static XbrHTMLElement *_XbrHTMLTypePreformat(XbrHTMLWidget, XbrHTMLToken *,
  _XbrHTMLFormatData *);
static XbrHTMLElement *_XbrHTMLTypeListItem(XbrHTMLWidget, _XbrHTMLFormatData *,  Boolean);
static XbrHTMLElement *_XbrHTMLTypeLine(_XbrHTMLFormatData *);
static XbrHTMLElement *_XbrHTMLTypeImage(XbrHTMLWidget, XbrHTMLToken *,
 _XbrHTMLFormatData *);
static XbrHTMLElement *_XbrHTMLTypeNewLine(_XbrHTMLFormatData *, int);
static XbrHTMLElement *_XbrHTMLTypeNumber(int);
static XbrHTMLElement *_XbrHTMLFormatNewElement(char *, int, int, int,
  _XbrHTMLFormatData *, XbrHTMLElement *);
static void _XbrHTMLFormatLineFeed(XbrHTMLWidget, _XbrHTMLFormatData *);
static char *_XbrHTMLFormatValue(char *, char *);
static void _XbrHTMLFormatInit(XbrHTMLWidget, _XbrHTMLFormatData *);
static XbrHTMLElement *_XbrHTMLFormatFragment(XbrHTMLWidget,
 _XbrHTMLFormatData *, XbrHTMLElement *element);

/*----------------------------------------------------------------------------
  External functions.
 ----------------------------------------------------------------------------*/
XbrHTMLElement *XbrHTMLFormat(XbrHTMLWidget);
void XbrHTMLElementsFree(XbrHTMLWidget, XbrHTMLElement *);
void XbrHTMLReformat(XbrHTMLWidget);


/*****************************************************************************/
/***************************** EXTERNAL FUNCTIONS ****************************/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  XbrHTMLFormat()
  Take the parsed tokens and create drawing elements such as text, circles
  pixmaps etc, which can be drawn quickly later.

  XbrHTMLWidget w        Widget to use
 -----------------------------------------------------------------------------*/
XbrHTMLElement *XbrHTMLFormat(XbrHTMLWidget w)
{
    XbrHTMLToken *token;
    Boolean title = False, bullet = True;
    Stack *items = StackCreate(20);
    int font_style = XbrHTML_FONT_NORMAL, font_size = _XbrHTMLDefaultFontSize;
    int indent = 0, item, newlines = 0;
    char *href;
    _XbrHTMLFormatData fd, *attributes;

    attributes = &fd;

    _XbrHTMLFormatInit(w, attributes);

    for(token = w->html.tokens; token; token = token->next) {
        switch(token->tag) {
          case XbrHTMLTAG_NONE:
              if(title)
                  w->html.title = strdup(token->text);
              else if(attributes->preformatted)
                  _XbrHTMLTypePreformat(w, token, attributes);
              else
                  _XbrHTMLTypeText(w, token, attributes);
              break;
          case XbrHTMLTAG_LI:
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeListItem(w, attributes, bullet);
              break;
          case XbrHTMLTAG_IMG:
              _XbrHTMLTypeImage(w, token, attributes);
              break;
          case XbrHTMLTAG_HR:
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              _XbrHTMLTypeLine(attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_A:
              href = _XbrHTMLFormatValue(token->text, "HREF");
              if(href && w->html.visited_test && w->html.visited_test(href))
                  attributes->foreground = XbrHTML_FOREGROUND_VISITED;
              else
                  attributes->foreground = XbrHTML_FOREGROUND_ANCHOR;
              attributes->anchor = w->html.nanchors;
              if(w->html.nanchors > w->html.maxanchor) {
                  w->html.anchors = realloc(w->html.anchors,
                    (w->html.maxanchor+20)*sizeof(char *));
                  w->html.maxanchor += 20;
              }
              if(href)
                  w->html.anchors[w->html.nanchors-1] = strdup(href);
              else
                  w->html.anchors[w->html.nanchors-1] = NULL;
              w->html.nanchors++;
              break;
          case XbrHTMLTAG_END_A:
              attributes->foreground = XbrHTML_FOREGROUND_DEFAULT;
              attributes->anchor = 0;
              break;
          case XbrHTMLTAG_OL:
              indent++;
              attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              _XbrHTMLFormatLineFeed(w, attributes);
              if(bullet)
                  StackPush(items, (void *) attributes->li_type);
              else
                  StackPush(items, (void *) attributes->li_count);
              bullet = False;
              attributes->li_count = 1;
              break;
          case XbrHTMLTAG_UL:
          case XbrHTMLTAG_MENU:
          case XbrHTMLTAG_DIR:
              indent++;
              attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              _XbrHTMLFormatLineFeed(w, attributes);
              if(bullet) {
                  StackPush(items, (void *) attributes->li_type);
              } else {
                  StackPush(items, (void *) attributes->li_count);
              }
              bullet = True;
              attributes->li_type--;
              break;
          case XbrHTMLTAG_END_OL:
              indent--;
              if(indent <= 0) {
                  indent = 0;
                  attributes->left_margin = XbrHTMLFORMAT_LEFT_MARGIN;
              } else {
                  attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              }
              _XbrHTMLFormatLineFeed(w, attributes);
              item = (int) StackPop(items);
              if(item == 0) {
                  attributes->li_type = 0;
                  attributes->li_count = 1;
                  bullet = True;
              } else if(item < 0) {
                  attributes->li_type = item;
                  attributes->li_count = 1;
                  bullet = True;
              } else {
                  attributes->li_type = 0;
                  attributes->li_count = item;
                  bullet = False;
              }
              break;
          case XbrHTMLTAG_END_UL:
          case XbrHTMLTAG_END_MENU:
          case XbrHTMLTAG_END_DIR:
              indent--;
              if(indent <= 0) {
                  indent = 0;
                  attributes->left_margin = XbrHTMLFORMAT_LEFT_MARGIN;
              } else {
                  attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              }
              _XbrHTMLFormatLineFeed(w, attributes);
              item = (int) StackPop(items);
              if(item == 0) {
                  attributes->li_type = 0;
                  attributes->li_count = 1;
                  bullet = True;
              } else if(item < 0) {
                  attributes->li_type = item;
                  attributes->li_count = 1;
                  bullet = True;
              } else {
                  attributes->li_type = 0;
                  attributes->li_count = item;
                  bullet = False;
              }
              break;
          case XbrHTMLTAG_TITLE:
              title = True;
              break;
          case XbrHTMLTAG_B:
          case XbrHTMLTAG_STRONG:
          case XbrHTMLTAG_ADDRESS:
              font_style = font_style|XbrHTML_FONT_BOLD;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_CITE:
          case XbrHTMLTAG_I:
          case XbrHTMLTAG_EM:
              font_style = font_style|XbrHTML_FONT_ITALIC;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_TT:
              font_style = font_style|XbrHTML_FONT_FIXED;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_H1:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 1;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_H2:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 2;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_H3:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 3;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_H4:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 4;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_H5:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 5;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_H6:
              font_style = font_style|XbrHTML_FONT_BOLD;
              font_size = 6;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_END_ADDRESS:
          case XbrHTMLTAG_END_B:
          case XbrHTMLTAG_END_STRONG:
              font_style = font_style&~XbrHTML_FONT_BOLD;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_END_EM:
          case XbrHTMLTAG_END_CITE:
          case XbrHTMLTAG_END_I:
              font_style = font_style&~XbrHTML_FONT_ITALIC;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_END_TT:
              font_style = font_style&~XbrHTML_FONT_FIXED;
              attributes->font = ((font_size-1)*8)+font_style;
              break;
          case XbrHTMLTAG_END_TITLE:
              title = False;
              break;
          case XbrHTMLTAG_END_H1:
          case XbrHTMLTAG_END_H2:
          case XbrHTMLTAG_END_H3:
          case XbrHTMLTAG_END_H4:
          case XbrHTMLTAG_END_H5:
          case XbrHTMLTAG_END_H6:
              font_style = font_style&~XbrHTML_FONT_BOLD;
              font_size = _XbrHTMLDefaultFontSize;
              attributes->font = ((font_size-1)*8)+font_style;
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 1);
              break;
          case XbrHTMLTAG_P:
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, 2);
              _XbrHTMLTypeNewLine(attributes, 2);
              break;
          case XbrHTMLTAG_END_PRE:
              font_style = font_style&~XbrHTML_FONT_FIXED;
              attributes->font = ((font_size-1)*8)+font_style;
              attributes->preformatted = False;
              printf("END PREFORMATTED!\n");
              break;
          case XbrHTMLTAG_PRE:
              font_style = font_style|XbrHTML_FONT_FIXED;
              attributes->font = ((font_size-1)*8)+font_style;
              attributes->preformatted = True;
              break;
          case XbrHTMLTAG_BR:
              _XbrHTMLFormatLineFeed(w, attributes);
              _XbrHTMLTypeNewLine(attributes, -1);
              break;
          case XbrHTMLTAG_DL:
              indent++;
              _XbrHTMLFormatLineFeed(w, attributes);
              break;
          case XbrHTMLTAG_END_DL:
              indent--;
              if(indent <= 0) {
                  indent = 0;
                  attributes->left_margin = XbrHTMLFORMAT_LEFT_MARGIN;
              } else {
                  attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              }
              break;
          case XbrHTMLTAG_DT:
              indent--;
              if(indent <= 0) {
                  indent = 0;
                  attributes->left_margin = XbrHTMLFORMAT_LEFT_MARGIN;
              } else {
                  attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              }
              _XbrHTMLFormatLineFeed(w, attributes);
              break;
          case XbrHTMLTAG_DD:
              indent++;
              attributes->left_margin = indent*XbrHTMLFORMAT_INDENTATION;
              _XbrHTMLFormatLineFeed(w, attributes);
              break;
          default:
              /*fprintf(stderr, "Unrecognised tag: %s\n", token->text);*/
              break;
        }
    }

    /* Format the very last line */
    _XbrHTMLFormatLineFeed(w, attributes);

    /* Free up memory allocated for stack */
    StackDestroy(items);

    /* Free up tokens as we don't need them anymore */
    XbrHTMLTokenFree(w->html.tokens);
    w->html.tokens = NULL;

    /* Set the height of the documet based on the cursor position */
    w->html.doc_height = attributes->y;

    return(attributes->first);
}

/*-----------------------------------------------------------------------------
  XbrHTMLElementsFree()
  Free up all the memory allocated for this list of elements. This also
  means destroying pixmaps and freeing up colours.
  ----------------------------------------------------------------------------*/
void XbrHTMLElementsFree(
 XbrHTMLWidget w,		/* Widget data - needed for display */
 XbrHTMLElement *element	/* First element in list to freee */ )
{
    XbrHTMLElement *tmp, *e;

    e = element;
    while(e) {
        tmp = e->next;
        free(e->text);
        if(e->pixmap) {
            XFreePixmap(XbrDisplay((Widget)w), e->pixmap);
            XbrColoursFree(XbrDisplay((Widget)w), XbrColourmap((Widget)w),
              e->colours, e->ncolours);
            free(e->colours);
        }
        free(e);
        e = tmp;
    }
}

/*-----------------------------------------------------------------------------
  XbrHTMLReformat()
  A text reformat becomes necessary when the window is resized horizontally.
  A vertical resize it easy because things like word wrap don't come into
  the equation. We reformat by checking if an element will still fit onto
  the line, if not it is fragmented.
  ----------------------------------------------------------------------------*/
void XbrHTMLReformat(
 XbrHTMLWidget w	/* Widget data */ )
{
    XbrHTMLElement *element;
    _XbrHTMLFormatData fd, *attributes;
    int difference;

    attributes = &fd;

    _XbrHTMLFormatInit(w, attributes);

    for(element = w->html.elements; element; element = element->next) {
        attributes->left_margin = element->left_margin;
        switch(element->type) {
            case XbrHTML_TYPE_NEWLINE:
                _XbrHTMLFormatLineFeed(w, attributes);
                attributes->y += XbrHTMLFORMAT_LINE_HEIGHT;
                attributes->baseline = attributes->y;
                break;
            case XbrHTML_TYPE_TEXT:
                if(element->preformatted) {
                    /* Move it on up */
                    difference = element->y - element->y0;
                    element->y0 = attributes->y;
                    element->y = attributes->y+difference;
                    attributes->y += element->height;
                    attributes->baseline = attributes->y;
                } else {
                    if((attributes->x + element->width) > attributes->width) {
                        element = _XbrHTMLFormatFragment(w, attributes,
                          element);
                    } else {
                        element->y = XbrHTMLFormat_Y_UNDEFINED;
                        element->y0 = XbrHTMLFormat_Y_UNDEFINED;
                        element->x = attributes->x;
                        element->x0 = attributes->x;
                        _XbrHTMLFormatAdjustBaseline(attributes, element,
                          attributes->y + w->html.fonts[element->font]->ascent,
                          (w->html.fonts[element->font]->descent*2),
                          w->html.fonts[element->font]->ascent +
                          w->html.fonts[element->font]->descent);
                        attributes->x += element->width;
                    }
                }
                break;
            case XbrHTML_TYPE_LINE:
                element->y = attributes->y;
                element->y0 = attributes->y;
                element->x = attributes->x;
                element->x0 = attributes->x;
                element->width = attributes->width - attributes->left_margin
                  - attributes->left_margin;
                attributes->y += 2;
                break;
            case XbrHTML_TYPE_LIST_ITEM:
                _XbrHTMLFormatLineFeed(w, attributes);
                element->x = XbrHTMLFormat_X_UNDEFINED;
                element->x0 = XbrHTMLFormat_X_UNDEFINED;
                element->y = XbrHTMLFormat_Y_UNDEFINED;
                element->y0 = XbrHTMLFormat_Y_UNDEFINED;

                element->width = XbrHTMLFormat_WIDTH_UNDEFINED;
                element->height = XbrHTMLFormat_HEIGHT_UNDEFINED;

                /* Shouldn't we adjust a baseline here? */
                if(attributes->start == NULL)
                    attributes->start = element;
                break;
            case XbrHTML_TYPE_IMAGE:
                /* What about preformatted images? */
                if(attributes->x+element->width > attributes->width)
                    _XbrHTMLFormatLineFeed(w, attributes);
                _XbrHTMLFormatAdjustBaseline(attributes, element,
                  attributes->y+element->height, 0, 0);
                element->x = attributes->x;
                element->x0 = attributes->x;
                element->y = XbrHTMLFormat_Y_UNDEFINED;
                element->y0 = XbrHTMLFormat_Y_UNDEFINED;
                attributes->x += element->width;
                break;
            case XbrHTML_TYPE_NUMBER:
                _XbrHTMLFormatLineFeed(w, attributes);
                element->x = attributes->x-element->width;
                element->x0 = attributes->x-element->width;
                element->y = XbrHTMLFormat_Y_UNDEFINED;
                element->y0 = XbrHTMLFormat_Y_UNDEFINED;
                _XbrHTMLFormatAdjustBaseline(attributes, element,
                  attributes->y + w->html.fonts[element->font]->ascent,
                  (w->html.fonts[element->font]->descent*2),
                  w->html.fonts[element->font]->ascent +
                  w->html.fonts[element->font]->descent);
                break;
        }
    }

    /* Format the very last line */
    _XbrHTMLFormatLineFeed(w, attributes);

    w->html.doc_height = attributes->y;
}

/*-----------------------------------------------------------------------------
  _XbrHTMLFormatFragment()
  Fragment text because it does not fit onto the current line.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLFormatFragment(
 XbrHTMLWidget w,			/* Widget data */
 _XbrHTMLFormatData *attributes,	/* Current context */
 XbrHTMLElement *element		/* Element to fragment */ )
{
    XbrHTMLElement *fragment;
    XCharStruct overall;
    char *start = element->text, *end = start, *ptr = NULL;
    int direction, ascent, descent, state = XbrHTMLFormat_NO_STRING_BUFFERED;
    int width, length = 0;

    /* Let's try an assertion because the parser should have stripped off
       leading white space.
    */
    assert(*end != ' '); /* OPTIMIZE: True/False */
    /* If I'm wrong then we need this line
    while(isspace(*end)) end++;
    */
    while(*end) {

        /* Jump to the end of the word */
        while(!isspace(*end) && *end) end++;
        /* Jump to the beginning of the next word */
        while(isspace(*end)) end++; /* OPTIMIZE: Only one space? */

        /* Get dimensions of string */
        XTextExtents(w->html.fonts[element->font], start, end-start,
          &direction, &ascent, &descent, &overall);

        /* Is it too long? */
        if(attributes->x+overall.width > attributes->width) {
            /* In that case we add the last length calculated to the list.
               If we have not previously calculated a width then we throw in
               a new line and start again. If the string still won't fit on the
               line then we have to make the page wider!
            */
            if(state == XbrHTMLFormat_NO_STRING_BUFFERED) {
                /* Set x to left margin and stick in a new line. */
                _XbrHTMLFormatLineFeed(w, attributes);
                state = XbrHTMLFormat_START_OF_LINE;
                end = start;
            } else {
                /* If we cannot fit the string on the line we add it to the list
                   anyway at this point or else we end up in an infinite loop */
                if(state == XbrHTMLFormat_START_OF_LINE) {
                    length = end-start;
                    width = overall.width;
                }

                /* If we have not yet split the text then just change the
                   current element.
                */
                if(start == element->text) {
                    element->x = attributes->x;
                    element->x0 = attributes->x;
                    element->y = XbrHTMLFormat_Y_UNDEFINED;
                    element->y0 = XbrHTMLFormat_Y_UNDEFINED;
                    element->length = length;
                    element->width = width;

                    /* Save this pointer so we can terminate the string later */
                    ptr = start + length;

                    /* If necessary we adjust the baseline and bottom pointer
                       based on the size of this font. Then start a new line. 
                    */
                    _XbrHTMLFormatAdjustBaseline(attributes, element,
                      attributes->y+ascent, descent+descent, ascent+descent);

                    fragment = element;
                } else {
                    /* Create a new element and add it to the linked list. */
                    fragment = _XbrHTMLFormatNewElement(start, length, width, 
                      ascent+descent, attributes, element);

                    /* If necessary we adjust the baseline and bottom pointer
                       based on the size of this font. Then start a new line. 
                    */
                    _XbrHTMLFormatAdjustBaseline(attributes, fragment,
                      attributes->y+ascent, descent+descent, ascent+descent);

                    /* Insert it in the list */
                    fragment->next = element->next;
                    element->next = fragment;
                    element = fragment;
                } 

                /* Start a new line */
                _XbrHTMLFormatLineFeed(w, attributes);

                /* Set up the string pointers */
                end = start = start + length;

                /* Let's try an assertion because the parser should have
                   stripped off leading white space.
                */
                assert(*start != ' '); /* OPTIMIZE: True/False */
                /* If I'm wrong then we need this line
                while(isspace(*start)) start++;
                */

                state = XbrHTMLFormat_NO_STRING_BUFFERED;
            }
        } else {
            length = end - start;
            width = overall.width;
            state = XbrHTMLFormat_STRING_BUFFERED;
        }
    }

    /* Check to see if we passed through without changing anything. If so
       adjust some x,y coordinates.
    */
    if(start == element->text) {
        element->x = attributes->x;
        element->x0 = attributes->x;
        element->y = XbrHTMLFormat_Y_UNDEFINED;
        element->y0 = XbrHTMLFormat_Y_UNDEFINED;
        _XbrHTMLFormatAdjustBaseline(attributes, element, attributes->y+ascent,
          descent+descent, ascent+descent);
        attributes->x += element->width;
        fragment = element;
    } else if(state == XbrHTMLFormat_STRING_BUFFERED) {
        /* We check to see if there is a string still buffered. If so we 
           it means we have to fragment. */
        fragment = _XbrHTMLFormatNewElement(start, length, width, 
          ascent+descent, attributes, element);

        _XbrHTMLFormatAdjustBaseline(attributes, fragment, attributes->y+ascent,
          descent+descent, ascent+descent);

        fragment->next = element->next;
        element->next = fragment;

        attributes->x += width;
    }

    /* If we have split an element, terminate the string */
    if(ptr)
        *ptr = 0;

    return(fragment);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLTypePreformat()
  Create a new text element. We have a different routine for parsing
  preformatted text because elements are split on new lines rather than when 
  a word wrap is necessary. Also it is necessary to handle tabs and other
  white space characters.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypePreformat(
 XbrHTMLWidget w,		/* Pointer to widget and data */
 XbrHTMLToken *token,		/* Text token to parse */
 _XbrHTMLFormatData *attributes	/* Current formatting details */ )
{
    XbrHTMLElement *element;
    XCharStruct overall;
    Boolean single_new_line = False;
    char *start = token->text, *end = start;
    int direction, ascent, descent, state = XbrHTMLFormat_STRING_BUFFERED;
    int length = 0, tabsize, x;

    /* In case we need to add a tab, we determine the size of a tab in the
       current font - which is set to 8 spaces.
       OPTIMIZE: We only need to do this if tabs ARE present and we only
       really need to do it once for each font anyway.
    */
    XTextExtents(w->html.fonts[attributes->font], " ", 1, &direction, &ascent,
      &descent, &overall);
    tabsize = 8*overall.width;

    while(*end) {
        /* Find the first tab or new line */
        while(*end && *end != '\n' && *end != '\t') end++;

        /* Get dimensions of string */
        XTextExtents(w->html.fonts[attributes->font], start, end-start,
          &direction, &ascent, &descent, &overall);

        /* Deal with tab and new line. */
        if(*end) {
            /* If this is not just a new line on its own add the text */
            if(start != end) {
                /* Length of string does not include new line */
                length = end-start;

                /* Create a new element and add it to the linked list. */
                element = _XbrHTMLFormatNewElement(start, length, overall.width,
                  ascent+descent, attributes, NULL);
                _XbrHTMLFormatAddToList(attributes,element);

                /* If necessary we adjust the baseline and bottom pointer
                   based on the size of this font.
                */
                _XbrHTMLFormatAdjustBaseline(attributes, element,
                  attributes->y+ascent, descent+descent, ascent+descent);

               /* Increment x pointer. */
               attributes->x += overall.width;

               single_new_line = False;
            } else {
               single_new_line = True;
            }
            if(*end == '\n') {
                if(single_new_line)
                    _XbrHTMLFormatAdjustBaseline(attributes, element,
                      0, XbrHTMLFORMAT_LINE_HEIGHT, XbrHTMLFORMAT_BULLET_SIZE);
                _XbrHTMLFormatLineFeed(w, attributes);
/*
                if(single_new_line) {
printf("Single new line\n");
                    _XbrHTMLTypeNewLine(attributes, -1);
}
*/
            } else if(*end == '\t') {
                x = num_roundup(attributes->x, tabsize);
                if(attributes->x == x)
                    attributes->x += tabsize;
                else
                    attributes->x = x;
            }
            start = end = end+1;
        }
    }

    /* Check to see if there is a string left to parse */
    if(start != end) {
        /* Set length */
        length = end-start;

        /* Get dimensions of string */
        XTextExtents(w->html.fonts[attributes->font], start, end-start,
          &direction, &ascent, &descent, &overall);

        /* Create a new element and add it to the linked list. */
        element = _XbrHTMLFormatNewElement(start, length, overall.width, 
            ascent+descent, attributes, NULL);
        _XbrHTMLFormatAddToList(attributes,element);
        _XbrHTMLFormatAdjustBaseline(attributes, element, attributes->y+ascent,
          descent+descent, ascent+descent);
        attributes->x += overall.width;
    }

    return(element);
}
/*-----------------------------------------------------------------------------
  _XbrHTMLTypeText()
  Create a new text element. We must parse the line of text a word at a time
  and check if the next word will require wrapping onto the next line. If so
  it becomes a new element.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeText(
 XbrHTMLWidget w,		/* Pointer to widget and data */
 XbrHTMLToken *token,		/* Text token to parse */
 _XbrHTMLFormatData *attributes	/* Current formatting details */ )
{
    XbrHTMLElement *element;
    XCharStruct overall;
    char *start = token->text, *end = start;
    int direction, ascent, descent, state = XbrHTMLFormat_NO_STRING_BUFFERED;
    int width, length = 0;

    /* Let's try an assertion because the parser should have stripped off
       leading white space.
    */
    assert(*end != ' '); /* OPTIMIZE: True/False */
    /* If I'm wrong then we need this line
    while(isspace(*end)) end++;
    */
    while(*end) {

        /* Jump to the end of the word */
        while(!isspace(*end) && *end) end++;
        /* Jump to the beginning of the next word */
        while(isspace(*end)) end++; /* OPTIMIZE: Only one space? */

        /* Get dimensions of string */
        XTextExtents(w->html.fonts[attributes->font], start, end-start,
          &direction, &ascent, &descent, &overall);

        /* Is it too long? */
        if(attributes->x+overall.width > attributes->width) {
            /* In that case we add the last length calculated to the list.
               If we have not previously calculated a width then we throw in
               a new line and start again. If the string still won't fit on the
               line then we have to make the page wider!
            */
            if(state == XbrHTMLFormat_NO_STRING_BUFFERED) {
                /* Set x to left margin and stick in a new line. */
                _XbrHTMLFormatLineFeed(w, attributes);
                state = XbrHTMLFormat_START_OF_LINE;
                end = start;
            } else {
                /* If we cannot fit the string on the line we add it to the list
                   anyway at this point or else we end up in an infinite loop */
                if(state == XbrHTMLFormat_START_OF_LINE) {
                    length = end-start;
                    width = overall.width;
                }

                /* Create a new element and add it to the linked list. */
                element = _XbrHTMLFormatNewElement(start, length, width, 
                  ascent+descent, attributes, NULL);
                _XbrHTMLFormatAddToList(attributes,element);

                /* If necessary we adjust the baseline and bottom pointer
                   based on the size of this font. Then start a new line. 
                */
                _XbrHTMLFormatAdjustBaseline(attributes, element,
                  attributes->y+ascent, descent+descent, ascent+descent);
                _XbrHTMLFormatLineFeed(w, attributes);

                /* Set up the string pointers */
                end = start = start + length;

                /* Let's try an assertion because the parser should have
                   stripped off leading white space.
                */
                assert(*start != ' '); /* OPTIMIZE: True/False */
                /* If I'm wrong then we need this line
                while(isspace(*start)) start++;
                */

                state = XbrHTMLFormat_NO_STRING_BUFFERED;
            }
        } else {
            length = end - start;
            width = overall.width;
            state = XbrHTMLFormat_STRING_BUFFERED;
        }
    }

    /* Now we've finished we check to see if there is a string still
       buffered. If so we add it to the line without appending a new line. */
    if(state == XbrHTMLFormat_STRING_BUFFERED) {
        /* Create a new element and add it to the linked list. */
        element = _XbrHTMLFormatNewElement(start, length, width, 
            ascent+descent, attributes, NULL);
        _XbrHTMLFormatAddToList(attributes,element);
        _XbrHTMLFormatAdjustBaseline(attributes, element, attributes->y+ascent,
          descent+descent, ascent+descent);
        attributes->x += width;
    }

    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLTypeListItem()
  Create a new list item element - this is a bullet point which is specified
  by the <LI> tag.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeListItem(
 XbrHTMLWidget w,		/* Widget data */
 _XbrHTMLFormatData *attributes,/* Some attributes, such as x,y location */
 Boolean bullet			/* Bullet point or numbered item? */ )
{
    XbrHTMLElement *element;
    char buffer[20];
    int direction, ascent, descent;
    XCharStruct overall;

    element = (XbrHTMLElement *)malloc(sizeof(XbrHTMLElement));
    if(element) {
        if(bullet) {
            element->type = XbrHTML_TYPE_LIST_ITEM;
            element->subtype = (attributes->li_type%3)-1;
            element->font = attributes->font;
            element->foreground = attributes->foreground;

            element->x = XbrHTMLFormat_X_UNDEFINED;
            element->y = XbrHTMLFormat_Y_UNDEFINED;
            element->x0 = XbrHTMLFormat_X_UNDEFINED;
            element->y0 = XbrHTMLFormat_Y_UNDEFINED;

            element->width = XbrHTMLFormat_WIDTH_UNDEFINED;
            element->height = XbrHTMLFormat_HEIGHT_UNDEFINED;

            element->left_margin = attributes->left_margin;

            element->preformatted = attributes->preformatted;

            element->anchor = attributes->anchor;

            element->text = NULL;
            element->length = 0;

            element->pixmap = 0;
            element->colours = NULL;
            element->ncolours = 0;

            element->next = NULL;

            if(attributes->start == NULL)
                attributes->start = element;
        } else {
            element->type = XbrHTML_TYPE_NUMBER;
            element->subtype = 0;
            element->font = attributes->font;
            element->foreground = attributes->foreground;

            sprintf(buffer, "%d. ", attributes->li_count++);
            XTextExtents(w->html.fonts[attributes->font],buffer,strlen(buffer),
              &direction, &ascent, &descent, &overall);

            element->x = attributes->x-overall.width;
            element->y = XbrHTMLFormat_Y_UNDEFINED;
            element->x0 = attributes->x-overall.width;
            element->y0 = XbrHTMLFormat_Y_UNDEFINED;

            element->width = overall.width;
            element->height = ascent+descent;

            element->left_margin = attributes->left_margin;

            element->preformatted = attributes->preformatted;

            element->anchor = attributes->anchor;

            element->text = strdup(buffer);
            element->length = strlen(buffer);

            element->pixmap = 0;
            element->colours = NULL;
            element->ncolours = 0;

            element->next = NULL;

            _XbrHTMLFormatAdjustBaseline(attributes, element,
              attributes->y+ascent, descent+descent, ascent+descent);
        }
        _XbrHTMLFormatAddToList(attributes,element);
    }
    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLTypeLine()
  Create a new line element - as specified by the <HR> tag. This is just a
  horizontal rule and is drawn as a line.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeLine(
 _XbrHTMLFormatData *attributes /* Some attributes, such as x,y location */ )
{
    XbrHTMLElement *element;

    element = (XbrHTMLElement *)malloc(sizeof(XbrHTMLElement));
    if(element) {
        element->type = XbrHTML_TYPE_LINE;
        element->subtype = 0;
        element->font = attributes->font;
        element->foreground = attributes->foreground;

        element->x = attributes->x;
        element->y = attributes->y;
        element->x0 = attributes->x;
        element->y0 = attributes->y;
        attributes->y++;

        element->width = attributes->width-attributes->left_margin-attributes->left_margin;
        element->height = 2;
        element->left_margin = attributes->left_margin;

        element->preformatted = attributes->preformatted;

        element->anchor = attributes->anchor;

        element->text = NULL;
        element->length = 0;

        element->pixmap = 0;
        element->colours = NULL;
        element->ncolours = 0;

        element->next = NULL;

        _XbrHTMLFormatAddToList(attributes,element);
    }
    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLTypeImage()
  Create a new list image element - this is heavy stuff. We must find the
  reference to the inlined image and retrieve it and then work out its
  width and height. Also a pixmap is created by the resolver.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeImage(
 XbrHTMLWidget w,		/* Widget data */
 XbrHTMLToken *token,		/* Token containing tag data */
 _XbrHTMLFormatData *attributes /* Some attributes, such as x,y location */ )
{
    XbrHTMLElement *element;
    Pixmap pixmap = 0;
    char *src = NULL;
    unsigned long *colours = NULL;
    int width = XbrHTMLFORMAT_IMAGE_SIZE, height = XbrHTMLFORMAT_IMAGE_SIZE;
    int ncolours = 0;
    src = _XbrHTMLFormatValue(token->text, "SRC");

    if(w->html.image_resolver && src)
        pixmap = w->html.image_resolver((Widget)w, src, &width, &height,
          &colours, &ncolours);

    element = (XbrHTMLElement *)malloc(sizeof(XbrHTMLElement));
    if(element) {
        if(attributes->x+width > attributes->width)
            _XbrHTMLFormatLineFeed(w, attributes);

        element->type = XbrHTML_TYPE_IMAGE;
        element->subtype = 0;
        element->font = attributes->font;
        element->foreground = attributes->foreground;

        element->x = attributes->x;
        element->y = XbrHTMLFormat_Y_UNDEFINED;
        element->x0 = attributes->x;
        element->y0 = XbrHTMLFormat_Y_UNDEFINED;

        element->width = width;
        element->height = height;

        element->left_margin = attributes->left_margin;

        element->preformatted = attributes->preformatted;

        element->anchor = attributes->anchor;

        element->text = NULL;
        element->length = 0;

        element->pixmap = pixmap;
        element->colours = colours;
        element->ncolours = ncolours;

        element->next = NULL;

        attributes->x += element->width;
        _XbrHTMLFormatAdjustBaseline(attributes, element,
          attributes->y+element->height, 0, 0);
        _XbrHTMLFormatAddToList(attributes, element);
    }
    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLTypeNewLine()
  Some tags force new lines to occur. So that we can reformat later when we
  do not have access to tags, we add this to the element list. When this is
  called we assume that a line feed has been inserted.
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLTypeNewLine(
 _XbrHTMLFormatData *attributes,/* Some attributes, such as x,y location */
 int max			/* Maximum number of new lines allowed */ )
{
    XbrHTMLElement *element;

    /* Under some circumstances we are only allowed a certain number of
       new lines. So we return if we have too many.
    */
    if(max != -1 && attributes->newlines+1>max)
        return(NULL);

    element = (XbrHTMLElement *)malloc(sizeof(XbrHTMLElement));
    if(element) {
        attributes->y += XbrHTMLFORMAT_LINE_HEIGHT;
        attributes->baseline = attributes->y; /* OPTIMIZE: Is this necessary? */

        /* Most of these settings are irrelevant. */
        element->type = XbrHTML_TYPE_NEWLINE;
        element->subtype = 0;
        element->font = attributes->font;
        element->foreground = attributes->foreground;

        element->x = attributes->x;
        element->y = attributes->y;
        element->x0 = attributes->x;
        element->y0 = attributes->y;

        element->width = attributes->width;
        element->height = XbrHTMLFORMAT_LINE_HEIGHT;

        element->left_margin = attributes->left_margin;

        element->preformatted = attributes->preformatted;

        element->anchor = attributes->anchor;

        element->text = NULL;
        element->length = 0;

        element->pixmap = 0;
        element->colours = NULL;
        element->ncolours = 0;

        element->next = NULL;

        _XbrHTMLFormatAddToList(attributes,element);
    }
    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLFormatNewElement()
  Create a new XbrHTMLElement structure and fill it with the given string
  and attributes. Note that we leave the y coordinates undefined because
  another portion of text on the same line may be in a different font which
  means this font may have to be moved. We only add a suggested y coordinate
  at this point and it is determined when we have a full line. Geddit?
  ----------------------------------------------------------------------------*/
static XbrHTMLElement *_XbrHTMLFormatNewElement(
 char *start,			/* Pointer to beginning of text string */
 int length,			/* Length of string 'start' points to */
 int width,			/* Length of string in pixels */
 int height,			/* Height of string/font in pixels */
 _XbrHTMLFormatData *attributes,/* Some attributes, such as x,y location */
 XbrHTMLElement *fragment 	/* Copy from this element instead of atts */ )
{
    XbrHTMLElement *element;

    element = (XbrHTMLElement *)malloc(sizeof(XbrHTMLElement));
    if(element) {
        element->text = (char *) malloc(length+1);
        if(element->text == NULL) {
            free(element);
            element = NULL;
        } else {
            element->type = XbrHTML_TYPE_TEXT;
            element->subtype = 0;
            strncpy(element->text, start, length);
            element->text[length] = 0;
            element->length = length;
            element->pixmap = 0;
            element->colours = NULL;
            element->ncolours = 0;
            element->width = width; 
            element->height = height;
            element->next = NULL;
            element->y = XbrHTMLFormat_Y_UNDEFINED;
            element->y0 = XbrHTMLFormat_Y_UNDEFINED;
            element->x = attributes->x;
            element->x0 = attributes->x;
            if(fragment) {
                element->font = fragment->font;
                element->foreground = fragment->foreground;
                element->left_margin = fragment->left_margin;
                element->preformatted = fragment->preformatted;
                element->anchor = fragment->anchor;
            } else {
                element->font = attributes->font;
                element->foreground = attributes->foreground;
                element->left_margin = attributes->left_margin;
                element->preformatted = attributes->preformatted;
                element->anchor = attributes->anchor;
            }
        }
    }
    return(element);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLFormatLineFeed()
  This doesn't actually insert a line feed or new line into the list but
  instead traces back through the last line and sets the baseline and
  line height correctly for each font on that line.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLFormatLineFeed(
 XbrHTMLWidget w,		/* Widget data */
 _XbrHTMLFormatData *attributes	/* Pointer to active format data */ )
{
    XbrHTMLElement *element;
    int line_width = 0;

    for(element = attributes->start; element; element = element->next) {
        switch(element->type) {
            case XbrHTML_TYPE_TEXT:
            case XbrHTML_TYPE_NUMBER:
                element->y = attributes->baseline;
                element->y0 = attributes->baseline -
                  w->html.fonts[element->font]->ascent;
                break;
            case XbrHTML_TYPE_LIST_ITEM:
                element->y = attributes->baseline - attributes->bullet_size -
                  XbrHTML_BULLET_BOTTOM_OFFSET;
                element->y0 = attributes->baseline - attributes->bullet_size -
                   XbrHTML_BULLET_BOTTOM_OFFSET;
                element->x = element->left_margin - attributes->bullet_size -
                  XbrHTML_BULLET_RIGHT_OFFSET;
                element->x0 = element->left_margin - attributes->bullet_size
                  - XbrHTML_BULLET_RIGHT_OFFSET;
                element->width = element->height = attributes->bullet_size;

                _XbrHTMLFormatAdjustBaseline(attributes, element,
                  attributes->y+element->height, 0, element->height);

                break;
            case XbrHTML_TYPE_IMAGE:
                element->y = element->y0 = attributes->y;
                break;
            default:
                break;
        }
        line_width = element->x + element->width;
    }

    /* Is this line longer than the current longest line? If it is then
       we set the max line width
    */
    if(line_width > w->html.doc_width)
        w->html.doc_width = line_width;

    attributes->x = attributes->left_margin;
    attributes->y = attributes->baseline+attributes->bottom;
    attributes->start = NULL;
    attributes->baseline = attributes->y;
    attributes->bullet_size = XbrHTMLFORMAT_BULLET_SIZE;
    attributes->bottom = 0;
}

/*-----------------------------------------------------------------------------
  _XbrHTMLFormatValue()
  Some tags have attributes which are specified as name value pairs. For
  example <A HREF=doc.html> where HREF is the name (case-insensitive) and
  doc.html is the value (which may be in quotes). We parse the tag looking for
  a name and if it is not found just return NULL.
  ----------------------------------------------------------------------------*/
static char *_XbrHTMLFormatValue(
 char *text,	/* Full tag to parse */
 char *name	/* Name we are searching for */ )
{
    static char buffer[256];	/* Use a static buffer for now */
    char *ptr, *bufptr = NULL;
    int length = 0;


    if(text && (ptr = str_casestr(text, name))) {
        ptr += strlen(name);
        while(*ptr && *ptr != '=') ptr++;
        if(*ptr) ptr++;
        while(isspace(*ptr)) ptr++;
        if(*ptr == '"') {
            ptr++;
            while(*ptr && *ptr != '"' && *ptr != '>')
                buffer[length++] = *ptr++;
            buffer[length] = 0;
            bufptr = buffer;
        } else if(*ptr) {
            while(*ptr && !isspace(*ptr) && *ptr != '>')
                buffer[length++] = *ptr++;
            buffer[length] = 0;
            bufptr = buffer;
        }
    }

    return(bufptr);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLFormatInit()
  So that we have the intialisation for XbrHTMLFormatData structures in one
  place we chuck it all into this function here.
  ----------------------------------------------------------------------------*/
static void _XbrHTMLFormatInit(
 XbrHTMLWidget w,		/* Widget data */
 _XbrHTMLFormatData *attributes	/* Structure to initialse */ )
{
    attributes->x = XbrHTMLFORMAT_LEFT_MARGIN;
    attributes->y = 10;
    attributes->width = w->html.da_width;
    attributes->left_margin = XbrHTMLFORMAT_LEFT_MARGIN;
    attributes->newlines = 0;
    attributes->bullet_size = XbrHTMLFORMAT_BULLET_SIZE;
    attributes->font = ((_XbrHTMLDefaultFontSize-1)*8)+XbrHTML_FONT_NORMAL;
    attributes->foreground = XbrHTML_FOREGROUND_DEFAULT;
    attributes->li_type = 0;
    attributes->li_count = 1;
    attributes->baseline = 10;
    attributes->bottom = 0;
    attributes->anchor = 0;
    attributes->preformatted = False;
    attributes->start = NULL;
    attributes->first = NULL;
    attributes->current = NULL;
}
