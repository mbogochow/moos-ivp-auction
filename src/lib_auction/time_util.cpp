/*
 * Mike Bogochow
 * March 8, 2015
 * time_util.c
 *
 * Utilities for dealing with POSIX time functions.
 */

#define _POSIX_C_SOURCE 200809L
#define _ISOC99_SOURCE
#define _XOPEN_SOURCE 700
#define __EXTENSIONS__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>

#include "time_util.h"

/**
 * Subtract the struct timespec or struct timeval values x and y, storing the
 * result in the result parameter.
 * Return 1 if the difference is negative, otherwise 0.
 *
 * From GNU site: gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */
#define time_subtract(result, x, y, field, units)                              \
  if (x->field < y->field) {                                                   \
    int nsec = (y->field - x->field) / units + 1;                              \
    y->field -= units * nsec;                                                  \
    y->tv_sec += nsec;                                                         \
  }                                                                            \
  if (x->field - y->field > units) {                                           \
    int nsec = (x->field - y->field) / units;                                  \
    y->field += units * nsec;                                                  \
    y->tv_sec -= nsec;                                                         \
  }                                                                            \
  result->tv_sec = x->tv_sec - y->tv_sec;                                      \
  result->field = x->field - y->field;                                         \
  return x->tv_sec < y->tv_sec;


void ms_to_timespec(struct timespec *result, const long ms)
{
  result->tv_sec = MS_TO_SEC(ms);
  result->tv_nsec = MS_TO_NS((ms - SEC_TO_MS(result->tv_sec)));
} /* ms_to_timespec */



double timespec_to_ms(struct timespec *time)
{
  // assuming no overflow for this application
  return (double)SEC_TO_MS(time->tv_sec) + NS_TO_MS((double)time->tv_nsec);
} /* timespec_to_ms */



double timespec_to_ns(struct timespec *time)
{
  // assuming no overflow for this application
  return (double)SEC_TO_NS(time->tv_sec) + (double)time->tv_nsec;
} /* timespec_to_ns */



int timespec_subtract(struct timespec *result, struct timespec *x,
    struct timespec *y)
{
  time_subtract(result, x, y, tv_nsec, NS_UNITS)
} /* timeval_subtract */



double timeval_to_ms(struct timeval *time)
{
  // assuming no overflow for this application
  return (double)SEC_TO_MS(time->tv_sec) + US_TO_MS((double)time->tv_usec);
} /* timeval_to_ms */



int timeval_subtract(struct timeval *result, struct timeval *x,
    struct timeval *y)
{
  time_subtract(result, x, y, tv_usec, US_UNITS)
} /* timeval_subtract */
