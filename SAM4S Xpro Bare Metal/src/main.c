/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <conf_board.h>
#include <string.h>

#define NUNCHUCK_ADDRESS 0x52
//#define LED0 IOPORT_CREATE_PIN( PIOC, 23)

uint8_t nunchuck_data[6];
static void twi_init(void)
{
	twi_master_options_t opt =
	{
		.speed = 100000,
		.chip = 0x52
	};
	twi_master_setup(TWI0, &opt);
}
static uint8_t nunchuck_x_axis() 
{
	return nunchuck_data[0];
}
static uint8_t decode_byte(uint8_t x)
{
	x =  (x ^ 0x17) + 0x17;
	return x;
}
static void nunchuck_read(void)
{
	uint8_t data_received[6];
	
	twi_packet_t packet_read =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = &data_received,
		.length = 6
	};
	while(twi_master_read(TWI0, &packet_read) != TWI_SUCCESS);
	uint8_t i;
	for (i=0; i<6; i++)
	{
		nunchuck_data[i] = data_received[i];
	}
	const uint8_t emptyByte[] = {0x00};
	twi_packet_t packet_write =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)emptyByte,
		.length = sizeof(emptyByte),
	};
	twi_master_write(TWI0, &packet_write);
}
static void nunchuck_init(void)
{
	const uint8_t handshake1[] = {0x40, 0x00};
	const uint8_t handshake2[] = {0x00};
	twi_packet_t packet_write1 =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)handshake1,
		.length = sizeof(handshake1)
	};
	twi_packet_t packet_write2 =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)handshake2,
		.length = sizeof(handshake2)
	};
	while(twi_master_write(TWI0, &packet_write1) != TWI_SUCCESS);
	while(twi_master_write(TWI0, &packet_write2) != TWI_SUCCESS);
	
}

int main(void)
{
	sysclk_init();
	board_init();
	ssd1306_init();
	ssd1306_clear();
	
	twi_init();
	nunchuck_init();
	ioport_set_pin_dir( LED0, IOPORT_DIR_OUTPUT );
	while(true)
	{
		ssd1306_clear();
		ssd1306_set_column_address(95);
		ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
		nunchuck_read();
		ssd1306_write_text("X-axis: ");
		ssd1306_write_data(nunchuck_x_axis());
	}
}