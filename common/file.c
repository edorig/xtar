/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	file.c
 * CREATED:	Tue Oct 26 1993
 * AUTHOR:	Rik Turnbull
 * DESCRIPTION:	Miscellaneous file handling routines.
 *
 */

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <utime.h>
#include <time.h>

#include "stat.h"		/* We use stat_size()           */
#include "fname.h"
#include "magic.h"

int file_exists(char *);	/* Does file exist?             */
int file_copy(char *, char *);	/* UNIX cp command              */
char *file_load(char *, int *);	/* Load whole file into memory  */
char *file_uncompress(char *);  /* Uncompress or ungzip a file  */
char *file_stdin();		/* Load all stdin to memory     */
int file_remove(char *);      /* Remove a file or directory   */

/* file_exists:***************************************************************/
/* file_exists: See if we can stat this file.                                */
/* file_exists:***************************************************************/

int file_exists(char *filename)
{
    struct stat statbuf;

    if(stat(filename, &statbuf) < 0)
        return(0);
    else
        return(1);
}

/* file_copy:*****************************************************************/
/* file_copy: Just a straight read -> write from one file to another given   */
/* file_copy: the file names.                                                */
/* file_copy:*****************************************************************/

int file_copy(char *fromname, char *toname)
{
    char buffer[1024];
    int fromptr, toptr, readbytes;

    /* Open source file for reading */
    if((fromptr = open(fromname, O_RDONLY)) < 0)
        return(0);

    /* Open destination file for writing */
    if((toptr = open(toname, O_WRONLY | O_CREAT, (mode_t) 0660)) < 0) {
        close(fromptr);
        return(0);
    }

    /* Copy the file across */
    while((readbytes = read(fromptr, buffer, 1024)) > 0) {
        if(write(toptr, buffer, readbytes) < 0) {
            close(fromptr);
            close(toptr);
            return(0);
        }
    }

    close(fromptr);
    close(toptr);

    /* Was the last read() an error? */
    if(readbytes == -1)
         return(0);

    return(1);
}

/* file_load:*****************************************************************/
/* file_load: Read the entire contents of a file into memory.                */
/* file_load:*****************************************************************/

char *file_load(char *filename, int *retsize)
{
    int size = stat_size(filename);
    FILE *fp;
    char *buffer;

    if((buffer = (char *) malloc(size+1)) == NULL) {
        if(retsize) *retsize = 0;
        return(NULL);
    }

    if((fp = fopen(filename, "r")) == NULL) {
        free(buffer);
        if(retsize) *retsize = 0;
        return(NULL);
    }

    if(read(fileno(fp), buffer, size) != size) {
        fclose(fp);
        free(buffer);
        if(retsize) *retsize = 0;
        return(NULL);
    }

    if(retsize) *retsize = size;

    buffer[size] = 0;

    fclose(fp);

    return(buffer);
}

/* file_uncompress:**********************************************************/
/* file_uncompress: Uncompress the given file with a simple system call.    */
/* file_uncompress:**********************************************************/

char *file_uncompress(char *filename)
{
    static char newfile[MAXFNAMELEN];
    char command[BUFSIZ], *ptr, *suffix = NULL;

    /* Filename to be returned */
    strcpy(newfile, filename);

    /* Uncompress depending on compression method */
    if(is_gzipped(filename)) {
        if(!env_inpath("gunzip"))
            return(NULL);
        sprintf(command, "gunzip -f %s", filename);
        if(!system(command))
            return(NULL);
        suffix = ".gz";
    }
    else if(is_compressed(filename)) {
        if(!env_inpath("uncompress"))
            return(NULL);
        sprintf(command, "uncompress -f %s", filename);
        if(!system(command))
            return(NULL);
        suffix = ".Z";
    }
    else if (is_bzipped(filename)) {
      if(!env_inpath("bunzip2"))
	return(NULL);
      sprintf(command,"bunzip2 -f %s",filename);
      if (!system(command))
	return(NULL);
      suffix=".bz2";
    }
    else if (is_xzipped(filename)) {
	if(!env_inpath("unxz")) 
	  return(NULL);
	sprintf(command,"unxz -f %s",filename);
	if (!system(command))
	return(NULL);
      suffix=".xz";
      }
    /* Strip of suffix if it was there */
    if(suffix != NULL) {
        if((ptr = strstr(newfile, suffix)) != NULL)
            *ptr = 0;
    }

    /* Return uncompressed filename */
    return(newfile);
}

/* file_stdin:****************************************************************/
/* file_stdin: Read ALL stdin to memory.                                     */
/* file_stdin:****************************************************************/

char *file_stdin()
{
    int readbytes = 0, first = 1, total = 0;
    char buffer[BUFSIZ+1], *newmem, *reply = NULL;

    /* Read the info from stdin and stick it in a buffer */
    while((readbytes = read(0, buffer, BUFSIZ)) > 0) {
        total += readbytes;
        buffer[readbytes] = 0;
        if((newmem = (char *) realloc(reply, total + 1)) == NULL) {
           if(reply != NULL) free(reply);
           return(NULL);
        }
        reply = newmem;
        /* First time do a strcpy() the rest do a strcat() */
        if(first) {
            strcpy(reply, buffer);
            first = 0;
        }
        else
            strcat(reply, buffer);
    }

    return(reply);
}

/* file_stamp:****************************************************************/
/* file_stamp: Change the date stamp on a file.                              */
/* file_stamp:****************************************************************/

int file_stamp(char *filename, time_t stamp)
{
    int done;
#ifdef _USE_UTIMES
    struct timeval tvp[2];

    tvp[0].tv_sec  = tvp[1].tv_sec = stamp;
    tvp[0].tv_usec = tvp[1].tv_usec = 0;

    done = (utimes(filename, tvp) == 0);
#else
    struct utimbuf utimbuf;

    utimbuf.actime = stamp;
    utimbuf.modtime = stamp;

    done = (utime(filename, &utimbuf) == 0);
#endif

    return(done);
}

/* file_remove:***************************************************************/
/* file_remove: Remove a file or directory.                                  */
/* file_remove:***************************************************************/
int file_remove(char *filename)
{
    char rmname[256];
    DIR *dirp;
    struct dirent *filep;

    if(filename) {
        if(stat_isdir(filename)) {
            dirp = opendir(filename);
            for(filep = readdir(dirp); filep != NULL; filep = readdir(dirp)) {
                if(strcmp(filep->d_name,".")!=0&&strcmp(filep->d_name,"..")!=0){
                    sprintf(rmname, "%s/%s", filename, filep->d_name);
                    file_remove(rmname);
                }
            } 
            closedir(dirp);
            rmdir(filename);
        } else {
            unlink(filename);
        }
    }

    return(1);
}
