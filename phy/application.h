typedef struct {
    uint8_t source[16];
    uint8_t dest[16];
    uint8_t type;
    uint8_t id[2];
    uint8_t packet_num[2];
    uint16_t num_packets;
    uint8_t checksum[40];
    uint8_t *data;
} application_packet_t;