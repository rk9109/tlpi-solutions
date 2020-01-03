/*
 *  The Linux Programming Interface
 *  Exercise 4-1
 */

#include <fcntl.h>
#include <tlpi.h>

#define BUF_SIZE 1024

int main(int argc, char** argv)
{
    int opt, fd, numread;
    int flags = O_WRONLY | O_CREAT;
    char* fname;
    char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usage_error("%s [-a] <filename>\n", argv[0]);
    }

    fname = argv[optind];
    while ((opt = getopt(argc, argv, "a:")) != -1) {
        switch (opt) {
        case 'a':
            flags |= O_APPEND;
            fname = optarg;
            break;
        case '?':
            usage_error("%s [-a] <filename>\n", argv[0]);
            break;
        }
    }

    // open output file
    fd = open(fname, flags, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        error_exit("open");
    }

    while ((numread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        // redirect to standard out
        if (write(STDOUT_FILENO, buf, numread) != numread) {
            error_exit("write");
        }
        // redirect to output file
        if (write(fd, buf, numread) != numread) {
            error_exit("write");
        }
    }

    // close file
    if (close(fd) == -1) {
        error_exit("close");
    }

    exit(EXIT_SUCCESS);
}
