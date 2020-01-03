/*
 *  The Linux Programming Interface
 *  Exercise 4-2
 */

#include <fcntl.h>
#include <tlpi.h>

#define HOLE_NUM 5
#define HOLE_SIZE 10
#define PATTERN "teststring"
#define PATTERN_SIZE 10

int main(int argc, char** argv)
{
    int fd;
    off_t offset = 0;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usage_error("%s <filename>\n", argv[0]);
    }

    // Open new file (rw-------)
    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        error_exit("open");
    }

    // Write PATTERN_SIZE bytes; then seek an additional HOLE_SIZE bytes
    // past the end of the file and repeat
    for (int i = 0; i < HOLE_NUM; i++) {
        if (write(fd, PATTERN, PATTERN_SIZE) == -1) {
            error_exit("write");
        }
        offset += PATTERN_SIZE + HOLE_SIZE;
        if (lseek(fd, offset, SEEK_SET) == -1) {
            error_exit("lseek");
        }
    }

    if (close(fd) == -1) {
        error_exit("close");
    }

    exit(EXIT_SUCCESS);
}
