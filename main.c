/*
 *Title: *********Temp_Read_Display16x02_I2C*********
 *Author: Dennys Ramos
 *Date: Oct/12nd/2023
 *Brief: Read RP2040 nucleo's temperature every 2 seconds and send the data to the I2C display and PC via USB_VCP (115200bps/ 8bits/ 1Stop / No Parity)
 
*/
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "displayI2C.h"
#include "math.h"

/* Choose 'C' for Celsius or 'F' for Fahrenheit. */
#define TEMPERATURE_UNITS 'C'

const uint led_pin = 25;
uint16_t tempo = 500;
char tempString[6];

float read_onboard_temperature(const char unit);

void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len) ;

int main() {

    // Initialize chosen serial port
    stdio_init_all();

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    /* Initialize hardware AD converter, enable onboard temperature sensor and
     *   select its channel */
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    //Init display 16x02 throuth I2C
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    //Wait to be able to open terminal
    sleep_ms(5000);

    lcd_init();
    lcd_posiciona_cursor(0, 0);
    lcd_envia_string("Ola Mundo");
    
    // Loop forever
    while (true) 
    {   
        gpio_put(led_pin, true);

        lcd_limpa_tela();
        lcd_posiciona_cursor(0, 2);
        lcd_envia_string("TEMPERATURA");
        lcd_posiciona_cursor(1,2);
        lcd_envia_string("PICO ");


        float temperature = read_onboard_temperature(TEMPERATURE_UNITS);
        printf("Onboard temperature = %.02f %c\n", temperature, TEMPERATURE_UNITS);
        //float to string
        ftoa(temperature, tempString, 2);
        lcd_envia_string(tempString);
        lcd_envia_string("C");

        sleep_ms(2000);

        gpio_put(led_pin, false);
        sleep_ms(50);
    }

    return 0;
}

/* References for this implementation:
 * raspberry-pi-pico-c-sdk.pdf, Section '4.1.1. hardware_adc'
 * pico-examples/adc/adc_console/adc_console.c */
float read_onboard_temperature(const char unit) {
    
    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    if (unit == 'C') {
        return tempC;
    } else if (unit == 'F') {
        return tempC * 9 / 5 + 32;
    }

    return -1.0f;
}

// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
 
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
 
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
 
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}

// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
 
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
 
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
}

// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 