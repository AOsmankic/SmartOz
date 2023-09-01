#include "s2lp_spi.h"

uint8_t s2lp_readreg(int fd, uint8_t addr) {
    uint8_t spiBufTx[3] = {S2LP_HEADER_READREG, addr, 0xFF};
    uint8_t spiBufRx[3];

    spi_transfer(fd, spiBufTx, spiBufRx, 3);
    return spiBufRx[2];
}

void s2lp_writereg(int fd, uint8_t addr, uint8_t data[], uint8_t nBytes) {
    uint8_t spiBufTx[2 + nBytes];
    uint8_t spiBufRx[2 + nBytes];

    spiBufTx[0] = S2LP_HEADER_WRITEREG;
    spiBufTx[1] = addr;

    // Copy the contents of data[] into the rest of spiBufTx[]
    memcpy(spiBufTx + 2, data, nBytes * sizeof(data[0]));

    spi_transfer(fd, spiBufTx, spiBufRx, 2 + nBytes);
    return;
}

// This is a wrapper function to help when loading data from the S2-LP GUI. When we start doing stuff ourselves, without importing the configs, this can be removed. That'll probably be a while, though.
void S2LPSpiWriteRegisters(int fd, uint8_t address, uint8_t n_regs, uint8_t* buffer) {
    s2lp_writereg(fd, address, buffer, n_regs);
    return;
}

void s2lp_command(int fd, uint8_t cmd) {
    uint8_t spiBufTx[2] = {S2LP_HEADER_CMD, cmd};
    uint8_t spiBufRx[2] = {};

    spi_transfer(fd, spiBufTx, spiBufRx, 2);
    return;
}

void s2lp_writefifo(int fd, uint8_t data[], uint8_t nBytes) {
    s2lp_writereg(fd, S2LP_FIFO_ADDR, data, nBytes);
    return;
}

uint8_t s2lp_readfifo(int fd) {
    s2lp_readreg(fd, S2LP_FIFO_ADDR);
}

uint8_t s2lp_txfifo_count(int fd) {
    return s2lp_readreg(fd, S2LP_TX_FIFO_STATUS_REG);
}

uint8_t s2lp_rxfifo_count(int fd) {
    return s2lp_readreg(fd, S2LP_RX_FIFO_STATUS_REG);
}