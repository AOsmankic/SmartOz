#include "s2lp_irq.h"
#include "s2lp_spi.h"

uint8_t s2lp_get_irq(int fd, uint8_t irq_addr, uint8_t mask) {
    return (s2lp_readreg(fd, irq_addr) & mask);
}