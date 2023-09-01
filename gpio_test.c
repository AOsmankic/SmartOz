#include "rpi_gpio.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Opening GPIO bus..");
    int fd_GPIO = gpio_open(GPIO0, O_RDONLY);

    // Query
    struct gpiochip_info info;
    struct gpioline_info line_info;
    gpio_get_chip_info(fd_GPIO, &info);



    printf("Chip name: %s\n", info.name);

    printf("Chip label: %s\n", info.label);

    printf("Number of lines: %d\n", info.lines);
    for (int i = 0; i < info.lines; i++) {
        gpio_print_chip_line_info(fd_GPIO, &line_info, i);
    }

    int target_gpio = 16;
    char *gpio_17_mode = (gpio_get_mode(fd_GPIO, target_gpio) == OUTPUT) ? "OUTPUT" : "INPUT";
    printf("GPIO17 Mode: %s\n", gpio_17_mode);
    gpio_set_mode(fd_GPIO, target_gpio, OUTPUT);
    gpio_17_mode = (gpio_get_mode(fd_GPIO, target_gpio) == OUTPUT) ? "OUTPUT" : "INPUT";
    printf("GPIO17 Mode: %s\n", gpio_17_mode);
    gpio_write(fd_GPIO, target_gpio, 1);
    while (1) {
        gpio_write(fd_GPIO, target_gpio, 0);
        usleep(100*1000);
        gpio_write(fd_GPIO, target_gpio, 1);
        usleep(100*1000);
    }
    // for (int i = 0; i < info.lines; i++) {
    //     gpio_get_chip_line_info(fd_GPIO, &line_info, i);
    // }
}