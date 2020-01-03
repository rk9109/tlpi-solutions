/*
 *  The Linux Programming Interface
 *  Standard header in exercise solutions
 */

#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// error handling functions
#include <error_functions.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#endif
