#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include "tx_test.h"
// #include <wiringPiSPI.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>
// #include <string.h>

#include "rpi_spi.h"
#include "s2lp_spi.h"


int main(int argc, char *argv[]) {

    printf("Opening and configuring SPI bus.. ");
	// int fd_SPI = spi_open(SPI00, O_RDWR);
    // int err;

    return 1;
}