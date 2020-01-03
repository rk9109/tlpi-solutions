# Chapter 3: System Programming Concepts

## Exercise 3-1

**Question:**

When using the Linux-specific `reboot()` system call to reboot the system, the second argument, *magic2*, must be specified as one of a set of magic numbers (e.g. `LINUX_REBOOT_MAGIC`). What is the significance of these numbers? (Converting them to hexadecimal provides a clue.)

**Answer:**

See `reboot.c`:
```
$ ./reboot
LINUX_REBOOT_MAGIC1 = 0xfee1dead
LINUX_REBOOT_MAGIC2 = 0x28121969
```
December 28, 1969 is Linux Torvalds' birth date!
