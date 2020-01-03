/*
 *  The Linux Programming Interface
 *  Standard error handling functions
 */

#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

// Prevent 'gcc -Wall' complaining that "control reaches 
// end of non-void function" when the error handlers are 
// used to terminate main() or some other non-void function. 
#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

// Print an error message using `errno` and return to
// caller
void error_message(const char *format, ...);

// Print an error message using `errno` and terminate
// the process
void error_exit(const char *format, ...) NORETURN;

// Print error message and terminate the process
void fatal(const char *format, ...) NORETURN;

// Print usage error and terminate the process
void usage_error(const char *format, ...) NORETURN;

// Print command-line usage error and terminate the process
void cmdline_error(const char *format, ...) NORETURN;

#endif
