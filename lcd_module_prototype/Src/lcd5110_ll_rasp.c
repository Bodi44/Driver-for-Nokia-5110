#include <linux/spi.h>
#include <linux/gpio.h>


#define RST 24
#define DC 23
#define SCLK 11
#define CS 8
#define DIN 10

static inline void init_requsts(void){
	gpio_requst(RST, "sysfs");
	gpio_requst(DC, "sysfs");
	gpio_requst(SCLK, "sysfs");
	gpio_requst(CS, "sysfs");
	gpio_requst(DIN, "sysfs");
	gpio_direction_output(CS, 0);
	gpio_direction_output(RST, 0);
	volatile int i = 100;
	while(--i){}
	LCD5110_RST_on();
	gpio_direction_output(DC, 0);

}

int send_byte_to_LCD5110(struct spi_device spi*, const void buf*){
	return spi_write(spi, buf, 1);
}

int send_data_to_LCD5110(struct spi_device spi*, const void buf*, size_t len){
	return spi_write(spi, buf, len);
}

static inline void LCD5110_CE_on(void){
	gpio_set_value(CS, 1);
}
static inline void LCD5110_CE_off(void){
	gpio_set_value(CS, 0);
}

static inline void LCD5110_DC_on(void) {
	gpio_set_value(DC, 0);
}
//! Commands mode on
static inline void LCD5110_DC_off(void) {
	gpio_set_value(DC, 0);
}
static inline void LCD5110_RST_on(void) {
	gpio_set_value(RST, 1);
}
static inline void LCD5110_RST_off(void) {
	gpio_set_value(RST, 0);
}

static inline int LCD5110_cmd_nop(struct spi_device spi*, const void buf*, size_t len)
{
	send_byte_to_LCD5110(spi, buf, len);
	return LCD5110_OK;
}



#define LCD5110_FN_SET_MASK (1<<5)
//! PD: 0 -- active, 1 -- power down.
//! "Before entering Power-down mode, the RAM needs to be filled
//!  with ‘0’s to ensure the specified current consumption"
#define LCD5110_FN_SET_PD_BIT 2
#define LCD5110_FN_SET_PD_MASK (1<<LCD5110_FN_SET_PD_BIT)
//! V: 0 -- horizontal addressing, 1 -- vertical addressing
#define LCD5110_FN_SET_V_BIT 1
#define LCD5110_FN_SET_V_MASK (1<<LCD5110_FN_SET_V_BIT)
//! H: 0 -- basic instruction set, 1 -- extended instruction set
#define LCD5110_FN_SET_H_BIT 0
#define LCD5110_FN_SET_H_MASK 1

#define FN_SET_MASK (1<<5)

//! Code: 0010 0PVH, accepts both 00100PVH and 0PVH, but no other.
static int LCD5110_set_function(uint8_t fn_byte, spi_device spi*)
{
	if ( (fn_byte & ~FN_SET_MASK) > 7) //0b111
	{
		//printf("Seq: err\n");
		return LCD5110_bad_opcode;
	}
	//printf("Seq: %i, dbg: %i \n", fn_byte | FN_SET_MASK, fn_byte );
	send_byte_to_LCD5110(spi, fn_byte | FN_SET_MASK,);
	return LCD5110_OK;
}

//! H==0
static int LCD5110_set_mode_base(LCD5110_modes mode_byte, spi_device spi*)
{
	if ( (mode_byte & (~LCD5110_INVERTED_MODE) ) != 0) //0b10x0y -- only possible values
	{
		//printf("Seq: err\n");
		return LCD5110_bad_dmode;
	}
	//printf("Seq: %i\n", mode_byte);
	send_byte_to_LCD5110(mode_byte, spi);
	return LCD5110_OK;
}

//! User mode function -- takes care about correct instruction set
int LCD5110_set_mode(LCD5110_modes mode, spi_device spi*)
{
	LCD5110_set_function(0, spi); // H==1, basic instruction set
	LCD5110_DC_off(); // Commands mode on
	int ec = LCD5110_set_mode_base(mode, spi);
	LCD5110_DC_on(); // Data mode on -- default
	return ec;
}

#define LCD5110_VOLTAGE_MASK (1<<7)

//! H==1
static int LCD5110_set_voltage_ext(uint8_t voltage, spi_device spi*)
{
	if ( voltage > 127)
	{
		//printf("Seq: err\n");
		return LCD5110_bad_voltage;
	}
	//printf("Seq: %i\n", voltage | LCD5110_VOLTAGE_MASK);
	send_byte_to_LCD5110(spi, voltage | LCD5110_VOLTAGE_MASK);
	return LCD5110_OK;
}

#define LCD5110_TEMP_COEFF_MASK (1<<2)
//! H==1
static int LCD5110_set_temp_coef_ext(uint8_t TC, spi_device* spi)
{
	if ( TC > 3)
	{
		//printf("Seq: err\n");
		return LCD5110_bad_TC;
	}
	//printf("Seq: %i\n", TC | LCD5110_TEMP_COEFF_MASK);
	send_byte_to_LCD5110(spi, TC | LCD5110_TEMP_COEFF_MASK);
	return LCD5110_OK;
}

//! User mode function -- takes care about correct instruction set
int LCD5110_set_temp_coef(uint8_t TC, spi_device spi*)
{
	LCD5110_set_function(LCD5110_FN_SET_H_MASK, spi); // H==1, basic instruction set
	LCD5110_DC_off(); // Commands mode on
	int ec = LCD5110_set_temp_coef_ext(TC, spi);
	LCD5110_DC_on(); // Data mode on -- default
	return ec;
}

#define LCD5110_BIAS_MASK (1<<4)
//! H==1
static int LCD5110_set_bias_ext(uint8_t bias, spi_device spi*)
{
	if ( bias > 7)
	{
		//printf("Seq: err\n");
		return LCD5110_bad_bias;
	}
	//printf("Seq: %i\n", bias | LCD5110_BIAS_MASK);
	send_byte_to_LCD5110(spi, bias | LCD5110_BIAS_MASK);
	return LCD5110_OK;
}

//! User mode function -- takes care about correct instruction set
int LCD5110_set_bias(uint8_t bias, spi_device spi*)
{
	LCD5110_set_function(LCD5110_FN_SET_H_MASK, spi); // H==1, basic instruction set
	LCD5110_DC_off(); // Commands mode on
	int ec = LCD5110_set_bias_ext(bias, spi);
	LCD5110_DC_on(); // Data mode on -- default
	return ec;
}

#define LCD5110_SET_X_BIT 7
#define LCD5110_SET_X_BIT_MASK (1<<LCD5110_SET_X_BIT)
#define LCD5110_SET_Y_BIT 6
#define LCD5110_SET_Y_BIT_MASK (1<<LCD5110_SET_Y_BIT)


static inline int LCD5110_set_X_base(int16_t x, spi_device spi*)
{
	if(x<0 || x>LCD_WIDTH-1)
		return LCD5110_bad_coordinate;
	send_byte_to_LCD5110(spi, x | LCD5110_SET_X_BIT_MASK);
	return LCD5110_OK;
}

//! Byte addresable!
static inline int LCD5110_set_Y_base(int16_t y, spi_device spi*)
{
	if(y<0 || y>LCD_HEIGHT/8-1) // Byte addressable!
		return LCD5110_bad_coordinate;
	send_byte_to_LCD5110(spi, y | LCD5110_SET_Y_BIT_MASK);
	return LCD5110_OK;
}

static inline int LCD5110_set_XY_base(int16_t x, int16_t y, spi_device spi*)
{
	int res;
	res = LCD5110_set_X_base(x, spi);
	if( res != LCD5110_OK)
		return res;
	res = LCD5110_set_Y_base(y, spi);
	if( res != LCD5110_OK)
		return res;

	return LCD5110_OK;
}

static inline void init_requsts(void){
    gpio_requst(RST, "sysfs");
    gpio_requst(DC, "sysfs");
    gpio_requst(SCLK, "sysfs");
    gpio_requst(CS, "sysfs");
    gpio_requst(DIN, "sysfs");
    gpio_direction_output(CS, 0);
    gpio_direction_output(RST, 0);
    volatile int i = 100;
    while(--i){}
    LCD5110_RST_on();
    gpio_direction_output(DC, 0);
    struct spi_master *master;
    struct spi_board_info spi_device_info = {
        .modalias = "st7735",
        .max_speed_hz = 62000000, //speed of your device splace can handle
        .bus_num = 0, //BUS number
        .chip_select = 0,
        .mode = SPI_MODE_2,  //SPI mode 3, 2 and 0 works
    };
    master = spi_busnum_to_master(spi_device_info.bus_num);
}


//
void LCD5110_refresh_ll(spi_device spi*)
{
	LCD5110_CE_off();
	LCD5110_DC_off();
	LCD5110_set_XY_base(0, 0, spi);
	LCD5110_DC_on();

	send_data_to_LCD5110(spi, lcd_conf->video_buffer, LCD_HEIGHT*LCD_WIDTH/8);
//!TODO: Check for transmission end and turn CE off (set it to HIGH).
}
