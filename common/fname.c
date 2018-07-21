/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	fname.c
 * CREATED:	Mon Oct 25 GMT 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Simple filename operations.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "fname.h"

void fname_change(char *, char *, char *);
char *fname_basename(char *);
char *fname_dirname(char *);
char *fname_resolve(char *, ...);     /* Resolve .. and . in filename */

/* fname_change:**************************************************************/
/* fname_change: If a filename has one ending then change it to the  new one */
/* fname_change: Presumes that it can overwrite the name passed in.          */
/* fname_change:**************************************************************/

void fname_change(char *filename, char *from, char *to)
{
    char *ptr;

    if((ptr = strrchr(filename, '.')) == NULL)
        return;

    if(!strcmp(ptr, from))
        strcpy(ptr, to);
}

/* fname_basename:************************************************************/
/* fname_basename: Does the equivalent of the UNIX basename().               */
/* fname_basename:************************************************************/

char *fname_basename(char *filename)
{
    char *ptr;

    if((ptr = strrchr(filename, '/')) == NULL)
        ptr = filename;
    else
        ptr++;

    return(ptr);
}

/* fname_dirname:*************************************************************/
/* fname_dirname: Does the equivalent of the UNIX dirname().                 */
/* fname_dirname:*************************************************************/

char *fname_dirname(char *filename)
{
    static char dirname[MAXFNAMELEN];
    char *ptr;

    strcpy(dirname, filename);

    if((ptr = strrchr(dirname, '/')) != NULL)
        *ptr = 0;
    else
        strcpy(dirname,".");
    return(dirname);
}

/* fname_resolve:*************************************************************/
/* fname_resolve: Given a filename with possible .'s and ..'s in it, remove  */
/* fname_resolve: then and return the actual filename.                       */
/* fname_resolve:*************************************************************/

char *fname_resolve(char *format, ...)
{
    va_list argp;
    char buffer[256], *ptr, *endptr;
    static char newname[256];

    newname[0] = 0;

    /* Get format string */
    va_start(argp, format);
    vsprintf(buffer, format, argp);
    va_end(argp);

    /* Split the filename into its paths */
    ptr = strtok(buffer, "/");
    while(ptr != NULL) {
        /* Skip any dots on their own */
        if(!strcmp(ptr, ".")) {
        }
        /* Double dot means back up one so delete last directory from
           current path.
        */
        else if(strlen(ptr) == 2 && !strcmp(ptr, "..")) {
            if((endptr = strrchr(newname, '/')) == NULL) {
                return(NULL);
            }
            *endptr = 0;
        } else {
            if(newname[0] != 0 || buffer[0] == '/')
                strcat(newname, "/");
            strcat(newname, ptr);
        }
        ptr = strtok(NULL, "/");
    }

    return(newname);
}
