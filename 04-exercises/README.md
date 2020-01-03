# Chapter 4: File I/O - The Universal I/O Model

## Exercise 4-1

**Question:**

The `tee` command reads its standard input until end-of-file, writing a copy of the input to standard output and to the file named in its command-line argument. (We show an example of the use of this command when we discuss FIFOs in Section 44.7.) Implement `tee` using I/O system calls. By default, `tee` overwrites any existing file with the given name. Implement the `-a` command-line option (`tee -a file`), which causes `tee` to append text to the end of a file if it already exists. (Refer to Appendix B for a description of the `getopt()` function, which can be used to parse command-line options.)

**Answer:**

See `tee.c`:
```sh
$ echo "teststring" | ./tee testfile
teststring
$ cat testfile
teststring
$ echo "teststring" | ./tee -a testfile
teststring
$ cat testfile
teststring
teststring
```

## Exercise 4-2

**Question:**

Write a program like `cp` that, when used to copy a regular file that contains holes (sequences of null bytes), also creates corresponding holes in the target file.

**Answer:**

See `hole.c` and `cp.c`:
```sh
$ ./hole test1 # create file containing holes
$ cat test1
teststring          teststring          teststring          teststring          teststring
$ ./cp test1 test2
$ cat test2
teststring          teststring          teststring          teststring          teststring
```
