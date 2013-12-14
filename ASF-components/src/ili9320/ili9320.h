// ILI9320 Display Controller Component Driver

#ifndef ILI9320_H_
#define ILI9320_H_

// use ASF SPI service and Delay routines
#include <asf.h>

#include "ili9320_regs.h"

// Controller and interface configuration file
#include "conf_ili9320.h"

/** Height of display using default orientation */
#define ILI9320_DEFAULT_HEIGHT   240

/** Width of display using default orientation */
#define ILI9320_DEFAULT_WIDTH    320

/** Type define for an integer type large enough to store a pixel color. */
typedef uint16_t ili9320_color_t;

#define ILI9320_RED		0xF800
#define ILI9320_GREEN	0x07E0
#define ILI9320_BLUE	0x001F
#define ILI9320_WHITE	0xFFFF
#define ILI9320_BLACK	0x0000
   
void ili9320_init(void);
void ili9320_write_reg(uint8_t regIndex, uint16_t regValue);
void ili9320_exit_sleep(void);
void ili9320_enter_sleep(void);

void ili9320_set_cursor(uint16_t H, uint16_t V);
void ili9320_write_gram(ili9320_color_t color);
//void ili9320_copy_pixels_to_screen(const ili9320_color_t *pixels, uint32_t count);
void ili9320_copy_progmem_pixels_to_screen(ili9320_color_t PROGMEM_PTR_T pixels, uint32_t count);
void ili9320_clear_screen(ili9320_color_t color);
void ili9320_set_window(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);
ili9320_color_t ili9320_rgb565(uint16_t red, uint16_t green, uint16_t blue);

#endif /* IL9320_H_ */