#include "app.h"
#include "oled.h"
#include "oled_horse.h"
#include <stdio.h>


void delay(){
	for (volatile int i = 0; i < 10; i++);
}

static void draw_bar(uint16_t adc_value)
{
    uint32_t bar_width;
    uint16_t x;
    uint16_t y;

    bar_width = ((uint32_t)adc_value * OLED_WIDTH) / 4095UL;

    for (x = 0U; x < OLED_WIDTH; x++) {
        for (y = 54U; y < 63U; y++) {
            if (x < bar_width) {
                oled_pixel(x, y, OLED_WHITE);
            } else {
                oled_pixel(x, y, OLED_BLACK);
            }
        }
    }
}

int main(void)
{
    uint16_t adc_value;
    uint32_t millivolts;
    char text[24];


    oled_init();
    adc1_pa0_init();

    while (1)
    {
        adc_value = adc1_read();
        millivolts = adc_to_millivolts(adc_value);

        oled_clear(OLED_BLACK);

        oled_cursor(0, 0);
        oled_text("ADC PA0", &font_11x18, OLED_WHITE);

        oled_cursor(0, 22);
        snprintf(text, sizeof(text), "Raw: %4u", adc_value);
        oled_text(text, &font_7x10, OLED_WHITE);

        oled_cursor(0, 36);
        snprintf(text, sizeof(text), "Volt:%lu.%03lu V",
                 millivolts / 1000UL,
                 millivolts % 1000UL);
        oled_text(text, &font_7x10, OLED_WHITE);

        draw_bar(adc_value);

        oled_update();

        delay();
    }
}
