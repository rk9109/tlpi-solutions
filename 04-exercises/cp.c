/*
 *  The Linux Programming Interface
 *  Exercise 4-2
 */

#include <fcntl.h>
#include <tlpi.h>

#define BUF_SIZE 1024

int main(int argc, const char** argv)
{
    int inputfd, outputfd;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        usage_error("%s <oldfile> <newfile>", argv[0]);
    }

    // Open source file
    inputfd = open(argv[1], O_RDONLY);
    if (inputfd == -1) {
        error_exit("open");
    }

    // Open destination file (rw-------)
    outputfd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (outputfd == -1) {
        error_exit("open");
    }

    int numread;
    int size = 0;

    // NOTE:
    //   Linux (Version 3.1) supports additional options to lseek() SEEK_DATA
    //   and SEEK_HOLE; could be used to improve this solution.
    //
    //   SOURCE: https://github.com/posborne/linux-programming-interface-exercises
    while ((numread = read(inputfd, buf, BUF_SIZE)) > 0) {
        for (int i = 0; i < numread; i++) {
            if (buf[i] == '\0') {
                size++;
            } else if (size != 0) {
                if (lseek(outputfd, size, SEEK_CUR) == -1)
                    error_exit("lseek");
                if (write(outputfd, buf + i, 1) != 1)
                    error_exit("write");
                size = 0;
            } else {
                if (write(outputfd, buf + i, 1) != 1) {
                    error_exit("write");
                }
            }
        }
    }

    if (close(inputfd) == -1) {
        error_exit("close");
    }
    if (close(outputfd) == -1) {
        error_exit("close");
    }

    exit(EXIT_SUCCESS);
}
