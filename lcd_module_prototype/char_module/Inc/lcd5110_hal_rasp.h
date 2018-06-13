void LCD5110_clrscr(LCD5110_conf* lcd_conf);
void LCD5110_fillscr(LCD5110_conf* lcd_conf);

void LCD5110_putpix(int x, int y, int mode, LCD5110_conf* lcd_conf);
void LCD5110_setpix(int x, int y, LCD5110_conf* lcd_conf);
void LCD5110_clrpix(int x, int y, LCD5110_conf* lcd_conf);
void LCD5110_invpix(int x, int y, LCD5110_conf* lcd_conf);
void LCD5110_fill_region(int x0, int y0, int w, int h, int color, LCD5110_conf* lcd_conf);

void LCD5110_drawBitmap(int x, int y, const unsigned char* bitmap, int cols, int rows, int invert, LCD5110_conf* lcd_conf);
