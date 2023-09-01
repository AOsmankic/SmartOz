#include "rpi_spi.h"





int spi_open(char *device, int mode) {
    int fd = open(device, mode);
    if (fd < 0) {
        printf("ERROR: Failed to open SPI bus. errno: %s", strerror(errno));
        abort();
    }
    return fd;
}

void spi_close(int fd) {
    close(fd);
    return;
}

void spi_transfer(int fd, uint8_t tx_buffer[], uint8_t rx_buffer[], uint8_t nBytes) {
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long) tx_buffer,
        .rx_buf = (unsigned long) rx_buffer,
        .len = nBytes,
        .delay_usecs = 0,
        .speed_hz = spi_get_speed(fd),
        .bits_per_word = spi_get_bits(fd),
    };

    int err = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

    if (err < 0) {
        printf("ERROR: Failed to transfer SPI bus data. errno; %s", strerror(errno));
        abort();
    }

    return;
}

uint32_t spi_get_speed(int fd) {
    uint32_t speed;
    int err = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (err < 0) {
        printf("ERROR: Failed to get SPI bus speed. errno: %s", strerror(errno));
        abort();
    }
    return speed;
}

void spi_set_speed(int fd, uint32_t speed) {
    int err = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (err < 0) {
        printf("ERROR: Failed to set SPI bus speed. errno: %s", strerror(errno));
        abort();
    }
    return;
}

uint8_t spi_get_bits(int fd) {
    uint8_t bits;
    int err = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (err < 0) {
        printf("ERROR: Failed to get SPI bus bits per word. errno: %s", strerror(errno));
        abort();
    }
    return bits;
}

void spi_set_bits(int fd, uint8_t bits) {
    int err = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (err < 0) {
        printf("ERROR: Failed to set SPI bus bits per word. errno: %s", strerror(errno));
        abort();
    }
    return;
}

uint8_t spi_get_mode(int fd) {
    uint8_t mode;
    int err = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (err < 0) {
        printf("ERROR: Failed to get SPI bus mode. errno: %s", strerror(errno));
        abort();
    }
    return mode;
}

void spi_set_mode(int fd, uint8_t mode) {
    int err = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (err < 0) {
        printf("ERROR: Failed to set SPI bus mode. errno: %s", strerror(errno));
        abort();
    }
    return;
}