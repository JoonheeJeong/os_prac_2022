#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include "btn1_led1_gpio_driver.h"
#include <errno.h>

#define PUSH 1
#define PULL 0

void blink_led(int fd)
{
    char read_data = 0;
    char pre_read_data = 0;
    int event_count = 0;

    while (1) {
        read(fd, &read_data, 1);
        if (pre_read_data == PULL && read_data == PUSH) {
            printf("%10d: [btn_red] PUSH\n", ++event_count);
        } else if (pre_read_data == PUSH && read_data == PULL) {
            printf("%10d: [btn_red] PULL\n", ++event_count);
        }
        pre_read_data = read_data;
        write(fd, &read_data, 1);
        usleep(100000); // delay by 0.1s to prevent duplicate inputs
    }
}

int main()
{
    printf("Read Button and Output to the RED LED\n");

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
