/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	num.c
 * CREATED:	Wed Oct 27 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION: Number handling bits'n'pieces - random number generator and
 *              something to convert an octal number, stored as a string to
 *              a decimal.
 *
 */

#include <sys/time.h>
#include <ctype.h>
#include <stdlib.h>
int num_random();
long num_decimal(char *, int);

/* num_random:****************************************************************/
/* num_random: Simple call to generate a random number. Wrote this yonks ago */
/* num_random: - it relys on a seed taken from the time of day!              */
/* num_random:****************************************************************/

int num_random()
{

  /* Structs returned by gettimeofday() */
  struct timeval tp;
  struct timezone tzp;

  /* Declare static boolean to seed just once */
  static int seeded = 0;

  if(!seeded) {
    /* Call gettimeofday() with pointers to structs */
    gettimeofday(&tp, &tzp);

    /* Seed the random number generator */
    srand(tp.tv_usec);

    /* Now seeded don't repeat */
    seeded = 1;
  }

  /* Return random number */
  return(rand());
}

/* num_decimal:***************************************************************/
/* num_decimal: Takes an octal number, stored as a string and converts it to */
/* num_decimal: a decimal. This routine comes from the xtar program as nums  */
/* num_decimal: are stored like this in the archive headers.                 */
/* num_decimal:***************************************************************/

long num_decimal(char *n, int len)
{
    int i = 0;
    long value = 0, power = 1;

    /* Get to start/end of numbers */
    len -= 1;
    while(!isdigit(n[--len]));

    /* Do conversion */
    for(i = len; i > 0; i--) {
        if(isdigit(n[i])) {
            value += power * (n[i] - 48);
            power *= 8;
        }
    }

    return(value);
}
