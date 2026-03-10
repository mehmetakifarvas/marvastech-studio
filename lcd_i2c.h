#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ==================== DEFAULT LCD AYARLARI ====================
// Kullanıcı main.c'de tanımlamazsa bu değerler kullanılır

#ifndef LCD_TYPE
#define LCD_TYPE LCD_16x2   // Varsayılan tip: 16x2
#endif

#ifndef LCD_I2C_ADDR
#define LCD_I2C_ADDR 0x27    // Varsayılan adres: 0x27
#endif

// ==================== LCD Tipi Seçimi ====================
#define LCD_16x2 1
#define LCD_20x4 2
#define LCD_20x2 3

#if LCD_TYPE == LCD_16x2
#define LCD_COLS 16
#define LCD_ROWS 2
#elif LCD_TYPE == LCD_20x4
#define LCD_COLS 20
#define LCD_ROWS 4
#elif LCD_TYPE == LCD_20x2
#define LCD_COLS 20
#define LCD_ROWS 2
#else
#error "Geçersiz LCD tipi! LCD_16x2, LCD_20x4 veya LCD_20x2 kullanın."
#endif

// ==================== LCD Bit Tanımları ====================
#define LCD_RS  0x01
#define LCD_RW  0x02
#define LCD_EN  0x04
#define LCD_BL  0x08

// ==================== Fonksiyon Prototipleri ====================
void lcd_init(void);
void lcd_clear(void);
void lcd_setcursor(uint8_t row, uint8_t col);
void lcd_print(char *str);
void lcd_print_char(char c);
void lcd_print_int(int value);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);

#endif