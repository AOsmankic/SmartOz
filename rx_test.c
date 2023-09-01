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
#include "rpi_gpio.h"
#include "s2lp_gpio.h"
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

    printf("Opening GPIO bus..");
    int fd_GPIO = gpio_open(GPIO0, O_RDONLY);
    printf("Done.\n");

    uint32_t iter = 0;

    // // BEGIN IMPORTED
    // uint8_t tmp[5];

    // tmp[0] = 0x46; /* reg. SYNT2 (0x06) */
    // tmp[1] = 0x66; /* reg. SYNT1 (0x07) */
    // tmp[2] = 0x66; /* reg. SYNT0 (0x08) */
    // tmp[3] = 0x2F; /* reg. IF_OFFSET_ANA (0x09) */
    // tmp[4] = 0xC2; /* reg. IF_OFFSET_DIG (0x0A) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x06, 5, tmp);
    // tmp[0] = 0x92; /* reg. MOD4 (0x0E) */
    // tmp[1] = 0xA5; /* reg. MOD3 (0x0F) */
    // tmp[2] = 0x27; /* reg. MOD2 (0x10) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x0E, 3, tmp);
    // tmp[0] = 0xA3; /* reg. MOD0 (0x12) */
    // tmp[1] = 0x13; /* reg. CHFLT (0x13) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x12, 2, tmp);
    // tmp[0] = 0x55; /* reg. ANT_SELECT_CONF (0x1F) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x1F, 1, tmp);
    // tmp[0] = 0x00; /* reg. PCKTCTRL3 (0x2E) */
    // tmp[1] = 0x01; /* reg. PCKTCTRL2 (0x2F) */
    // tmp[2] = 0x30; /* reg. PCKTCTRL1 (0x30) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x2E, 3, tmp);
    // tmp[0] = 0x01; /* reg. PROTOCOL1 (0x3A) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x3A, 1, tmp);
    // tmp[0] = 0x40; /* reg. FIFO_CONFIG3 (0x3C) */
    // tmp[1] = 0x40; /* reg. FIFO_CONFIG2 (0x3D) */
    // tmp[2] = 0x40; /* reg. FIFO_CONFIG1 (0x3E) */
    // tmp[3] = 0x40; /* reg. FIFO_CONFIG0 (0x3F) */
    // tmp[4] = 0x41; /* reg. PCKT_FLT_OPTIONS (0x40) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x3C, 5, tmp);
    // tmp[0] = 0x18; /* reg. PA_POWER8 (0x5A) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x5A, 1, tmp);
    // tmp[0] = 0x07; /* reg. PA_POWER0 (0x62) */
    // tmp[1] = 0x01; /* reg. PA_CONFIG1 (0x63) */
    // S2LPSpiWriteRegisters(fd_SPI, 0x62, 2, tmp);
    //     // END IMPORTED

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

// Configure for var length transmissions
// s2lp_writereg(fd_SPI, 0x2F, , 1);
// tmp[0] = 24;
// s2lp_writereg(fd_SPI, 0x32, tmp, 1);
// tmp[0] = 0;
//     s2lp_writereg(fd_SPI, 0x31, tmp, 1);

  // Configure GPIO0 to be nIRQ output
    uint8_t data[] = {(0x00 << 3) | 0b10};
    printf("Writing %d to GPIO0CONF.\n", data[0]);
  s2lp_writereg(fd_SPI, S2LP_GPIO0_CONF_ADDR, data, 1);

  // Configure IRQ_MASK0 for data ready interrupts and RX under/overflows
    uint8_t data2[] = {0x41};
    printf("Writing to IRQMASK0.\n");
    s2lp_writereg(fd_SPI, 0x53, data2, 1);
    
    
    printf("RX FIFO Size Pre-Flush: %d\n", s2lp_rxfifo_count(fd_SPI));
    s2lp_command(fd_SPI, S2LP_CMD_FLUSHRXFIFO);
    usleep(100*1000);
    printf("RX FIFO Size Post-Flush: %d\n", s2lp_rxfifo_count(fd_SPI));
    printf("Enabling Rx in 1 second!\n");
    usleep(1*1000*1000);

    s2lp_command(fd_SPI, S2LP_CMD_RX);

    int firstRun = 1;
    while (1) {
        // printf("Current device mode: %d\n", s2lp_readreg(fd_SPI, 0x8E) >> 1);

        // Make a packet
        s2lp_packet_t rx_packet;
        rx_packet.irq_gpio_fd = fd_GPIO;
        rx_packet.irq_gpio_num = S2LP_GPIO0;
        rx_packet.variable_length = 1;
        uint8_t data[128];
        rx_packet.payload = data;

        // Receive packet
        s2lp_rx_packet(fd_SPI, &rx_packet);

        // Print the RSSI and payload.
        printf("Packet Length: %d\n", rx_packet.payload_length);
        printf("RSSI: %d dBm\n", rx_packet.rssi_dbm);
        printf("Payload: %s\n", rx_packet.payload);

        // if (gpio_read(fd_GPIO, S2LP_GPIO0) == 0) {
        //     // if (s2lp_readreg(fd_SPI, 0xFD) == 1) {
        //     // An interrupt was thrown. Print the value of IRQ_STATUS0.
        //     printf("IRQ_STATUS0 = 0d%d\n", s2lp_readreg(fd_SPI, 0xFD));
        //     printf("The RX FIFO Size is = %d\n", s2lp_rxfifo_count(fd_SPI));

        //     uint16_t packet_length = 0;
        //     packet_length |= s2lp_readreg(fd_SPI, 0xA4) << 8;
        //     packet_length |= s2lp_readreg(fd_SPI, 0xA5); 
        //     printf("The RX message length is = %d\n", packet_length);
        //     char rxMsg[s2lp_rxfifo_count(fd_SPI)];
        //     for (int i = 0; i < packet_length; i++) {
        //         rxMsg[i] = s2lp_readfifo(fd_SPI);
        //     }
        //     printf("RSSI: %d dBm\n", s2lp_readreg(fd_SPI, 0xA2) - 146);
        //     printf("The data in the RX FIFO is: \"%s\"\n", rxMsg);
        //     printf("IRQ_STATUS0 = 0d%d\n", s2lp_readreg(fd_SPI, 0xFD));
        //     s2lp_command(fd_SPI, S2LP_CMD_RX);
        // }
        
    }


    return 0;
}