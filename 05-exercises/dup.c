/*
 *  The Linux Programming Interface
 *  Exercise 5-4 and 5-5
 */

#include <assert.h>
#include <fcntl.h>
#include <tlpi.h>

// Returns (new) file descriptor on success, or -1 on error
int dup_tlpi(int fd)
{
    return fcntl(fd, F_DUPFD);
}

// Returns (new) file descriptor on success, or -1 on error
int dup2_tlpi(int fd_old, int fd_new)
{
    if (fcntl(fd_old, F_GETFL) == -1) {
        return -1;
    }
    if (fd_old == fd_new) {
        return fd_old;
    }
    // F_DUPFD returns the smallest file descriptor greater than
    // or equal to `arg`. Closing `fd_new` guarantees that the
    // duplicated file descriptor is `fd_new`.
    //
    // NOTE:
    //   This implementation is not protected against race conditions.
    //   A separate process could open `fd_new` between calls to close()
    //   and fcntl().
    if ((fcntl(fd_new, F_GETFD) != -1) || errno != EBADF) {
        if (close(fd_new) == -1)
            return -1;
    }
    return fcntl(fd_old, F_DUPFD, fd_new);
}

#define OFFSET 128

int main(int argc, char** argv)
{
    int fd, fd_dup, fd_dup2;
    int flags, flags_init;
    off_t offset;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usage_error("%s <filename>\n", argv[0]);

    // Open test file (rw-------)
    fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        error_exit("open");
    flags_init = fcntl(fd, F_GETFL);
    if (flags_init == -1)
        error_exit("fcntl");

    // Update original offset
    if (lseek(fd, OFFSET, SEEK_SET) == -1)
        error_exit("lseek");

    if ((fd_dup = dup_tlpi(fd)) != -1) {
        // Test `dup_tlpi` offset
        offset = lseek(fd_dup, 0, SEEK_CUR);
        if (offset == -1)
            error_exit("lseek");
        assert(offset == OFFSET);

        // Test `dup_tlpi` flags
        flags = fcntl(fd_dup, F_GETFL);
        if (flags == -1)
            error_exit("fcntl");
        assert(flags == flags_init);
    } else {
        error_exit("dup_tlpi");
    }

    if ((fd_dup2 = dup2_tlpi(fd, 2 * fd)) != -1) {
        // Test `dup2_tlpi` offset
        offset = lseek(fd_dup2, 0, SEEK_CUR);
        if (offset == -1)
            error_exit("lseek");
        assert(offset == OFFSET);

        // Test `dup2_tlpi` flags
        flags = fcntl(fd_dup2, F_GETFL);
        if (flags == -1)
            error_exit("fcntl");
        assert(flags == flags_init);
    } else {
        error_exit("dup2_tlpi");
    }

    if (close(fd) == -1)
        error_exit("close");

    printf("Assertions passed\n");
    exit(EXIT_SUCCESS);
}
