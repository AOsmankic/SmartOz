#include "s2lp.h"
#include "rpi_gpio.h"
#include "s2lp_irq.h"

// void set_packet_format(int fd, packet_format_t packet_format) {
//     if (packet_format == BASIC) {

//     }
// }

void s2lp_tx_packet(int fd, s2lp_packet_t *packet) {
    // Set the tx packet length
    uint8_t length_msb = (packet->payload_length >> 8);
    uint8_t length_lsb = (packet->payload_length & 0x00FF);
    s2lp_writereg(fd, S2LP_PCKT_LEN_MSB, &length_msb, 1);
    s2lp_writereg(fd, S2LP_PCKT_LEN_LSB, &length_lsb, 1);

    // Write data to the FIFO
    s2lp_writefifo(fd, packet->payload, packet->payload_length);

    // Enter TX mode
    s2lp_command(fd, S2LP_CMD_TX);
}

void s2lp_rx_packet(int fd, s2lp_packet_t *packet) {
    // If the packet should be variable length, set the S2LP accordingly.
    uint8_t pckt_ctrl = s2lp_readreg(fd, S2LP_PCKT_CTRL_2_REG);
    pckt_ctrl = pckt_ctrl | (packet->variable_length & 0x01);
    printf("Is the packet variable length? %d\n", s2lp_readreg(fd, S2LP_PCKT_CTRL_2_REG) & 0x01);
    s2lp_writereg(fd, S2LP_PCKT_CTRL_2_REG, &pckt_ctrl, 1);

    // // If fixed length, set the desired payload length
    // if (packet.variable_length) {
    //     s2lp_writereg(fd, S2LP_PCKT_LEN_MSB, &(packet.payload_length))
    // }

    // Place the transceiver into receive mode
   s2lp_command(fd, S2LP_CMD_RX);

//    //  If an interrupt gpio pin has been defined, poll it. Else, poll the IRQ over SPI
//    if (packet.irq_gpio_fd != -1 && packet.irq_gpio_num != -1) {
//     while (1) {
//         // Poll the gpio pin. If it's zero, an interrupt has been thrown. Check if IRQ_STATUS0 indicates a packet has been received
//         if (gpio_read(packet.fd_GPIO, packet.irq_gpio_num) == 0) {
//             if (s2lp_get_irq(fd, S2LP_IRQ_STATUS_0, S2LP_IRQ_0_MASK_RX_DATA_READY)) {

//             }
//         }
//     }
//     while ()
//    } else {

//    }
    // Poll the IRQ_STATUS_0 register to see if a message has been received
    uint8_t received_packet = 0;
    while (!received_packet) {
        if (s2lp_get_irq(fd, S2LP_IRQ_STATUS_0, S2LP_IRQ_0_MASK_RX_DATA_READY)) {
            // A packet was received. Stop polling.
            received_packet = 1;

            // Read the packet using the packet length registers to determine the number of bytes.
            uint16_t packet_length = 0;
            packet_length |= s2lp_readreg(fd, S2LP_PCKT_RX_LEN_MSB) << 8;
            packet_length |= s2lp_readreg(fd, S2LP_PCKT_RX_LEN_LSB);

            // uint16_t packet_length = 0;
            // packet_length |= s2lp_readreg(fd, 0xA4) << 8;
            // packet_length |= s2lp_readreg(fd, 0xA5); 
            packet->payload_length = packet_length;

            // Read the FIFO
            for (int i = 0; i < packet_length; i++) {
                packet->payload[i] = s2lp_readfifo(fd);
            }

            // Get the RSSI in dBm
            uint8_t rssi_raw = s2lp_readreg(fd, S2LP_RSSI_LEVEL_REG);
            packet->rssi_dbm = rssi_raw - 146;
        }
        
    }
}