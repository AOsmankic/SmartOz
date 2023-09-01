#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi_spi.h"

#define S2LP_HEADER_READREG 0x01
#define S2LP_HEADER_WRITEREG 0x00
#define S2LP_HEADER_CMD 0x80

#define S2LP_FIFO_ADDR 0xFF
#define S2LP_GPIO0_CONF_ADDR 0x00

#define S2LP_TX_FIFO_STATUS_REG 0x8F
#define S2LP_RX_FIFO_STATUS_REG 0x90

#define S2LP_CMD_TX 0x60
#define S2LP_CMD_RX 0x61
#define S2LP_CMD_FLUSHTXFIFO 0x72
#define S2LP_CMD_FLUSHRXFIFO 0x71

uint8_t s2lp_readreg(int fd, uint8_t addr);
void s2lp_writereg(int fd, uint8_t addr, uint8_t data[], uint8_t nBytes);
void S2LPSpiWriteRegisters(int fd, uint8_t address, uint8_t n_regs, uint8_t* buffer);
void s2lp_command(int fd, uint8_t cmd);
uint8_t s2lp_txfifo_count(int fd);
uint8_t s2lp_rxfifo_count(int fd);
void s2lp_writefifo(int fd, uint8_t data[], uint8_t nBytes);
uint8_t s2lp_readfifo(int fd);