#include <avr/io.h>
#include "cpu_speed.h"

void setup() {
    // B REGISTER
    DDRB &= (0 << 0); //Set SWCENTER as INPUT
    DDRB &= (0 << 1); //Set SWB as INPUT
    DDRB &= (1 << 2); //Set LED0 as OUTPUT
    DDRB &= (1 << 3); //Set LED1 as OUTPUT
    DDRB &= (1 << 4); //Set LCD_RST as OUTPUT
    DDRB &= (1 << 5); //Set LCD_DC as OUTPUT
    DDRB &= (1 << 6); //Set LCD_DIN as OUTPUT
    DDRB &= (0 << 7); //Set SWA as INPUT

    // C REGISTER
    //DDRC &= ~(1<<6); //Set S0 as INPUT (expansion header)
    DDRC &= (1<<7); //Set LCD_LED_CTL as OUTPUT

    // D REGISTER
    DDRD &= (0<<0); //Set SWD as INPUT
    DDRD &= (0<<1); //Set SWC as INPUT
    //DDRD &= ~(1<<2); DON'T TOUCH!! (RX)
    //DDRD &= ~(1<<3); DON'T TOUCH!! (TX)
    DDRD &= (1<<7); //Set LCD_SCE as OUTPUT

    // F REGISTER
    DDRF &= ~(0<<0); //Set ADC0 (Pot0) as INPUT
    DDRF &= (0<<1); //Set ADC1 (Pot1) as INPUT
    //DDRF &= ~(1<<4); //Set S1 as INPUT (expansion header)
    DDRF &= (0<<5); //Set switch 3 as INPUT
    DDRF &= (0<<6); //Set switch 2 as INPUT
    DDRF &= (1<<6); //Set LCD_SCK as OUTPUT
}

void led0(int state) {
    if (state) { PORTB |= (1 << 2); }

    else { PORTB |= (0 << 2); }
}

void led1(int state) {
    if (state) { PORTB |= (1 << 3); }

    else { PORTB |= (0 << 3); }
}

int readSwitch(int num) {
    if (num == 2) { return (PINF>>6) & 1; }

    else if (num == 3) { return (PINF>>5) & 1; }

    else { return 0; }
}

int main(){
	set_clock_speed(CPU_8MHz);
    setup();

	while(1){
        if (readSwitch(3) == 0) {
            led0(0);
        }

        else {
            led0(1);
            led1(1);
        }

	}
}
