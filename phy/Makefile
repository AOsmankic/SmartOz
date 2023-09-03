target_files := rpi_spi.c rpi_spi.h s2lp_spi.c s2lp_spi.h tx_test.h rpi_gpio.h rpi_gpio.c s2lp_gpio.h s2lp.h s2lp.c s2lp_irq.h s2lp_irq.c mainTest.h

all: txTest rxTest mainTest

rxTest: rx_test.c $(target_files)
	$(CC) $(CFLAGS) $(LDFLAGS) -lm -o $@ $^

txTest: tx_test.c $(target_files)
	$(CC) $(CFLAGS) $(LDFLAGS) -lm -o $@ $^

mainTest: mainTest.c $(target_files)
	$(CC) $(CFLAGS) $(LDFLAGS) -lm -o $@ $^

