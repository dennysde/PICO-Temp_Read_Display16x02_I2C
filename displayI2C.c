#include "displayI2C.h"

void lcd_envia_comando(uint8_t val) {
    i2c_write_blocking(i2c_default,DISPLAY_BUS_ADDR, &val, 1, false);
}

void lcd_pulsa_enable(uint8_t val){
    sleep_us(DELAY_US);
    lcd_envia_comando(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    lcd_envia_comando(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void lcd_envia_byte(uint8_t caractere, int dado_comando) {
    uint8_t nible_high = dado_comando | (caractere & 0xF0) | LCD_BACKLIGHT;
    uint8_t nible_low = dado_comando | ((caractere << 4) & 0xF0) | LCD_BACKLIGHT;

    lcd_envia_comando(nible_high);
    lcd_pulsa_enable(nible_high);
    lcd_envia_comando(nible_low);
    lcd_pulsa_enable(nible_low);
}

void lcd_limpa_tela() {
    lcd_envia_byte(LCD_LIMPA_TELA, LCD_COMANDO);
}

void lcd_posiciona_cursor(int linha, int coluna){
    int aux = (linha == 0) ? 0x80 + coluna : 0xC0 + coluna;
    lcd_envia_byte(aux, LCD_COMANDO);
}

void lcd_envia_caractere(char caractere) {
    lcd_envia_byte(caractere, LCD_CARACTERE);
}

void lcd_envia_string(const char *s) {
    while(*s) {
        lcd_envia_caractere(*s++);
    }
}

void lcd_init() {
    lcd_envia_byte(LCD_INICIA, LCD_COMANDO);
    lcd_envia_byte(LCD_INICIA | LCD_LIMPA_TELA, LCD_COMANDO);
    lcd_envia_byte(LCD_ENTRYMODESET | LCD_INICIO_ESQUERDA, LCD_COMANDO);
    lcd_envia_byte(LCD_DISPLAY_FUNCTIONSET | LCD_16X2, LCD_COMANDO);
    lcd_envia_byte(LCD_DISPLAY_CONTROL | LCD_LIGA_DISPLAY, LCD_COMANDO);
    lcd_limpa_tela();
}