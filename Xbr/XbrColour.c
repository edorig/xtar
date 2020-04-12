/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	XbrColour.c
 * CREATED:	Thu Oct 28 1993
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: Routines to allocate and free up colours.
 *
 */
#include <math.h> 
#include <Xm/Xm.h>

#define	XbrColour_MAX_COLOURS	256

void XbrColourAllocate(Display *, Colormap, XColor *);
void XbrColoursFree(Display *, Colormap, unsigned long [], int);

static void _XbrColourBest(Display *, Colormap, XColor *);

static Boolean initialised = False;
static unsigned long assigned[XbrColour_MAX_COLOURS];
static XColor cmap[XbrColour_MAX_COLOURS];

/*-----------------------------------------------------------------------------
  XbrColourAllocate()
  We keep track of the colour cells that have been grabbed by the application.
  We allocate up to a maximum number of colours and then we just use the
  best match from the colours we have. If the application frees the colours
  then we check that they are not used in any other images and if so
  we don't actually perform the free operation.
  ----------------------------------------------------------------------------*/
void XbrColourAllocate(
 Display *display,	/* Display to allocate colours on */
 Colormap colourmap,	/* Colourmap to allocate colours from */
 XColor *colour		/* RGB colour specification */ )
{
    int i;

    if(initialised == False) {
        for(i = 0; i < XbrColour_MAX_COLOURS; i++)
            assigned[i] = 0;
        initialised = True;
    }
    if(XAllocColor(display, colourmap, colour)) {
        assigned[colour->pixel]++;
        cmap[colour->pixel].pixel = colour->pixel;
        cmap[colour->pixel].red = colour->red;
        cmap[colour->pixel].green = colour->green;
        cmap[colour->pixel].blue = colour->blue;
        cmap[colour->pixel].flags = colour->flags;
    } else {
        _XbrColourBest(display, colourmap, colour);
        assigned[colour->pixel]++;
    }
}

/*-----------------------------------------------------------------------------
  XbrColourFree()
  If the colour count for this pixel has reached zero then free up the
  colour, otherwise just decrement the counter.
  ----------------------------------------------------------------------------*/
void XbrColoursFree(
 Display *display,	/* Display to allocate colours on */
 Colormap colourmap,	/* Colourmap to search colours on */
 unsigned long pixels[],/* Array of pixels to free */
 int npixels		/* Number of pixels to free */ )
{
    int i, npixfree = 0;
    unsigned long pixfree[XbrColour_MAX_COLOURS];

    for(i = 0; i < npixels; i++) {
        if(assigned[pixels[i]]) {
            assigned[pixels[i]]--;
            if(assigned[pixels[i]] == 0)
                pixfree[npixfree++] = pixels[i];
        }
    }

    if(npixfree)
        XFreeColors(display, colourmap, pixfree, npixfree, NULL);
}

/*-----------------------------------------------------------------------------
  _XbrColourBest()
  Look through the colours we have so far and send back the best match.
  ----------------------------------------------------------------------------*/
static void _XbrColourBest(
 Display *display,	/* Display to allocate colours on */
 Colormap colourmap,	/* Colourmap to search colours on */
 XColor *colour		/* RGB colour specification */ )
{
    int i, value, best = 0, best_value = 65535 * 3;

    for(i = 0; i < XbrColour_MAX_COLOURS; i++) {
        if(assigned[i]) {
            value = abs(colour->red - cmap[i].red) +
              abs(colour->green - cmap[i].green) +
              abs(colour->blue - cmap[i].blue);

            if(value == 0) {
                best = i;
                break;
            }

            if(value < best_value) {
                best_value = value;
                best = i;
            }
        }
    }

    colour->pixel = best;
}
