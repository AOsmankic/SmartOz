#include "s2lp_spi.h"

typedef enum {
    BASIC
} packet_format_t;

void set_packet_format(int fd, packet_format_t packet_format);