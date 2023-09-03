#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>


#define SPI01 "/dev/spidev0.1"
#define SPI00 "/dev/spidev0.0"

int spi_open(char *device, int mode);
void spi_transfer(int fd, uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t nBytes);
uint32_t spi_get_speed(int fd);
void spi_set_speed(int fd, uint32_t speed);
uint8_t spi_get_bits(int fd);
void spi_set_bits(int fd, uint8_t bits);
uint8_t spi_get_mode(int fd);
void spi_set_mode(int fd, uint8_t mode);