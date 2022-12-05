#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include "btnN_ledN_gpio_driver.h"
#include <errno.h>

#define PUSH 1
#define PULL 0

void blink_led(int fd)
{
    char read_data = 0;
    char input_r = 0;
    char input_g = 0;
    char input_w = 0;
    char input_y = 0;
    char pre_read_data = 0;
    char pre_input_r = 0;
    char pre_input_g = 0;
    char pre_input_w = 0;
    char pre_input_y = 0;
    unsigned long event_count = 0;

    while (1) {
        read(fd, &read_data, 1);
        input_r = read_data & 1;
        input_g = (read_data >> 1) & 1;
        input_w = (read_data >> 2) & 1;
        input_y = (read_data >> 3) & 1;
        if (pre_input_r == PULL && input_r == PUSH) {
            printf("%10d: [btn_red] PUSH\n", ++event_count);
        } else if (pre_input_r == PUSH && input_r == PULL) {
            printf("%10d: [btn_red] PULL\n", ++event_count);
        }
        if (pre_input_g == PULL && input_g == PUSH) {
            printf("%10d: [btn_blue] PUSH\n", ++event_count);
        } else if (pre_input_g == PUSH && input_g == PULL) {
            printf("%10d: [btn_blue] PULL\n", ++event_count);
        }
        if (pre_input_w == PULL && input_w == PUSH) {
            printf("%10d: [btn_white] PUSH\n", ++event_count);
        } else if (pre_input_w == PUSH && input_w == PULL) {
            printf("%10d: [btn_white] PULL\n", ++event_count);
        }
        if (pre_input_y == PULL && input_y == PUSH) {
            printf("%10d: [btn_x] PUSH\n", ++event_count);
        } else if (pre_input_y == PUSH && input_y == PULL) {
            printf("%10d: [btn_x] PULL\n", ++event_count);
        }
        pre_read_data = read_data;
        pre_input_r = input_r;
        pre_input_g = input_g;
        pre_input_w = input_w;
        pre_input_y = input_y;
        write(fd, &read_data, 1);
        usleep(100000); // delay by 0.1s to prevent duplicate inputs
    }
}

int main()
{
    printf("Read buttons and Output to the LEDs\n");

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
        fprintf(stderr, "[Error:%d]: Cannot open GPIO Driver\n", errno);
        exit(1);
    }

    blink_led(fd);
    close(fd);
    return 0;
}
