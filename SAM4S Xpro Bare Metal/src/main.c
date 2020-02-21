/*
 * CFile1.c
 *
 * Created: 2/20/2020 1:09:27 PM
 *  Author: kjohnson4
 */ 

#include <asf.h>

#define NUNCHUCK_ADDRESS 0x52

uint8_t nunchuck_data[6];
static void twi_init(void)
{
	twi_master_options_t opt = 
	{
		.speed = 100000,
		.chip = 0x52
	};
	twi_master_setup(&TWI0, &opt);
}


static void nunchuck_read(void)
{
	uint8_t data_received[6];
	
	twi_packet_t packet_read =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = data_received,
		.length = 6
	}
	if(twi_master_read(&TWI0, &packet_read) == TWI_SUCCESS)
	{
		uint8_t i;
		for (i=0; i<6; i++)
		{
			nunchuck_data[i] = data_received[i];
		}
	}
	const uint8_t emptyByte[] = {0x00};
	twi_packet_t packet_write = 
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = (void *)emptyByte,
		.length = 1,
	}
	twi_master_write(&TWI0, packet_write);
}
static void nunchuck_init(void)
{
	const uint8_t handshake1[] = {0xF0, 0x55};
	const uint8_t handshake2[] = {0xFB, 0x00};
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
	twi_master_write(&TWI0, packet_write1);
	twi_master_write(&TWI0, packet_write2);
	
}

int main(void)
{
	sysclk_init();
	board_init();
	
	twi_init();
	nunchuck_init();
}