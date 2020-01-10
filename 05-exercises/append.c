/*
 *  The Linux Programming Interface
 *  Exercise 5-2
 */

#include <fcntl.h>
#include <tlpi.h>

int main(int argc, char** argv)
{
    int fd;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usage_error("%s <filename>\n", argv[0]);

    // Open existing file
    fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1)
        error_exit("open");

    // Seek to start of file
    if (lseek(fd, 0, SEEK_SET) == -1)
        error_exit("lseek");

    // Write test bytes
    char buf[10] = "teststring";
    if (write(fd, buf, 10) == -1)
        error_exit("write");

    if (close(fd) == -1)
        error_exit("close");

    exit(EXIT_SUCCESS);
}
