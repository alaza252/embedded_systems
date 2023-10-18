/*
 * Experiment3.c
 *
 * Created: 10/16/2023 2:10:56 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "spi.h"
#include "io.h"

#define CLOCK_POLARITY_BIT (0) // should be shifted by CPOL (3)
#define CLOCK_PHASE_BIT (0) // should be shifted by CPHA (2)

//#define SPI_MSB_FIRST (0 << 5)
//#define SPI_LSB_FIRST (1 << 5)
#define SPI_MSB_FIRST (0 << DORD)
#define SPI_LSB_FIRST (1 << DORD)

/**
 * Returns the "shift amount" for the divider, which represents the number of times you need to shift 1 to get the divider.
 * (1 << returned_value) = divider
 * A value of 0b001 is a shift of 1 (multiply by 2).
 * Notice that 0b000 will never be returned from this method.
 *
 * To get the bits you need to put into SPI2X, SPR[1], SPR[0], you first need to subtract 1, then take the not of the SPI2X bit. The other 2 bits are the same
 */
uint8_t get_divider_shift_amount(uint32_t clock_rate) {
	uint8_t divider = (uint8_t) ((F_CPU / OSC_DIV + clock_rate - 1) / clock_rate); // round divider up


	uint8_t shift_amount = 0;
	// for 2 (0b10), we want a value of 1 (1 << 1 = 2)
	// for 3, 4 (0b11, 0b100), we want a value of 2 (1 << 2 = 4)
	// for 5, 6, 7, 8, (0b101, ..., 0b1000) we want a value of 3 (1 << 3 = 8)
	// When the divider is 3 (0b11), this loop should run 2 times. We want the value to be 2 in this case
	while ((1 << shift_amount) < divider) {
		shift_amount++;
	}
	return shift_amount;
}

void set_mosi(volatile SPI_t *addr, uint8_t bit_val) {
	if (addr == SPI0) {
		if (bit_val != 0) {
			gpio_output_set(SPI0_MOSI_REG, SPI0_MOSI_PIN_MASK);
		}
		else {
			gpio_output_clear(SPI0_MOSI_REG, SPI0_MOSI_PIN_MASK);
		}
	}
	else { // else assume addr is SPI1
		if (bit_val != 0) {
			gpio_output_set(SPI1_MOSI_REG, SPI1_MOSI_PIN_MASK);
		}
		else {
			gpio_output_clear(SPI1_MOSI_REG, SPI1_MOSI_PIN_MASK);
		}
	}
}

void set_sck(volatile SPI_t *addr, uint8_t bit_val) {
	if (addr == SPI0) {
		if (bit_val != 0) {
			gpio_output_set(SPI0_SCK_REG, SPI0_SCK_PIN_MASK);
		} else {
			gpio_output_clear(SPI0_SCK_REG, SPI0_SCK_PIN_MASK);
		}
	} else { // else assume addr is SPI1
		if (bit_val != 0) {
			gpio_output_set(SPI1_SCK_REG, SPI1_SCK_PIN_MASK);
		} else {
			gpio_output_clear(SPI1_SCK_REG, SPI1_SCK_PIN_MASK);
		}
	}
}

uint8_t spi_master_init(volatile SPI_t *addr, uint32_t clock_rate) {
	if (addr == SPI0) {
		gpio_output_init(SPI0_MOSI_REG, SPI0_MOSI_PIN_MASK);
		gpio_output_init(SPI0_SCK_REG, SPI0_SCK_PIN_MASK);
	} else {
		gpio_output_init(SPI1_MOSI_REG, SPI1_MOSI_PIN_MASK);
		gpio_output_init(SPI1_SCK_REG, SPI1_SCK_PIN_MASK);
	}
	set_mosi(addr, 1); // MOSI must have an initial value of 1
	set_sck(addr, CLOCK_POLARITY_BIT); // SCK must have same initial value as CPOL
	
	
	// remember for a divider value of 2 (1 << _1_), get_divider_shift_amount() returns 1
	//   For a "shift amount" of 1, we need to use a value of 0b100 for SPI2X,SPR[1],SPR[0]
	uint8_t divider_value = get_divider_shift_amount(clock_rate) - 1;
	if (divider_value & (1 << 2) == 0) { // if bit 2 of divider_value is 0, then we need to enable SPI2X to get a smaller divider
		addr->STATUS |= (1 << SPI2X);
	} else {
		addr->STATUS &= ~(1 << SPI2X);
	}
	addr->CONTROL = (1 << SPE) | SPI_LSB_FIRST | (1 << MSTR) | (CLOCK_POLARITY_BIT << CPOL) | (CLOCK_PHASE_BIT << CPHA) | (divider_value & 0b11);
}

uint8_t spi_transfer(volatile SPI_t *addr, uint8_t send_val, uint8_t *rcv_val) {
	addr -> DATA = send_val;
	
	while ((addr -> STATUS & (1<<SPIF)) == 0) {
		// do nothing
	}
	
	(*rcv_val) = addr -> DATA;
	
	return 0;
}

uint8_t spi_transmit(volatile SPI_t *addr, uint8_t send_val) {
	uint8_t unused_val;
	return spi_transfer(addr, send_val, &unused_val);
}