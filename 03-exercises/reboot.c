/*
 *  The Linux Programming Interface
 *  Exercise 3-1
 */

#include <linux/reboot.h>

#include <tlpi.h>

int main(int argc, char** argv) {
    printf("LINUX_REBOOT_MAGIC1 = 0x%x\n", LINUX_REBOOT_MAGIC1);
    printf("LINUX_REBOOT_MAGIC2 = 0x%x\n", LINUX_REBOOT_MAGIC2);

    exit(EXIT_SUCCESS);
}
