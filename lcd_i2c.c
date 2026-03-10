#include "lcd_i2c.h"
#include <stdlib.h>
#include <util/delay.h>

// I2C (TWI) registerlarının tanımlı olup olmadığını kontrol et
#ifdef TWCR
    #define HAVE_I2C 1
#else
    #define HAVE_I2C 0
    #warning "I2C not supported on this MCU.LCD I2C functions will be disabled."
#endif

#if HAVE_I2C

// ==================== I2C TEMEL FONKSİYONLAR ====================
static void i2c_init(void)
{
    TWSR = 0x00;       // Prescaler = 1
    TWBR = 0x48;       // 100kHz @16MHz
}

static void i2c_start(uint8_t addr)
{
    TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
    TWDR = addr << 1;
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

static void i2c_stop(void)
{
    TWCR = (1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
    _delay_us(10);
}

static void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

// ==================== LCD DÜŞÜK SEVİYE FONKSİYONLAR ====================
static void lcd_pulse(uint8_t data)
{
    i2c_start(LCD_I2C_ADDR);
    i2c_write(data | LCD_EN);
    _delay_us(1);
    i2c_write(data & ~LCD_EN);
    i2c_stop();
}

static void lcd_write4(uint8_t data)
{
    i2c_start(LCD_I2C_ADDR);
    i2c_write(data | LCD_BL);
    lcd_pulse(data | LCD_BL);
    i2c_stop();
}

// ==================== LCD YÜKSEK SEVİYE FONKSİYONLAR ====================
void lcd_command(uint8_t cmd)
{
    lcd_write4(cmd & 0xF0);
    lcd_write4((cmd << 4) & 0xF0);
    _delay_ms(2);
}

void lcd_data(uint8_t data)
{
    lcd_write4((data & 0xF0) | LCD_RS);
    lcd_write4(((data << 4) & 0xF0) | LCD_RS);
    _delay_us(50);
}

void lcd_init(void)
{
    i2c_init();
    _delay_ms(50);

    // Başlangıç sequence
    lcd_write4(0x30);
    _delay_ms(5);
    lcd_write4(0x30);
    _delay_us(150);
    lcd_write4(0x30);
    _delay_us(150);
    lcd_write4(0x20); // 4-bit mode
    _delay_us(150);

    lcd_command(0x28); // 4-bit, 2 satır, 5x8 dots
    lcd_command(0x08); // Display off
    lcd_clear();
    lcd_command(0x06); // Entry mode
    lcd_command(0x0C); // Display on
}

void lcd_clear(void)
{
    lcd_command(0x01);
    _delay_ms(2);
}

void lcd_setcursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    
    if(row < LCD_ROWS && col < LCD_COLS)
    {
        lcd_command(0x80 | (col + row_offsets[row]));
    }
}

void lcd_print(char *str)
{
    while(*str)
    {
        lcd_data(*str++);
    }
}

void lcd_print_char(char c)
{
    lcd_data(c);
}

void lcd_print_int(int value)
{
    char buffer[8];
    itoa(value, buffer, 10);
    lcd_print(buffer);
}

#else  // HAVE_I2C == 0

// ==================== I2C OLMAYAN İŞLEMCİLER İÇİN BOŞ FONKSİYONLAR ====================
// Bu fonksiyonlar hiçbir şey yapmaz, sadece derleme hatasını önler
void lcd_command(uint8_t cmd) { (void)cmd; }
void lcd_data(uint8_t data) { (void)data; }
void lcd_init(void) { }
void lcd_clear(void) { }
void lcd_setcursor(uint8_t row, uint8_t col) { (void)row; (void)col; }
void lcd_print(char *str) { (void)str; }
void lcd_print_char(char c) { (void)c; }
void lcd_print_int(int value) { (void)value; }

#endif  // HAVE_I2C