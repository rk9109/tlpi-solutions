/*
 *  The Linux Programming Interface
 *  Exercise 5-3
 */

#include <fcntl.h>
#include <tlpi.h>

int main(int argc, char** argv)
{
    int fd;
    int flags = O_WRONLY | O_CREAT;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usage_error("%s <filename> <num-bytes> [x]\n", argv[0]);

    if (argc != 4)
        flags |= O_APPEND;

    // Open file (rw-------)
    fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        error_exit("open");

    int num_bytes = atoi(argv[2]);
    for (int i = 0; i < num_bytes; i++) {
        // NOTE:
        //   Invoking 2 system calls during each iteration of the loop is
        //   expensive. However, this exaggerates the race conditions created
        //   if O_APPEND is not used.
        if (lseek(fd, 0, SEEK_END) == -1)
            error_exit("lseek");
        if (write(fd, "A", 1) == -1)
            error_exit("write");
    }

    if (close(fd) == -1)
        error_exit("close");

    exit(EXIT_SUCCESS);
}
