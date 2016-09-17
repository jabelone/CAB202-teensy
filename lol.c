#define F_CPU 8000000UL

#include <avr/io.h>
#include "cpu_speed.h"
#include <util/delay.h>
#include <string.h>

#include "my_functions.h"

int main(){
	set_clock_speed(CPU_8MHz);
    setup();
    lcdLight(1);
    _delay_ms(500);
    lcdLight(0);
    _delay_ms(500);
    lcdLight(1);

    while(1){

        if (readJoy("left") == 1) {
            led0(1);
            led1(0);
            _delay_ms(100);
        }

        else if (readJoy("right") == 1) {
            led0(0);
            led1(1);
            _delay_ms(100);
        }

        else if (readJoy("up") == 1) {
            led0(1);
            led1(1);
            _delay_ms(100);
        }

        else if (readJoy("down") == 1) {
            led0(0);
            led1(0);
            _delay_ms(100);
        }

        else if (readJoy("center") == 1) {
            PORTB ^= _BV(PB2);
            PORTB ^= _BV(PB3);
            _delay_ms(500);
        }

        if (readSwitch(2)) {
            _delay_ms(500);
            if (readSwitch(2)) {
                PORTC ^= _BV(PC7);
            }
        }

	}
}