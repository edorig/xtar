/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:   XbrHTMLEscapes.h
 * CREATED:     Sat May 27 1995
 * AUTHOR:      Rik Turnbull
 * DESCRIPTION: Header file containing HTML escape sequences.
 *
 */

#ifndef	_XBR_XBRHTMLESCAPES_H
#define	_XBR_XBRHTMLESCAPES_H

#define	XbrHTMLNumEscapes (sizeof(XbrHTMLEscapes)/sizeof(XbrHTMLEscapes[0]))

typedef struct _XbrHTMLEscape {
	char *tag;
	char value;
} XbrHTMLEscape;

static XbrHTMLEscape XbrHTMLEscapes[] = {
	{"lt", '<'},
	{"LT", '<'},
	{"gt", '>'},
	{"GT", '>'},
	{"amp", '&'},
	{"AMP", '&'},
	{"quot", '\"'},
	{"QUOT", '\"'},
	{"Agrave", '\300'},
	{"Aacute", '\301'},
	{"Acirc", '\302'},
	{"Atilde", '\303'},
	{"Auml", '\304'},
	{"Aring", '\305'},
	{"AElig", '\306'},
	{"Ccedil", '\307'},
	{"Egrave", '\310'},
	{"Eacute", '\311'},
	{"Ecirc", '\312'},
	{"Euml", '\313'},
	{"Igrave", '\314'},
	{"Iacute", '\315'},
	{"Icirc", '\316'},
	{"Iuml", '\317'},
	{"ETH", '\320'},
	{"Ntilde", '\321'},
	{"Ograve", '\322'},
	{"Oacute", '\323'},
	{"Ocirc", '\324'},
	{"Otilde", '\325'},
	{"Ouml", '\326'},

	{"?", '\327'}, /* ? */

	{"Oslash", '\330'},
	{"Ugrave", '\331'},
	{"Uacute", '\332'},
	{"Ucirc", '\333'},
	{"Uuml", '\334'},
	{"Yacute", '\335'},
	{"THORN", '\336'},
	{"szlig", '\337'},
	{"agrave", '\340'},
	{"aacute", '\341'},
	{"acirc", '\342'},
	{"atilde", '\343'},
	{"auml", '\344'},
	{"aring", '\345'},
	{"aelig", '\346'},
	{"ccedil", '\347'},
	{"egrave", '\350'},
	{"eacute", '\351'},
	{"ecirc", '\352'},
	{"euml", '\353'},
	{"igrave", '\354'},
	{"iacute", '\355'},
	{"icirc", '\356'},
	{"iuml", '\357'},
	{"eth", '\360'},
	{"ntilde", '\361'},
	{"ograve", '\362'},
	{"oacute", '\363'},
	{"ocirc", '\364'},
	{"otilde", '\365'},
	{"ouml", '\366'},

	{"?", '\367'}, /* ? */

	{"oslash", '\370'},
	{"ugrave", '\371'},
	{"uacute", '\372'},
	{"ucirc", '\373'},
	{"uuml", '\374'},
	{"yacute", '\375'},
	{"thorn", '\376'},
	{"yuml", '\377'},

	{NULL, '\0'},
};

#endif
