# Chapter 5: File I/O - Further Details

## Exercise 5-1

**Question:**
If you have access to a 32-bit Linux system, modify the program in Listing 5-3 to use standard file I/O system calls (`open()` and `lseek()`) and the `off_t` data type. Compile the program with the `_FILE_OFFSET_BITS` macro set to 64, and test it to show that a large file can be successfully created.

**Answer:**

*skipped*

## Exercise 5-2

**Question:**

Write a program that opens an existing file for writing with the `O_APPEND` flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?

**Answer:**

See `append.c`:
```sh
$ echo "teststring" > tmp
$ ./append tmp
$ cat tmp
teststring
teststring
```
See `man open(2)`. The `O_APPEND` flag repositions the file offset to the end of the file before each `write()` (in a single atomic operation). As a result, changing the file offset manually using `lseek()` does not change the location of written data.

## Exercise 5-3

**Question:**

This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the `O_APPEND` flag is necessary. Write a program that takes up to three command-line arguments:
```sh
$ atomic_append <filename> <num-bytes> [x]
```
This program should open the specified filename (creating it if necessary) and append *num-bytes* bytes to the file by using `write()` to write a byte at a time. By default, the program should open the file with the `O_APPEND` flag, but if a third command-line argument (*x*) is specified, then the `O_APPEND` flag should be omitted, and instead the program should perform an `lseek(fd, 0, SEEK_END)` call before each `write()`. Run two instances of this program at the same time without the *x* argument to write 1 million bytes to the same file:
```sh
$ atomic_append f1 1000000 & atomic_append f1 1000000
```
Repeat the same steps, writing to a different file, but this time specifying the *x* argument:
```sh
$ atomic_append f2 1000000 x & atomic_append f2 1000000 x
```
List the sizes of the files `f1` and `f2` using `ls -l` and explain the difference.

**Answer:**

See `atomic_append.c`:
```sh
$ ./atomic_append f1 100000 & ./atomic_append f1 100000
$ ./atomic_append f2 100000 x & ./atomic_append f2 100000 x
$ ls -l
...
-rw------- 1 rinik rinik 200000 Jan  9 22:00 f1
-rw------- 1 rinik rinik 100867 Jan  9 22:00 f2
...
```
The difference in size is large. The race condition occurs when 2 processes attempt to `lseek()` at the same time. Suppose the first process calls `lseek(fd, 0, SEEK_END)` and is interrupted by the second process doing the same thing. Both processes set their file offsets to the same location, and one process will overwrite the other when `write()` is called. The processes overwriting each other result in `f2` being nearly 2 times smaller than `f1`.

## Exercise 5-4

**Question:**

Implement `dup()` and `dup2()` using `fcntl()` and, where necessary, `close()`. (You may ignore the fact that `dup2()` and `fcntl()` return different `errno` values for some error cases.) For `dup2()`, remember to handle the special case where `oldfd` equals `newfd`. In this case, you should check whether `oldfd` is valid, which can be done by, for example, checking if `fcntl(oldfd, F_GETFL)` succeeds. If `oldfd` is not valid, then the function should return -1 with `errno` set to `EBADF`.

**Answer:**

See `dup_tlpi()` and `dup2_tlpi()` in `dup.c`.

## Exercise 5-5

**Question:**

Write a program to verify that duplicated file descriptors share a file offset value and open file status flags.

**Answer:**

See `main()` in `dup.c`.

## Exercise 5-6

**Question:**

After each of the calls to `write()` in the following code, explain what the content of the output file would be, and why:
```C
fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, " world", 6);
lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
```

**Answer:**
```C
write(fd1, "Hello,", 6);
```
The output file contains the string `Hello,`.
```C
write(fd2, " world", 6);
```
Since `fd2` is a duplicate of `fd1`, the 2 file descriptors share the same file offset. As a result, the output file is not overwritten, and contains the string `Hello, world`.
```C
write(fd1, "HELLO,", 6);
```
The call to `lseek()` resets the offset of `fd2` to 0. Since `fd1` and `fd2` share the same file offset, the call to `write()` overwrites the first 6 characters, resulting in the string `HELLO, world`.
```C
write(fd3, "Gidday", 6);
```
Since `fd3` was returned from a separate call to `open()`, `fd3` does not share the same file offset as `fd1` and `fd2`. As a result, the call to `write()` overwrites the first 6 characters again, resulting in the string `Gidday world`.

## Exercise 5-7

**Question:**

Implement `readv()` and `writev()` using `read()`, `write()`, and suitable functions from the `malloc` package (Section 7.1.2).

**Answer:**

See `readv_tlpi()` and `writev_tlpi()` in `vectorio.c`.
