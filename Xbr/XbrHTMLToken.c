/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLToken.c
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Code for lexical analysis of HTML documents.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>

#include "XbrHTML.h"
#include "XbrHTMLP.h"
#include "XbrHTMLEscapes.h"
#include "XbrHTMLTags.h"

/*****************************************************************************/
/******************************** DEFINES ************************************/
/*****************************************************************************/
#define _XbrHTMLLowercase(c)             ((c>64&&c<91)?c+32:c)


/*****************************************************************************/
/*************************** FUNCTIONS DECLARATIONS **************************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
  Internal functions.
 ----------------------------------------------------------------------------*/
static XbrHTMLToken *_XbrHTMLNewTagToken(char *, char *);
static unsigned int _XbrHTMLGetTag(char *);
static XbrHTMLToken *_XbrHTMLNewTextToken(char *, char *, Boolean);
static char _XbrHTMLEscape(char **);


/*----------------------------------------------------------------------------
  External functions.
 ----------------------------------------------------------------------------*/
XbrHTMLToken *XbrHTMLTokenize(XbrHTMLWidget, char *);
void XbrHTMLTokenFree(XbrHTMLToken *);




/*****************************************************************************/
/***************************** EXTERNAL FUNCTIONS ****************************/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  XbrHTMLTokenize()
  Put the data into tokens which can be parsed much more easily by the parser.
  This is the lexical analysis stage which splits the HTML data into tokens
  and text, stripping off whitespace where necessary. If it comes across
  a preformat token then the spaces and new lines are not stripped.

  XbrHTMLWidget w        Widget to use
  char *data             Data to set
 -----------------------------------------------------------------------------*/
XbrHTMLToken *XbrHTMLTokenize(XbrHTMLWidget w, char *data)
{
    Boolean preformatted = False;
    XbrHTMLToken *first = NULL, *last = NULL, *current = NULL, *token;
    char *start = data, *end, *ptr;

        
    while(*start) {
        if(preformatted == False && isspace(*start)) {
            if(last&&last->text&&last->text[strlen(last->text)-1] != ' ')
                last->text[strlen(last->text)] = ' ';
            while(isspace(*start)) start++;
        }

        if(*start == '<') {
            for(end = start; *end && *end != '>'; end++);
            if(*end) {
                token = _XbrHTMLNewTagToken(start, end);
                if(token->tag == XbrHTMLTAG_PRE)
                    preformatted = True;
                else if(token->tag == XbrHTMLTAG_END_PRE)
                    preformatted = False;
                start = end+1;
                XbrHTMLAddToList(first,current,token);
            } else {
                fprintf(stderr, "Document ends with an open tag!\n");
                start = end;
            }
        } else if(*start) {
            for(end = start; *end && *end != '<'; end++);
            if(*end) {
                token = _XbrHTMLNewTextToken(start, end, preformatted);
                XbrHTMLAddToList(first,current,token);
                last = token;
            }
            start = end;
        }
    }
    return(first);
}

/*-----------------------------------------------------------------------------
  XbrHTMLTokenFree()
  Free up memory used by tokens when finished. For now.
  ----------------------------------------------------------------------------*/

void XbrHTMLTokenFree(
 XbrHTMLToken *token	/* First token in list to free */ )
{
    XbrHTMLToken *tmp, *t;

    t = token;
    while(t) {
        tmp = t->next;
        free(t->text);
        free(t);
        t = tmp;
    }
}

/*****************************************************************************/
/***************************** INTERNAL FUNCTIONS ****************************/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
  _XbrHTMLNewTagToken()
  Allocate memory for a new token structure and set the token text and the
  token type based on the string, starting at 'start' and ending at 'end'.

  char *start	Pointer to start of token
  char *end	Pointer to end of toke
  ----------------------------------------------------------------------------*/
static XbrHTMLToken *_XbrHTMLNewTagToken(char *start, char *end)
{
    XbrHTMLToken *token = NULL;
    char *ptr, *tptr;

    token = (XbrHTMLToken *) malloc(sizeof(XbrHTMLToken));
    if(token) {
        token->text = (char *) malloc(end-start+1);
        if(token->text == NULL) {
            free(token);
            token = NULL;
        } else {
            for(ptr = start+1, tptr = token->text; ptr != end;)
                *tptr++ = *ptr++;
            *tptr = 0;
            token->tag = _XbrHTMLGetTag(token->text);
            token->next = NULL;
        }
    }
    return(token);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLGetTag()
  Each HTML tag is given an ID based on a hashing function. So far this hashing
  function has produced unique results for each tag. So far.

  char *tag	Tag to decode
  ----------------------------------------------------------------------------*/
static unsigned int _XbrHTMLGetTag(char *tag)
{
    unsigned int i = 0;
    char *ptr = tag;

    assert(*ptr != '<');
    while(*ptr && *ptr != ' ') {
        i = (i<<1)+_XbrHTMLLowercase(*ptr);
        ptr++;
    }
    return(i);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLNewTextToken()
  Allocate memory for a new token structure and set the token text. Decode
  any ampersand encoded structures.

  char *start		Pointer to start of token
  char *end		Pointer to end of toke
  Boolean preformatted	If this is true then we keep hold of white space
  ----------------------------------------------------------------------------*/
static XbrHTMLToken *_XbrHTMLNewTextToken(char *start, char *end,
 Boolean preformatted)
{
    XbrHTMLToken *token = NULL;
    char *ptr, *tptr;
    Boolean spaced = False;

    token = (XbrHTMLToken *) malloc(sizeof(XbrHTMLToken));
    if(token) {
        /* +2 in case we add a space later */
        token->text = malloc(end-start+2);
        if(token->text == NULL) {
            free(token);
            token = NULL;
        } else {
            for(ptr = start, tptr = token->text; ptr != end;) {
                /* Collapse spaces into single space */
                if(isspace(*ptr)) {
                    /* Ignore space if we have added one already. */
                    if(preformatted == True) {
                        *tptr++ = *ptr;
                    } else if(spaced == False) {
                        *tptr++ = ' ';
                        spaced = True;
                    }
                    ptr++;
                } else if(*ptr == '&') {
                    /* If this is the case then the text is encoded. */
                    ptr++;
                    *tptr = _XbrHTMLEscape(&ptr);
                    /* If we couldn't escape it then make it an ampersand. */
                    if(*tptr == 0)
                        *tptr = '&';
                    tptr++;
                    spaced = False;
                } else {
                    *tptr++ = *ptr++;
                    spaced = False;
                }
            }
            *tptr = 0;
            *(tptr+1) = 0;
            token->tag = XbrHTMLTAG_NONE;
            token->next = NULL;
        }
    }
    return(token);
}

/*-----------------------------------------------------------------------------
  _XbrHTMLEscape()
  In HTML documents special characters can be added by using the ampersand
  operator. After the ampersand '&' character may come a special code or an
  ISO LATIN character number - these are converted into the correct 'char'
  value.

  char **text	Pointer to escaped text
  ----------------------------------------------------------------------------*/
static char _XbrHTMLEscape(char **text)
{
    int i;
    char c, escape, *ptr;

    /* A hash indicates that a character value follows. */
    if(**text == '#') {
        ptr = (*text+1);
        while(isdigit(*ptr)) ptr++;
        c = *ptr;
        *ptr = 0;
        escape = (char) atoi(*text+1);
        *ptr = c;
        if(*ptr == ';') {
            *text = ptr+1;
        } else {
            *text = ptr;
        }
        return(escape);
    } else {
        for(i = 0; i < XbrHTMLNumEscapes; i++) {
            if(!strncmp(XbrHTMLEscapes[i].tag, *text,
              strlen(XbrHTMLEscapes[i].tag))){
                *text += strlen(XbrHTMLEscapes[i].tag);
                if(**text == ';') (*text)++;
                return(XbrHTMLEscapes[i].value);
            }
        }
    }

    return(0);
}
