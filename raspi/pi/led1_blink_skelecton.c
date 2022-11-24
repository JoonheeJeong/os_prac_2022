#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include "led1_gpio_driver.h"
#include <errno.h>

void blink_led(int fd)
{
    /* here */

    while (1) {
        printf("LED ON\n");
        /* here */

        printf("LED OFF\n");
        /* here */
    }
}

int main()
{
    printf("Test GPIO\n");

    // check if the driver file exists
    if (access(DEV_FILE_NAME, F_OK) != 0) {
        int mode = (S_IRWXU|S_IRWXG|S_IFCHR);
        // make the driver file
        int ret = mknod(DEV_FILE_NAME, mode, MKDEV(DEV_GPIO_MAJOR, 0));
        if (ret < 0) {
            fprintf(stderr, "[Error:%d] Cannot make device %s(Major:%d)\n",
                    errno, DEV_FILE_NAME, DEV_GPIO_MAJOR);
            exit(1);
        }
    }

    // open the driver file
    int fd = open(DEV_FILE_NAME, O_RDWR|O_NONBLOCK);
    if (fd < 0) {
        fprintf(stderr, "[Error:%d] Cannot open GPIO Driver\n", errno);
        exit(1);
    }

    blink_led(fd);
    close(fd);
    return 0;
}
