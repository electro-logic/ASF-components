// ILI9320 Display Controller Component Driver

#include "ili9320.h"

struct spi_device ili9320_spi_device_conf = { .id = CONF_ILI9320_CS_PIN };

void ili9320_controller_init_registers_seller()
{		
	ili9320_write_reg(ILI9320_DRIVER_CODE_READ,0x0000);
	ili9320_write_reg(ILI9320_DRIVER_OUTPUT_CONTROL_1,0x0100);		/* Driver Output Contral */
	ili9320_write_reg(ILI9320_LCD_DRIVING_CONTROL,0x0700);			/* LCD Driver Waveform Contral */
	ili9320_write_reg(ILI9320_ENTRY_MODE,0x1038);					/* Set the scan mode AM = 1: the address is updated in vertical writing direction*/
	ili9320_write_reg(ILI9320_RESIZE_CONTROL,0x0000);				/* Scalling Contral */
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_2,0x0202);			/* Display Contral 2 */
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_3,0x0000);			/* Display Contral 3 */
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_4,0x0000);			/* Frame Cycle Contal */
	ili9320_write_reg(ILI9320_RGB_DISPLAY_INTERFACE_CONTROL_1,(1<<0));	/* Extern Display Interface Contral 1: 16-bit RGB interface (1 transfer/pixel), DB[17:13] and DB[11:1]*/
	ili9320_write_reg(ILI9320_FRAME_MARKER_POSITION,0x0000);			/* Frame Maker Position */
	ili9320_write_reg(ILI9320_RGB_DISPLAY_INTERFACE_CONTROL_2,0x0000);	/* Extern Display Interface Contral 2 */
	delay_ms(50);
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_1,0x0101);							/* Display Contral */
	delay_ms(50);
	ili9320_write_reg(ILI9320_POWER_CONTROL_1,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4)); /* Power Control 1 constant current-op amp 0.5 0.62*/
	ili9320_write_reg(ILI9320_POWER_CONTROL_2,0x0007);                              /* Power Control 2 */
	ili9320_write_reg(ILI9320_POWER_CONTROL_3,(1<<8)|(1<<4)|(0<<0));                /* Power Control 3 */
	ili9320_write_reg(ILI9320_POWER_CONTROL_4,0x0b00);                              /* Power Control 4 */
	ili9320_write_reg(ILI9320_POWER_CONTROL_7,0x0000);                              /* Power Control 7 */
	ili9320_write_reg(ILI9320_FRAME_RATE_AND_COLOR_CONTROL,(1<<14)|(1<<4));			// 90 Hz ????? 1<<14??? 1<<9 maybe
	
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_START_POSITION,0);     /* Set X Start */
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_END_POSITION,239);	    /* Set X End */
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_START_POSITION,0);	    /* Set Y Start */
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_END_POSITION,319);	    /* Set Y End */
	delay_ms(50);

	ili9320_write_reg(ILI9320_DRIVER_OUTPUT_CONTROL_2,0x2700);		/* Driver Output Control */
	ili9320_write_reg(ILI9320_BASE_IMAGE_DISPLAY_CONTROL,0x0001);	/* Driver Output Control */
	ili9320_write_reg(ILI9320_VERTICAL_SCROLL_CONTROL,0x0000);		/* Vertical Srcoll Control */
	
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_DISPLAY_POSITION,0x0000);		/* Display Position? Partial Display 1 */
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_AREA_START_LINE,0x0000);		/* RAM Address Start? Partial Display 1 */
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_AREA_END_LINE,0x0000);		/* RAM Address End-Partial Display 1 */
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_DISPLAY_POSITION,0x0000);		/* Displsy Position? Partial Display 2 */
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_AREA_START_LINE,0x0000);		/* RAM Address Start? Partial Display 2 */
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_AREA_END_LINE,0x0000);		/* RAM Address End? Partial Display 2 */
		
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_1,(0<<7)|(16<<0));	/* Frame Cycle Contral */
	//ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_1, 0x300);
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_2,0x0000);			/* Panel Interface Contral 2 */
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_3,0x0001);			/* Panel Interface Contral 3 */
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_4,0x0110);			/* Frame Cycle Contral */
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_5,(0<<8));
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_6,0x0000);			/* Frame Cycle Contral */
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_1,0x0133);
	delay_ms(100);   /* delay 50 ms */		
}
// Initialize all the display registers 
void ili9320_controller_init_registers(void)
{		
	// Start Initial Sequence
	ili9320_write_reg(0x00E5, 0x8000);							// Set the internal vcore voltage
	ili9320_write_reg(ILI9320_START_OSCILLATION, 0x0001);		// Start internal OSC.
	delay_ms(10);
	ili9320_write_reg(ILI9320_DRIVER_OUTPUT_CONTROL_1, 0x0100); // set SS and SM bit
	ili9320_write_reg(ILI9320_LCD_DRIVING_CONTROL, 0x0700);		// set 1 line inversion
	ili9320_write_reg(ILI9320_ENTRY_MODE, 0x1030);				// set GRAM write direction and BGR=1 (Swap the R and B order of written data)		
	//ili9320_write_reg(ILI9320_ENTRY_MODE, 0x1430);				// High Speed
	//#if LCD_ALIGNMENT == VERTICAL
	//	ili9320_write_reg(ILI9320_ENTRY_MODE, 0x1030); 	//Entry mode
	//#elif LCD_ALIGNMENT == HORIZONTAL
	//	ili9320_write_reg(ILI9320_ENTRY_MODE, 0x1028); 	//Entry mode
	//#endif
	
	ili9320_write_reg(ILI9320_RESIZE_CONTROL, 0x0000);			// Resize register	
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_2, 0x0202);		// set the back porch and front porch
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_3, 0x0000);		// set non-display area refresh cycle ISC[3:0]
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_4, 0x0000);		// FMARK function	
	
	ili9320_write_reg(ILI9320_RGB_DISPLAY_INTERFACE_CONTROL_1, 0x0000);		// RGB interface setting			
	ili9320_write_reg(ILI9320_FRAME_MARKER_POSITION, 0x0000);				// Frame marker Position
	ili9320_write_reg(ILI9320_RGB_DISPLAY_INTERFACE_CONTROL_2, 0x0000);		// RGB interface polarity
	
	// Power On sequence
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x0000);			// SAP, BT[3:0], AP, DSTB, SLP, STB
	ili9320_write_reg(ILI9320_POWER_CONTROL_2, 0x0007);			// DC1[2:0], DC0[2:0], VC[2:0]
	ili9320_write_reg(ILI9320_POWER_CONTROL_3, 0x0000);			// VREG1OUT voltage
	ili9320_write_reg(ILI9320_POWER_CONTROL_4, 0x0000);			// VDV[4:0] for VCOM amplitude
	delay_ms(200);												// Dis-charge capacitor power voltage	
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x17B0);			// SAP, BT[3:0], AP, DSTB, SLP, STB
	ili9320_write_reg(ILI9320_POWER_CONTROL_2, 0x0037);			// DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50);												// Delay 50ms
	ili9320_write_reg(ILI9320_POWER_CONTROL_3, 0x013A);			// VREG1OUT voltage
	delay_ms(50);												// Delay 50ms
	ili9320_write_reg(ILI9320_POWER_CONTROL_4, 0x1600);			// VDV[4:0] for VCOM amplitude
	ili9320_write_reg(ILI9320_POWER_CONTROL_7, 0x000C);			// VCM[4:0] for VCOMH
	delay_ms(50);
		
	ili9320_write_reg(ILI9320_FRAME_RATE_AND_COLOR_CONTROL, 0x0000);	// Internal Resistor, 100 Hz
	
	ili9320_write_reg(ILI9320_HORIZONTAL_GRAM_ADDRESS_SET, 0x0000);		// GRAM horizontal Address
	ili9320_write_reg(ILI9320_VERTICAL_GRAM_ADDRESS_SET, 0x0000);		// GRAM Vertical Address
	
	// Adjust the Gamma Curve
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_1, 0x0504);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_2, 0x0703);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_3, 0x0702);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_4, 0x0101);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_5, 0x0A1F);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_6, 0x0504);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_7, 0x0003);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_8, 0x0706);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_9, 0x0707);
	ili9320_write_reg(ILI9320_GAMMA_CONTROL_10, 0x091F);	
	
	// Set GRAM area
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_START_POSITION, 0x0000);	// Horizontal GRAM Start Address
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_END_POSITION, 239);		// Horizontal GRAM End Address
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_START_POSITION, 0x0000);		// Vertical GRAM Start Address
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_END_POSITION, 319);			// Vertical GRAM Start Address
	
	ili9320_write_reg(ILI9320_DRIVER_OUTPUT_CONTROL_2, 0x2700);				// Gate Scan Line
	ili9320_write_reg(ILI9320_BASE_IMAGE_DISPLAY_CONTROL, 0x0001);			// NDL,VLE, REV
	ili9320_write_reg(ILI9320_VERTICAL_SCROLL_CONTROL, 0x0000);				// set scrolling line
	
	// Partial Display Control
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_DISPLAY_POSITION, 0x0000);
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_AREA_START_LINE, 0x0000);
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_1_AREA_END_LINE, 0x0000);
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_DISPLAY_POSITION, 0x0000);
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_AREA_START_LINE, 0x0000);
	ili9320_write_reg(ILI9320_PARTIAL_IMAGE_2_AREA_END_LINE, 0x0000);
	
	// Panel Control
	//ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_1, 0x0010);			
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_1, 0x300);			// Fosc/8
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_2, 0x0000);
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_3, 0x0003);
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_4, 0x0110);
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_5, 0x0000);
	ili9320_write_reg(ILI9320_PANEL_INTERFACE_CONTROL_6, 0x0000);
	
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_1, 0x0173);					// 262K color and display ON
}

// Initialize SPI interface to the controller
void ili9320_interface_init(void)
{
	spi_flags_t spi_flags = SPI_MODE_3;
	board_spi_select_id_t spi_select_id = 0; 		

	spi_master_init(CONF_ILI9320_SPI);
	spi_master_setup_device(CONF_ILI9320_SPI, &ili9320_spi_device_conf, spi_flags, CONF_ILI9320_CLOCK_SPEED, spi_select_id);
	spi_enable(CONF_ILI9320_SPI);
}

// Reset the display using the digital control interface
void ili9320_reset_display(void)
{	
	ioport_set_pin_level(CONF_ILI9320_RESET_PIN, true);
	delay_ms(10);
	ioport_set_pin_level(CONF_ILI9320_RESET_PIN, false);
	delay_ms(10);
	ioport_set_pin_level(CONF_ILI9320_RESET_PIN, true);
	delay_ms(150);
}

// Initialize the controller
void ili9320_init(void)
{
	// Set the pin used for slave select as output high
	ioport_configure_pin(CONF_ILI9320_CS_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	// Enable pull-up on own chip select (SS), If this pin is pulled low the SPI module will go into slave mode
	// If the SS pin is not used and is configured as input, it must be held high to ensure master operation.
	// If the SS pin is set as input and is being driven low, the SPI module will interpret this as another master trying to take control of the bus
	ioport_configure_pin(CONF_ILI9320_SS_PIN, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	
	// Set MOSI and SCL as output high, and set MISO as input
	ioport_configure_pin(CONF_ILI9320_MOSI_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_pin(CONF_ILI9320_MISO_PIN, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_ILI9320_SCK_PIN,  IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	ioport_configure_pin(CONF_ILI9320_RESET_PIN, IOPORT_DIR_OUTPUT);
	
	// Initialize the communication interface
	ili9320_interface_init();

	// Reset the display
	ili9320_reset_display();		

	// Write all the controller registers with correct values
	//ili9320_controller_init_registers();
	ili9320_controller_init_registers_seller();
}

// Write 16-bit Index
void ili9320_write_index(uint16_t index)
{
	uint8_t data_buffer[3];	
	spi_select_device(&SPIC, &ili9320_spi_device_conf);
	data_buffer[0] = ILI9320_CMD_WRITE_INDEX;
	data_buffer[1] = (index >> 8);
	data_buffer[2] = (index & 0xFF);
	spi_write_packet(&SPIC, data_buffer, 3);
	spi_deselect_device(&SPIC, &ili9320_spi_device_conf);
}

// Write 16-bit Reg
void ili9320_write_data(uint16_t data)
{
	uint8_t data_buffer[3];	
	spi_select_device(&SPIC, &ili9320_spi_device_conf);
	data_buffer[0] = ILI9320_CMD_WRITE_CONTROL_GRAM_REGISTER;
	data_buffer[1] = (data >> 8);
	data_buffer[2] = (data & 0xFF);
	spi_write_packet(&SPIC, data_buffer, 3);
	spi_deselect_device(&SPIC, &ili9320_spi_device_conf);
}

void ili9320_write_reg(uint8_t regIndex, uint16_t regValue)
{	
	ili9320_write_index(regIndex);	
	ili9320_write_data(regValue);
}

void ili9320_exit_sleep(void)
{
	// Power On sequence
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x0000);		// SAP, BT[3:0], AP, DSTB, SLP
	ili9320_write_reg(ILI9320_POWER_CONTROL_2, 0x0000);		// DC1[2:0], DC0[2:0], VC[2:0]
	ili9320_write_reg(ILI9320_POWER_CONTROL_3, 0x0000);		// VREG1OUT voltage
	ili9320_write_reg(ILI9320_POWER_CONTROL_4, 0x0000);		// VDV[4:0] for VCOM amplitude
	delay_ms(200);											// Dis-charge capacitor power voltage
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x17B0);		// SAP, BT[3:0], AP, DSTB, SLP, STB
	ili9320_write_reg(ILI9320_POWER_CONTROL_2, 0x0037);		// DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50);											// Delay 50ms
	ili9320_write_reg(ILI9320_POWER_CONTROL_3, 0x013A);		// VREG1OUT voltage
	delay_ms(50);											// Delay 50ms
	ili9320_write_reg(ILI9320_POWER_CONTROL_4, 0x1600);		// VDV[4:0] for VCOM amplitude
	ili9320_write_reg(ILI9320_POWER_CONTROL_7, 0x000C);		// VCM[4:0] for VCOMH
	delay_ms(50);
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_1, 0x0173);	// 262K color and display ON
}

void ili9320_enter_sleep(void)
{
	ili9320_write_reg(ILI9320_DISPLAY_CONTROL_1, 0x0000);	// display OFF
	// Power OFF sequence
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x0000);		// SAP, BT[3:0], APE, AP, DSTB, SLP
	ili9320_write_reg(ILI9320_POWER_CONTROL_2, 0x0000);		// DC1[2:0], DC0[2:0], VC[2:0]
	ili9320_write_reg(ILI9320_POWER_CONTROL_3, 0x0000);		// VREG1OUT voltage
	ili9320_write_reg(ILI9320_POWER_CONTROL_4, 0x0000);		// VDV[4:0] for VCOM amplitude
	delay_ms(200);											// Dis-charge capacitor power voltage
	ili9320_write_reg(ILI9320_POWER_CONTROL_1, 0x0002);		// SAP, BT[3:0], APE, AP, DSTB, SLP
}

void ili9320_set_cursor(uint16_t H, uint16_t V)
{			
	ili9320_write_reg(ILI9320_HORIZONTAL_GRAM_ADDRESS_SET, H);
	ili9320_write_reg(ILI9320_VERTICAL_GRAM_ADDRESS_SET, V);
}

// Write the graphical memory with a single color pixel 
void ili9320_write_gram(ili9320_color_t color)
{	
	ili9320_write_reg(ILI9320_WRITE_DATA_GRAM, color);
}

// Copy pixels from program memory (flash) to the display 
void ili9320_copy_progmem_pixels_to_screen(ili9320_color_t PROGMEM_PTR_T pixels, uint32_t count)
{
	ili9320_color_t color;
	while (count--) {
		color = PROGMEM_READ_WORD(pixels);	
		ili9320_write_gram(color);	
		pixels++;
	}
}

void ili9320_clear_screen(ili9320_color_t color)
{	
	uint8_t data_buffer[3];									
	
	spi_select_device(&SPIC, &ili9320_spi_device_conf);										
	
	data_buffer[0] = ILI9320_CMD_WRITE_CONTROL_GRAM_REGISTER;
	data_buffer[1] = (color >> 8);
	data_buffer[2] = (color & 0xFF);	
	spi_write_packet(&SPIC, data_buffer, 3);
	
	uint32_t count = 76800-1; //240UL * 320UL;
		
	data_buffer[0] = (color >> 8);
	data_buffer[1] = (color & 0xFF);
	
	while (count--) {				
		spi_write_packet(&SPIC, data_buffer, 2);										
	}
	spi_deselect_device(&SPIC, &ili9320_spi_device_conf);					
}

void ili9320_set_window(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{	
	ili9320_set_cursor(startX,startY);
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_START_POSITION, startX);
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_START_POSITION, startY);
	ili9320_write_reg(ILI9320_HORIZONTAL_ADDRESS_END_POSITION, endX);
	ili9320_write_reg(ILI9320_VERTICAL_ADDRESS_END_POSITION, endY);
}

ili9320_color_t ili9320_rgb565(uint16_t red, uint16_t green, uint16_t blue)
{
	return (((red>>3) & 0x1F) << 11)  | (((green>>2) & 0x3F) << 5) | ((blue>>3) & 0x1F);
}