#include <stdint.h>

#define S2LP_IRQ_STATUS_0 0xFD
#define S2LP_IRQ_STATUS_1 0xFC
#define S2LP_IRQ_STATUS_2 0xFB
#define S2LP_IRQ_STATUS_3 0xFA

#define S2LP_IRQ_0_MASK_RX_DATA_READY 0x01

uint8_t s2lp_get_irq(int fd, uint8_t irq_addr, uint8_t mask);