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

static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 8000000;
static uint16_t delay = 0;


int main(int argc, char *argv[]) {
    printf("Opening and configuring SPI bus.. ");
	int fd_SPI = spi_open(SPI00, O_RDWR);
    int err;
    
	spi_set_mode(fd_SPI, mode);
	spi_set_bits(fd_SPI, bits);
	spi_set_speed(fd_SPI, speed);

	printf("Done.\n");

    uint32_t iter = 0;

    // BEGIN IMPORTED
    uint8_t tmp[5];

    tmp[0] = 0x46; /* reg. SYNT2 (0x06) */
    tmp[1] = 0x66; /* reg. SYNT1 (0x07) */
    tmp[2] = 0x66; /* reg. SYNT0 (0x08) */
    tmp[3] = 0x2F; /* reg. IF_OFFSET_ANA (0x09) */
    tmp[4] = 0xC2; /* reg. IF_OFFSET_DIG (0x0A) */
    S2LPSpiWriteRegisters(fd_SPI, 0x06, 5, tmp);
    tmp[0] = 0x92; /* reg. MOD4 (0x0E) */
    tmp[1] = 0xA5; /* reg. MOD3 (0x0F) */
    tmp[2] = 0x27; /* reg. MOD2 (0x10) */
    S2LPSpiWriteRegisters(fd_SPI, 0x0E, 3, tmp);
    tmp[0] = 0xA3; /* reg. MOD0 (0x12) */
    tmp[1] = 0x13; /* reg. CHFLT (0x13) */
    S2LPSpiWriteRegisters(fd_SPI, 0x12, 2, tmp);
    tmp[0] = 0x55; /* reg. ANT_SELECT_CONF (0x1F) */
    S2LPSpiWriteRegisters(fd_SPI, 0x1F, 1, tmp);
    tmp[0] = 0x00; /* reg. PCKTCTRL3 (0x2E) */
    tmp[1] = 0x01; /* reg. PCKTCTRL2 (0x2F) */
    tmp[2] = 0x30; /* reg. PCKTCTRL1 (0x30) */
    S2LPSpiWriteRegisters(fd_SPI, 0x2E, 3, tmp);
    tmp[0] = 0x01; /* reg. PROTOCOL1 (0x3A) */
    S2LPSpiWriteRegisters(fd_SPI, 0x3A, 1, tmp);
    tmp[0] = 0x40; /* reg. FIFO_CONFIG3 (0x3C) */
    tmp[1] = 0x40; /* reg. FIFO_CONFIG2 (0x3D) */
    tmp[2] = 0x40; /* reg. FIFO_CONFIG1 (0x3E) */
    tmp[3] = 0x40; /* reg. FIFO_CONFIG0 (0x3F) */
    tmp[4] = 0x41; /* reg. PCKT_FLT_OPTIONS (0x40) */
    S2LPSpiWriteRegisters(fd_SPI, 0x3C, 5, tmp);
    tmp[0] = 0x18; /* reg. PA_POWER8 (0x5A) */
    S2LPSpiWriteRegisters(fd_SPI, 0x5A, 1, tmp);
    tmp[0] = 0x07; /* reg. PA_POWER0 (0x62) */
    tmp[1] = 0x01; /* reg. PA_CONFIG1 (0x63) */
    S2LPSpiWriteRegisters(fd_SPI, 0x62, 2, tmp);
        // END IMPORTED

    uint8_t spiBufTx[7] = {0x01, 0x30, 0xFF, 0xFF, 0xBB, 0xCC, 0xAA};
    uint8_t spiBufRx[7];
    uint8_t addr = 0x8E;
    printf("TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
    printf("Added 0xAA to FIFO.\n");

    uint8_t fifodata[] = "Hello from my Pi!";
    uint8_t fifodatalength = sizeof(fifodata) / sizeof(fifodata[0]);
    // s2lp_writefifo(fd_SPI, fifodata, fifodatalength);
    // printf("TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
    // s2lp_command(fd_SPI, S2LP_CMD_TX);
    usleep(100*1000);
    printf("TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
    s2lp_command(fd_SPI, S2LP_CMD_FLUSHTXFIFO);
    usleep(5*1000*1000);

    while (iter < 500) {
        while (s2lp_txfifo_count(fd_SPI) != 0);
        s2lp_writefifo(fd_SPI, fifodata, fifodatalength);
        printf("Pre-TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
        s2lp_command(fd_SPI, S2LP_CMD_TX);
        usleep(300*1000);
        printf("Post-TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
        printf("Iteration: %d\n", iter++);
    }


    return 0;
}