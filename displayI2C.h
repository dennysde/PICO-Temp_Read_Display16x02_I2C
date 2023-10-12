#ifndef _DISPLAYI2C_H_
#define _DISPLAYI2C_H_

#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "displayI2C.h"

#define LCD_LIMPA_TELA          0x01
#define LCD_INICIA              0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAY_CONTROL     0x08
#define LCD_DISPLAY_FUNCTIONSET 0x20

#define LCD_INICIO_ESQUERDA 0x02
#define LCD_LIGA_DISPLAY    0x04

#define LCD_16X2 0x08

#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE_BIT 0x04

#define LCD_CARACTERE 1
#define LCD_COMANDO  0

#define MAX_LINHAS   2
#define MAX_COLUNAS 16

#define DISPLAY_BUS_ADDR 0x27

#define DELAY_US 600

void lcd_envia_comando(uint8_t val);
void lcd_pulsa_enable(uint8_t val);
void lcd_envia_byte(uint8_t caractere, int dado_comando);
void lcd_limpa_tela();
void lcd_posiciona_cursor(int linha, int coluna);
void lcd_envia_caractere(char caractere);
void lcd_envia_string(const char *s);
void lcd_init();

#endif /*_DISPLAYI2C_H_*/