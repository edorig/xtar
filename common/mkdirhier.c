/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	mkdirhier.c
 * CREATED:	Mon Feb  7 1994
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: This is an implementation of the mkdirhier script which creates
 *              a directory and if its parents do not exist it creates them as
 *              well.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "fname.h"

int mkdirhier(char *, mode_t);

/* mkdirhier:*****************************************************************/
/* mkdirhier: This an implementation of the mkdirhier script which creates a */
/* mkdirhier: directory and if its parents do not exist it creates them as   */
/* mkdirhier: well.                                                          */
/* mkdirhier:*****************************************************************/

int mkdirhier(char *dirname, mode_t mode)
{
    char name[MAXFNAMELEN], create[MAXFNAMELEN], *ptr;

    /* Make a copy of the directory name */
    strcpy(name, dirname);
    
    /* Split it up into tokens with / as the split character */
    ptr = strtok(name, "/");
    if(dirname[0] == '/')
        sprintf(create, "/%s", ptr);
    else
        strcpy(create, ptr);

    while(ptr) {
        /* If the file exists do nothing */
        if(file_exists(create)) {
            /* If it is not a directory then return */
            if(!stat_isdir(create))
                return(0);
        } else {
            if(mkdir(create, mode) != 0)
                return(0);
        }
        ptr = strtok(NULL, "/");
        /* Better not strcat a NULL pointer */
        if(ptr != (char *)NULL) {
            strcat(create, "/");
            strcat(create, ptr);
        }
    }

    return(1);
}
