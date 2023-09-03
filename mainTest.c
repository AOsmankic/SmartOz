#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
// #include <wiringPiSPI.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <string.h>

#include "rpi_spi.h"
#include "s2lp_spi.h"
#include "rpi_gpio.h"
#include "s2lp_gpio.h"
#include "s2lp.h"
#include "application.h"

#include <sys/socket.h>
// #include <arpa/inet.h>
#include <sys/un.h>

#define PORT 8080
#define server_path "/tmp/phy_network_server"

static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 8000000;
static uint16_t delay = 0;


int main(int argc, char *argv[]) {

    // Open a socket for communicating between application and phy
    printf("Connecting to socket.. ");
    int fd_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd_socket < 0) {
        printf("ERROR: Failed to open application socket. errno: %s\n", strerror(errno));
        abort();
    }
    printf("Done.\n");

    // // Bind the socket to the defined port
    // int opt = 1;
    // printf("Setting socket options.. ");
    // setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    // printf("Done.\n");

    // Attach the socket to the defined port
    struct sockaddr_un server_address;
    // memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, server_path);

    unlink(server_path);
    int res = bind(fd_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    
    printf("Binding socket..\n");
    if (res < 0) {
        printf("ERROR: Failed to bind socket. errno: %s. Return: %d\n", strerror(errno), res);
        abort();
    }
    printf("Done\n");

    // Wait for at least one connection to the socket
    printf("Listening for connections.. ");
    res = listen(fd_socket, 3);

    if (res < 0) {
        printf("ERROR: Failed to listen for connections. errno: %s\n", strerror(errno));
        abort();
    }
    printf("Done.\n");

    // int client_addr;
    // struct sockaddr_in client_address;
    // socklen_t client_address_len = sizeof(client_address);

    int client_socket = accept(fd_socket, NULL, NULL);

    if (client_socket < -1) {
        printf("ERROR: Failed to accept connection from client. errno: %s", strerror(errno));
        abort();
    }


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


    // Configure rx timer to P=4, C=1
    tmp[0] = 1;
    s2lp_writereg(fd_SPI, 0x46, tmp, 1);
    tmp[0] = 4;
    s2lp_writereg(fd_SPI, 0x47, tmp, 1);
    // Turn of SQI_TIMEOUT_MASK
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

    // int fd_in = open("transmit_node", O_RDONLY);
    // if (fd_in < 0) {
    //     printf("ERROR: Failed to open fd_in. errno: %s", strerror(errno));
    //     return -1;
    // }

    // int fd_out = open("./receive_node", O_RDWR);
    // if (fd_out < 0) {
    //     printf("ERROR: Failed to open fd_out. errno: %s", strerror(errno));
    //     return -1;
    // }

    // Make a packet for rx
    s2lp_packet_t rx_packet;
    rx_packet.irq_gpio_fd = fd_GPIO;
    rx_packet.irq_gpio_num = S2LP_GPIO0;
    rx_packet.variable_length = 1;
    uint8_t rx_data[128];
    rx_packet.payload = rx_data;

    // Make a packet for tx
    s2lp_packet_t tx_packet;

    // Make a packet for application
    application_packet_t app_packet;

    fd_set set;
    struct timeval timeout;

    // Initialize the file descriptor set
    FD_ZERO(&set);
    FD_SET(client_socket, &set);

    // Initialize the timeout data structure
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (1) {
        // First, try receiving data
        s2lp_rx_packet(fd_SPI, &rx_packet);
        if (rx_packet.payload_length > 0) {
            send(client_socket, rx_packet.payload, rx_packet.payload_length, 0);
            // write(fd_out, rx_packet.payload, rx_packet.payload_length);
        }
        rx_packet.payload_length = 0;

        // Now try sending data
        // Check if there is any data to tx
        if (select(FD_SETSIZE, &set, NULL, NULL, &timeout)) {
            printf("There is data in the socket to transmit.\n");
            // Read the first 79 bytes into the application packet
            recv(client_socket, &app_packet, 79, 0);

            uint8_t data_to_tx[app_packet.num_packets];
            recv(client_socket, data_to_tx, app_packet.num_packets, 0);

            app_packet.data = data_to_tx;

            tx_packet.payload = &app_packet;
            tx_packet.payload_length = app_packet.num_packets + 79;
            printf("Sending payload.. ");
            s2lp_tx_packet(fd_SPI, &tx_packet);
            printf("Done.\n");
            
        }



        // if (select(FD_SETSIZE, &set, NULL, NULL, &timeout)) {
        //     // Data is available. Read the first 79 bytes into the application packet
        //     printf("There is data to transmit. To do.\n");
        //     read(fd_in, &app_packet, 16);
        //     printf("First 16 bytes: %s\n", app_packet.source);
        // }

        
    }

    return 0;
}