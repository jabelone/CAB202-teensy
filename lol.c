

//Other libraries
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>

//CAB202 Libraries
#include "cpu_speed.h"
#include "graphics.h"
#include "lcd.h"

//All my helper functions
#include "my_functions.h"

#define MOVE_DELAY 10 // Approximate refresh rate in hertz (Hz)
int moveAmount = 2; // How many pixels to move each time
int squareSize = 3; // Size of the "character square"

int dotX = 39; // Start the character square at x of 39
int dotY = 22; // Start the character square at y of 22

// Variable we use for game state - NOT CONFIGURABLE!
int dirX = 0; // Direction of x
int dirY = 0; // Direction of y
int xMod = 0; // Compensate by this amount when moving
int yMod = 0; // Makes it nicer to look at when moving

int main(){
	set_clock_speed(CPU_8MHz);
    setup(); // Run meh setup code
    initScreen(); // Run meh screen setup code

    while(1){

        if (readJoy("left") == 1) {
            led0(1);
            led1(0);
            dirX = 1;
            dirY = 0;
        }

        else if (readJoy("right") == 1) {
            led0(0);
            led1(1);
            dirX = 2;
            dirY = 0;
        }

        else if (readJoy("up") == 1) {
            led0(1);
            led1(1);
            dirX = 0;
            dirY = 1;
        }

        else if (readJoy("down") == 1) {
            led0(0);
            led1(0);
            dirX = 0;
            dirY = 2;
        }

        else if (readJoy("center") == 1) {
            PORTB ^= _BV(PB2);
            PORTB ^= _BV(PB3);
            _delay_ms(500);
        }

        clear_screen();
        drawBorder();

        if (dirX == 1) { // If we should move left
            dotX -= moveAmount;
            xMod = -1;
            yMod = 0;
        }

        else if (dirX == 2) { // If we should move right
            dotX += moveAmount;
            xMod = -1;
            yMod = 0;
        }

        else {  } // Otherwise do nothing

        if (dirY == 1) { // If we should move left
            dotY -= moveAmount;
            xMod = 0;
            yMod = 0;
        }

        else if (dirY == 2) { // If we should move right
            dotY += moveAmount;
            xMod = 0;
            yMod = 0;
        }

        else {  } // Otherwise do nothing

        if (dotX > 80) {
            dotX = 1;
        }

        else if (dotX < 1) {
            dotX = 80;
        }

        if (dotY > 44) {
            dotY = 0;
        }

        else if (dotY < 1) {
            dotY = 44;
        }

        // Draw the lead square - xMod and yMod make it nicer to look at when moving
        drawFilledRect(dotX+xMod, dotY+yMod, dotX+3, dotY+3);
        _delay_ms(1000/MOVE_DELAY);
        show_screen();

        if (readSwitch(2)) {
            _delay_ms(500);
            if (readSwitch(2)) {
                PORTC ^= _BV(PC7);
            }
        }

	}
}