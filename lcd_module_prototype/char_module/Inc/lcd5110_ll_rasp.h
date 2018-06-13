/*
 * lcd5110_ll.h -- LCD 5110 low-level interface.
 *
 * RST        Reset pin for LCD
 * CE        Chip enable for SPI3
 * DC        Data/Command pin
 * DIN        MOSI pin for SPIx
 * CLK        CLOCK pin for SPIx
 * VCC3.3V     VCC Power
 * LIGHT    GND    If you connect this to GND, backlight is turned on
 * GND        GND    Ground
 *
 * IT is expected, that SPI and all used pins are correctly initialized
 * (for example by cube-generated code, though, can be initialized by
 * any suitable means).
 *
 * SPI Configuration:
 * Frame format -- "Motorola"
 * Data size -- 8 bit
 * First bit -- MSB First
 * Speed -- 4Mbit/s or less (prescaler for 64MHz -- 8 or more)
 * Clock polarity (CPOL) -- Low
 * Clock phase (CPHA) -- 1 Edge
 * (So called mode 00)
 * CRC Calculation -- disabled
 * NSS -- Software.
 *  Created on: 1 ��. 2017
 *      Author: indrekis
 */

#ifndef APPLICATION_USER_LCD5110_LL_H_
#define APPLICATION_USER_LCD5110_LL_H_

#include <stdbool.h>

#define BLACK 1
#define WHITE 0

#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD5110_BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT/8)


typedef enum {LCD5110_NORMAL_MODE    = 4 | LCD5110_MODE_SET_MASK,
    LCD5110_INVERTED_MODE  = 5 | LCD5110_MODE_SET_MASK,
    LCD5110_ALL_WHITE_MODE = 0 | LCD5110_MODE_SET_MASK,
    LCD5110_ALL_BLACK_MODE = 1 | LCD5110_MODE_SET_MASK
} LCD5110_modes;


typedef struct LCD5110_conf{
    spi_device *spi;
    int spi_cs_pin;
    int rst_pin;
    int dc_pin;
    unsigned char video_buffer[LCD5110_BUFFER_SIZE];
} LCD5110_conf;

void LCD5110_refresh_ll(LCD5110_conf* lcd_conf);

int LCD5110_set mode(LCD5110_conf* lcd_conf);
int LCD5110_set_temp_coef(uint8_t TC, LCD5110_conf* lcd_conf);
int LCD5110_set_bias(uint8_t bias, LCD5110_conf* lcd_conf);
