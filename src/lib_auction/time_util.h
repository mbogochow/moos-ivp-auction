/*
 * Mike Bogochow
 * March 8, 2015
 * time_util.h
 *
 * Utilities for dealing with POSIX time functions.
 */

#include <sys/time.h>
#include <time.h>

#define MS_UNITS 1000L
#define US_UNITS 1000000L
#define NS_UNITS 1000000000L

#define SEC_TO_MS(sec) (sec * MS_UNITS)
#define SEC_TO_NS(sec) (sec * NS_UNITS)
#define NS_TO_MS(ns) (ns / (NS_UNITS / MS_UNITS))
#define US_TO_MS(us) (us / (US_UNITS / MS_UNITS))
#define MS_TO_NS(ms) (ms * (NS_UNITS / MS_UNITS))

#define MS_TO_SEC(ms) (ms / MS_UNITS)

/**
 * Convert milliseconds to a timespec struct. Returns value in result param.
 */
void ms_to_timespec(struct timespec *result, const long ms);

/**
 * Convert and return timespec to milliseconds.
 */
double timespec_to_ms(struct timespec *time);

/**
 * Convert and return timespec to nanoseconds.
 */
double timespec_to_ns(struct timespec *time);

/**
 * Subtract the struct timespec values x and y, storing the result in the
 * result parameter.
 * Return 1 if the difference is negative, otherwise 0.
 */
int timespec_subtract(struct timespec *result, struct timespec *x,
    struct timespec *y);

/**
 * Convert and return timeval to milliseconds.
 */
double timeval_to_ms(struct timeval *time);

/**
 * Subtract the struct timeval values x and y, storing the result in the
 * result parameter.
 * Return 1 if the difference is negative, otherwise 0.
 */
int timeval_subtract(struct timeval *result, struct timeval *x,
    struct timeval *y);
