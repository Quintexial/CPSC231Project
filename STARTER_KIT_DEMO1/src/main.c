#include <asf.h>
#include <conf_board.h>
#include <string.h>

extern void turnOnPC16(void);
extern void turnOnPC20(void); 
extern void turnOffPC16(void);
extern void turnOffPC20(void);

#define NUNCHUCK_ADDRESS 0x52
#define LED1 IOPORT_CREATE_PIN( PIOC, 20)
#define LED2 IOPORT_CREATE_PIN( PIOA, 16)

uint8_t nunchuck_data[6];
char display_value[6];

// Initializes TWI on the Sam4S
static void twi_init(void)
{
	twi_master_options_t opt =
	{
		.speed = 100000,
		.chip = 0x52
	};
	twi_master_setup(TWI0, &opt);
}

// Returns joystick x-axis data for nunchuk
static uint8_t nunchuck_x_axis()
{
	return nunchuck_data[0];
}

// Returns x-acceleration data
static uint8_t nunchuck_x_accel()
{
	return nunchuck_data[1];
}

// Returns y-acceleration data
static uint8_t nunchuck_y_accel()
{
	return nunchuck_data[2];
}

// Returns z-acceleration data
static uint8_t nunchuck_z_accel()
{
	return nunchuck_data[3];
}

// Returns 1 if the Z button is being pressed; 0 otherwise.
static uint8_t nunchuck_buttonZ()
{
	return (~nunchuck_data[4] >> 0) & 1;
}

// Returns 1 if the C button is bring pressed; 0 otherwise.
static uint8_t nunchuck_buttonC()
{
	if(nunchuck_buttonZ() == 1)
	{
		return (~nunchuck_data[4] >> 1) & 1;
	}
	else
	{
		return (~nunchuck_data[4] >> 1) & 1;
	}
}

// Legacy function used for debugging purposes
static uint8_t decode_byte(uint8_t x)
{
	x =  (x ^ 0x17) + 0x17;
	return x;
}

// Reads in packets from the nunchuk
static void nunchuck_read(void)
{
	uint8_t data_received[6];
	
	twi_packet_t packet_read =
	{
		.addr = 0x00,
		.addr_length = sizeof (uint16_t),
		.chip = NUNCHUCK_ADDRESS,
		.buffer = &data_received,
		.length = sizeof(data_received)
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

// Sends a handshake signal to the Nunchuk to initiate communication
static void nunchuck_init(void)
{
	// Transmitting these bytes in sequence constitutes the handshake
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
	
	// Reads nunchuk data and updates display and LEDs accordingly.
	while(true)
	{
		ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
		nunchuck_read();
		ssd1306_set_page_address(0);
		ssd1306_set_column_address(0);

		sprintf(display_value, "%03d", nunchuck_x_axis());
		ssd1306_write_text("X-axis: ");
		ssd1306_write_text(display_value);
		ssd1306_set_page_address(1);
		ssd1306_set_column_address(0);

		sprintf(display_value, "%03d", nunchuck_x_accel());
		ssd1306_write_text("X-accel: ");
		ssd1306_write_text(display_value);
		ssd1306_set_page_address(2);
		ssd1306_set_column_address(0);

		sprintf(display_value, "%03d", nunchuck_y_accel());
		ssd1306_write_text("Y-accel: ");
		ssd1306_write_text(display_value);
		ssd1306_set_page_address(3);
		ssd1306_set_column_address(0);

		sprintf(display_value, "%03d", nunchuck_z_accel());
		ssd1306_write_text("Z-accel: ");
		ssd1306_write_text(display_value);

		if(nunchuck_buttonZ() == 1)
		{
			// The on/off LED functions are Assembly subroutines.
			turnOnPC16();
		}
		else
		{
			turnOffPC16();
		}

		if(nunchuck_buttonC() == 1)
		{
			turnOnPC20();
		}
		else
		{
			turnOffPC20();
		}
		delay_ms(50);
	}
}