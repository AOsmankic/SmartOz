#include "s2lp_spi.h"

typedef enum {
    BASIC
} packet_format_t;

// struct Packet_Basic_Var_Length {
//     uint8_t preamble_sel = 0,
//     uint8_t length = 

// }

typedef struct S2LP_PACKET {
    int8_t rssi_dbm;
    uint8_t *payload;
    uint8_t variable_length;
    uint16_t payload_length;
    int irq_gpio_fd;
    int irq_gpio_num;
} s2lp_packet_t;

// void set_packet_format(int fd, packet_format_t packet_format);

void s2lp_rx_packet(int fd, s2lp_packet_t *packet);

void s2lp_tx_packet(int fd, s2lp_packet_t *packet);