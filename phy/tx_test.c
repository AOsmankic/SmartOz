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
#include "s2lp.h"

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

 tmp[0] = 0x33; /* reg. SYNT2 (0x06) */
  tmp[1] = 0x13; /* reg. SYNT1 (0x07) */
  tmp[2] = 0xB1; /* reg. SYNT0 (0x08) */
  tmp[3] = 0x29; /* reg. IF_OFFSET_ANA (0x09) */
  tmp[4] = 0xB7; /* reg. IF_OFFSET_DIG (0x0A) */
  S2LPSpiWriteRegisters(fd_SPI, 0x06, 5, tmp);
  tmp[0] = 0x27; /* reg. MOD2 (0x10) */
  S2LPSpiWriteRegisters(fd_SPI, 0x10, 1, tmp);
  tmp[0] = 0x55; /* reg. ANT_SELECT_CONF (0x1F) */
  tmp[1] = 0x00; /* reg. CLOCKREC2 (0x20) */
  S2LPSpiWriteRegisters(fd_SPI, 0x1F, 2, tmp);
  tmp[0] = 0x00; /* reg. PCKTCTRL3 (0x2E) */
  tmp[1] = 0x01; /* reg. PCKTCTRL2 (0x2F) */
  tmp[2] = 0x30; /* reg. PCKTCTRL1 (0x30) */
  S2LPSpiWriteRegisters(fd_SPI, 0x2E, 3, tmp);
  tmp[0] = 0x01; /* reg. PROTOCOL1 (0x3A) */
  S2LPSpiWriteRegisters(fd_SPI, 0x3A, 1, tmp);
  tmp[0] = 0x41; /* reg. PCKT_FLT_OPTIONS (0x40) */
  S2LPSpiWriteRegisters(fd_SPI, 0x40, 1, tmp);
  tmp[0] = 0x4C; /* reg. CSMA_CONFIG3 (0x4C) */
  S2LPSpiWriteRegisters(fd_SPI, 0x4C, 1, tmp);
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
    // s2lp_command(fd_SPI, S2LP_CMD_FLUSHTXFIFO);
    usleep(5*1000*1000);

    // while (iter < 500) {
    //     while (s2lp_txfifo_count(fd_SPI) != 0);
    //     s2lp_writefifo(fd_SPI, fifodata, fifodatalength);
    //     printf("Pre-TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
    //     s2lp_command(fd_SPI, S2LP_CMD_TX);
    //     usleep(300*1000);
    //     printf("Post-TX FIFO Size: %d\n", s2lp_txfifo_count(fd_SPI));
    //     printf("Iteration: %d\n", iter++);
    // }

    int testCnt = 20;

    
    for (int i = 1; i <= testCnt; i++) {
        s2lp_command(fd_SPI, S2LP_CMD_FLUSHTXFIFO);

        int size = snprintf(NULL, 0, "Test packet %d of %d", i, testCnt);
        char *msg = malloc(size + 1);
        sprintf(msg, "Test packet %d of %d", i, testCnt);
        
        s2lp_packet_t tx_packet;
        tx_packet.payload = msg;
        tx_packet.payload_length = size +1;

        s2lp_tx_packet(fd_SPI, &tx_packet);

        printf("Sending payload: \"%s\"\n", msg);
        printf("The payload size is: %d. snprintf said %d\n", tx_packet.payload_length, size);

        usleep(1*1000*1000);
    }


    return 0;
}