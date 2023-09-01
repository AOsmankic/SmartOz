#include <linux/gpio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <errno.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define GPIO0 "/dev/gpiochip0"

typedef enum {
    OUTPUT,
    INPUT
} gpio_mode_t;

int gpio_open(char *device, int mode);
void gpio_close(int fd);

void gpio_set_mode(int fd, int bcm_offset, gpio_mode_t mode);
gpio_mode_t gpio_get_mode(int fd, int bcm_offset);
void gpio_write(int fd, int bcm_offset, int val);
int gpio_read(int fd, int bcm_offset);

void gpio_get_chip_info(int fd, struct gpiochip_info *info);
void gpio_get_chip_line_info(int fd, struct gpioline_info *info, int line_number);

void gpio_print_chip_line_info(int fd, struct gpioline_info *info, int line_number);