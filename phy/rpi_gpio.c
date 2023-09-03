#include "rpi_gpio.h"

int gpio_open(char *device, int mode) {
    int fd = open(device, mode);
    if (fd < 0) {
        printf("ERROR: Failed to open GPIO. errno: %s\n", strerror(errno));
        abort();
    }
    return fd;
}

void gpio_close(int fd) {
    close(fd);
    return;
}

void gpio_set_mode(int fd, int bcm_offset, gpio_mode_t mode) {
    struct gpiohandle_request rq;

    rq.lineoffsets[0] = bcm_offset;
    rq.lines = 1;
    if (mode == OUTPUT) {
        rq.flags = GPIOHANDLE_REQUEST_OUTPUT;
    } else {
        rq.flags = GPIOHANDLE_REQUEST_INPUT;
    }

    int err = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);

    if (err < 0) {
        printf("ERROR: Failed to set GPIO bcm_offset %d mode. errno: %s\n", bcm_offset, strerror(errno));
        abort();
    }

    close(rq.fd);
}

gpio_mode_t gpio_get_mode(int fd, int bcm_offset) {
    struct gpioline_info line_info;
    gpio_get_chip_line_info(fd, &line_info, bcm_offset);
    gpio_mode_t to_return = (line_info.flags & GPIOLINE_FLAG_IS_OUT) ? OUTPUT : INPUT;
    
    return to_return;
}

void gpio_write(int fd, int bcm_offset, int val) {
    struct gpioline_info line_info;

    struct gpiohandle_request rq;
    rq.lineoffsets[0] = bcm_offset;
    rq.lines = 1;
    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    printf("Setting to output..");
    int err = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);
    printf("Set.\n");

    if (err < 0) {
        printf("ERROR: Failed to set GPIO bcm_offset %d mode. errno: %s\n", bcm_offset, strerror(errno));
        abort();
    }

    struct gpiohandle_data data;
    data.values[0] = val;
    printf("Writing..");
    err = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    printf("Wrote.\n");

    if (err < 0) {
        printf("ERROR: Failed to write value to GPIO pin with bcm_offset %d mode. errno: %s\n", bcm_offset, strerror(errno));
        abort();
    }

    close(rq.fd);
}

int gpio_read(int fd, int bcm_offset) {
    struct gpioline_info line_info;

    struct gpiohandle_request rq;
    rq.lineoffsets[0] = bcm_offset;
    rq.lines = 1;
    rq.flags = GPIOHANDLE_REQUEST_INPUT;

    int err = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);

    if (err < 0) {
        printf("ERROR: Failed to set GPIO bcm_offset %d mode. errno: %s\n", bcm_offset, strerror(errno));
        abort();
    }

    struct gpiohandle_data data;
    data.values[0] = 1;
    err = ioctl(rq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

    if (err < 0) {
        printf("ERROR: Failed to read value to GPIO pin with bcm_offset %d mode. errno: %s\n", bcm_offset, strerror(errno));
        abort();
    }
    
    close(rq.fd);

    return data.values[0];
}

void gpio_get_chip_info(int fd, struct gpiochip_info *info) {
    int err = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, info);
    if (err < 0) {
        printf("ERROR: Failed to get GPIO chip info. errno: %s", strerror(errno));
        abort();
    }
}

void gpio_get_chip_line_info(int fd, struct gpioline_info *info, int line_number) {
    info->line_offset = line_number;
    
    int err = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, info);
    if (err < 0) {
        printf("ERROR: Failed to get GPIO line info. errno: %s", strerror(errno));
        abort();
    }

        // printf("offset: %d, name: %s, consumer: %s. Flags:\t[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n", line_number, info->name, info->consumer, (info->flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT", (info->flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGH", (info->flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "...", (info->flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPEN_SOURCE" : "...", (info->flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");
}

void gpio_print_chip_line_info(int fd, struct gpioline_info *info, int line_number) {
    info->line_offset = line_number;
    
    int err = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, info);
    if (err < 0) {
        printf("ERROR: Failed to get GPIO line info. errno: %s", strerror(errno));
        abort();
    }

        printf("offset: %d, name: %s, consumer: %s. Flags:\t[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n", line_number, info->name, info->consumer, (info->flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT", (info->flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGH", (info->flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "...", (info->flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPEN_SOURCE" : "...", (info->flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");
}