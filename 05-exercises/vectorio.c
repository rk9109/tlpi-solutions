/*
 *  The Linux Programming Interface
 *  Exercise 5-7
 */

#include <assert.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <tlpi.h>

// Returns number of bytes read, 0 on EOF, or -1 on error
ssize_t readv_tlpi(int fd, const struct iovec* iov, int iovcnt)
{
    int numread;
    int totalread = 0;

    if (iovcnt < 0 || iovcnt > __IOV_MAX)
        return -1;

    for (int i = 0; i < iovcnt; i++) {
        numread = read(fd, iov[i].iov_base, iov[i].iov_len);
        if (numread == -1)
            return -1;
        totalread += numread;
        if (numread < iov[i].iov_len) { // EOF
            return totalread;
        }
    }
    return totalread;
}

// Returns number of bytes written, or -1 on error
ssize_t writev_tlpi(int fd, const struct iovec* iov, int iovcnt)
{
    int numwrite;
    int totalwrite = 0;

    if (iovcnt < 0 || iovcnt > __IOV_MAX)
        return -1;

    for (int i = 0; i < iovcnt; i++) {
        numwrite = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (numwrite == -1)
            return -1;
        totalwrite += numwrite;
        if (numwrite < iov[i].iov_len) { // EOF
            return totalwrite;
        }
    }
    return totalwrite;
}

#define IOVEC_CNT 64
#define IOVEC_LEN 10
#define IOVEC_STR "teststring"

static struct iovec iovec_pre[IOVEC_CNT];
static struct iovec iovec_post[IOVEC_CNT];

int main(int argc, char** argv)
{
    int fd, numread, numwrite;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usage_error("%s <filename>\n", argv[0]);

    // Open test file (rw-------)
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        error_exit("open");

    for (int i = 0; i < IOVEC_CNT; i++) {
        // Initialize `iovec_pre`
        iovec_pre[i].iov_base = malloc(IOVEC_LEN);
        iovec_pre[i].iov_base = &IOVEC_STR;
        iovec_pre[i].iov_len = IOVEC_LEN;

        // Initialize `iovec_post`
        iovec_post[i].iov_base = malloc(IOVEC_LEN);
        iovec_post[i].iov_len = IOVEC_LEN;
    }

    // Test `writev_tlpi`
    numwrite = writev_tlpi(fd, iovec_pre, IOVEC_CNT);
    if (numwrite == -1)
        error_exit("writev_tlpi");
    assert(numwrite == (IOVEC_CNT * IOVEC_LEN));

    // Reset offset
    if (lseek(fd, 0, SEEK_SET) == -1)
        error_exit("lseek");

    // Test `readv_tlpi`
    numread = readv_tlpi(fd, iovec_post, IOVEC_CNT);
    if (numread == -1)
        error_exit("readv_tlpi");
    assert(numread == (IOVEC_CNT * IOVEC_LEN));

    // Assert `iovec_pre` == `iovec_post`
    for (int i = 0; i < IOVEC_CNT; i++) {
        assert(memcmp(iovec_pre[i].iov_base, iovec_post[i].iov_base,
                   IOVEC_LEN)
            == 0);
    }

    if (close(fd) == -1)
        error_exit("close");

    printf("Assertions passed\n");
    exit(EXIT_SUCCESS);
}
