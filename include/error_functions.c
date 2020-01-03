/*
 *  The Linux Programming Interface
 *  Standard error handling functions
 */

#include <stdarg.h>

#include <error_functions.h>
#include <tlpi.h>

#define BUF_SIZE 1024

#ifdef __GNUC__
__attribute__((__noreturn__))
#endif
static void terminate(bool use_exit3)
{
    char *s;

    // Dump core if EF_DUMPCORE environment variable is defined
    // and is a nonempty string; call exit(3) or _exit(2) otherwise
    // based on `use_exit3`.
    s = getenv("EF_DUMPCORE");
    
    if (s != NULL && *s != '\0') {
        abort();
    }
    else if (use_exit3) {
        exit(EXIT_FAILURE);
    }
    else {
        _exit(EXIT_FAILURE);
    }
}

static void error_output(bool use_error, int error, bool flush_stdout, 
        const char* format, va_list ap)
{
    char buf[BUF_SIZE];
    char user_message[BUF_SIZE];
    char error_message[BUF_SIZE];

    vsnprintf(user_message, BUF_SIZE, format, ap);

    if (use_error) {
        snprintf(error_message, BUF_SIZE, " [%s]", strerror(error));
    }
    else {
        snprintf(error_message, BUF_SIZE, ":");
    }

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", error_message, user_message);

    // Flush pending stdout
    if (flush_stdout) {
        fflush(stdout);
    }
    fputs(buf, stderr);
    // Flush pending stderr
    // (necessary if stderr is not line-buffered)
    fflush(stderr);
}

// Print an error message using `errno` and return to
// caller
void error_message(const char* format, ...)
{
    va_list arglist;
    int saved_errno;

    saved_errno = errno;

    va_start(arglist, format);
    error_output(true, errno, true, format, arglist);
    va_end(arglist);

    errno = saved_errno;
}

// Print an error message using `errno` and terminate
// the process
void error_exit(const char* format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    error_output(true, errno, true, format, arglist);
    va_end(arglist);

    terminate(true);
}

// Print error message and terminate the process
void fatal(const char* format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    error_output(false, 0, true, format, arglist);
    va_end(arglist);

    terminate(true);
}

// Print usage error and terminate the process
void usage_error(const char* format, ...)
{
    va_list arglist;

    // Flush pending stdout
    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
    va_end(arglist);

    // Flush pending stderr
    // (necessary if stderr is not line-buffered)
    fflush(stderr);

    exit(EXIT_FAILURE);
}

// Print command-line usage error and terminate the process
void cmdline_error(const char* format, ...)
{
    va_list arglist;

    // Flush pending stdout
    fflush(stdout);

    fprintf(stderr, "Command-line usage error: ");
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
    va_end(arglist);

    // Flush pending stderr
    // (necessary if stderr is not line-buffered)
    fflush(stderr);

    exit(EXIT_FAILURE);
}
