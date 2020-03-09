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

#define NUNCHUCK_ADDRESS 0x52
#define LED0 IOPORT_CREATE_PIN( PIOC, 23)

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

static uint8_t nunchuck_buttonZ() 
{
	return (~nunchuck_data[5] >> 0) & 1;
}
static void nunchuck_read(void)
{
	uint8_t data_received[6];
	
	twi_package_t packet_read =
	{
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = &data_received,
		.length = sizeof(nunchuck_data)
	};
	while(twi_master_read(TWI0, &packet_read) != TWI_SUCCESS);
	uint8_t i;
	for (i=0; i<6; i++)
	{
		nunchuck_data[i] = data_received[i];
	}
	const uint8_t emptyByte[] = {0x00};
	twi_package_t packet_write =
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
	const uint8_t handshake1[] = {0xF0, 0x55};
	const uint8_t handshake2[] = {0xFB, 0x00};
	twi_package_t packet_write1 =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)handshake1,
		.length = sizeof(handshake1)
	};
	twi_package_t packet_write2 =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)handshake2,
		.length = sizeof(handshake2)
	};
	twi_master_write(TWI0, &packet_write1);
	twi_master_write(TWI0, &packet_write2);
	
}

int main(void)
{
	sysclk_init();
	board_init();
	
	twi_init();
	nunchuck_init();
	ioport_set_pin_dir( LED0, IOPORT_DIR_OUTPUT );
	while(true)
	{
		nunchuck_read();
		if(nunchuck_buttonZ == 1)
		{
			ioport_set_pin_level( LED0, false);
		}
		else
		{
			ioport_set_pin_level( LED0, true);
		}
	}
}