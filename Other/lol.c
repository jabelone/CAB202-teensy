

//Other libraries
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

//CAB202 Libraries
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

//All my helper functions
#include "my_functions.h"

#define MOVE_DELAY 4 // Approximate refresh rate in hertz (Hz)
int moveAmount = 2; // How many pixels to move each time
int squareSize = 3; // Size of the "character square"

int dotX = 0; // Start the character square at x of 39
int dotY = 0; // Start the character square at y of 22

// Variable we use for game state - NOT CONFIGURABLE!
int dirX = 0; // Direction of x
int dirY = 0; // Direction of y
int xMod = 0; // Compensate by this amount when moving
int yMod = 0; // Makes it nicer to look at when moving
int lives = 3; // Lives
int tick = 0;
char str[10];

// Define our sprites
/*unsigned char bmSquare[3] = {0b00000111, 0b00000111, 0b00000111};
Sprite player;
init_sprite(player, 10, 10, 3, 3, bmSquare);
*/

#include "disablewdt.h"

int main(){



	set_clock_speed(CPU_8MHz);
    setup(); // Run meh setup code
    initScreen(); // Run meh screen setup code
    dotX = rand();
    dotY = rand();

    while(1){

        tick++;

        if (readJoy("left") == 1) { // If the left joystick button is pressed
            if (dirX == 2) { // If user tries to go back
                flashLeds(2);
                lives --;
                dirX = 2;
                dirY = 0;
                led0(0);
                led1(1);
            }
            else {
                led0(1);
                led1(0);
                dirX = 1;
                dirY = 0;
            }
        }

        else if (readJoy("right") == 1) { // If the right joystick button is pressed
            if (dirX == 1) { // If user tries to go back
                flashLeds(2);
                lives --;
                dirX = 1;
                dirY = 0;
                led0(1);
                led1(0);
            }
            else {
                led0(0);
                led1(1);
                dirX = 2;
                dirY = 0;
            }
        }

        else if (readJoy("up") == 1) { // If the up joystick button is pressed
            if (dirY == 2) {
                flashLeds(2);
                lives --;
                dirX = 0;
                dirY = 2;
                led0(1);
                led1(0);
            }
            else {
                led0(1);
                led1(1);
                dirX = 0;
                dirY = 1;
            }
        }

        else if (readJoy("down") == 1) { // If the down joystick button is pressed
            if (dirY == 1) {
                flashLeds(2);
                lives --;
                dirX = 0;
                dirY = 1;
                led0(1);
                led1(0);
            }
            led0(0);
            led1(0);
            dirX = 0;
            dirY = 2;
        }

        else if (readJoy("center") == 1) { // If the center joystick button is pressed
            PORTB ^= _BV(PB2);
            PORTB ^= _BV(PB3);
            _delay_ms(500);
        }

        if (dirX == 1) { // If we should move left
            dotX -= moveAmount;
            xMod = 0;
            yMod = 0;
        }

        else if (dirX == 2) { // If we should move right
            dotX += moveAmount;
            xMod = 0;
            yMod = 0;
        }

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

        if (dotX > 80) { // If we hit the right side then wrap around
            dotX = 1;
        }

        else if (dotX < 1) { // If we hit the left side then wrap around
            dotX = 80;
        }

        if (dotY > 44) { // If we hit the bottom side then wrap around
            dotY = 11;
        }

        else if (dotY < 11) { // If we hit the top side then wrap around
            dotY = 44;
        }

        if (lives < 1) {
            led0(0);
            led1(0);
            clear_screen();
            drawDankBorder();
            centerString(15, "GAME OVER");
            centerString(25, "DUDE!");
            show_screen();
            for (int i = 0; i < 2; i++) {
                lcdLight(0);
                _delay_ms(500);
                lcdLight(1);
                _delay_ms(500);
            }
            _delay_ms(2000);
            soft_reset();
        }

        clear_screen(); // Clear the previous frame
        itoa(tick, str, 10);
        char buffer[80];
        strcpy(buffer, " ");
        strcat(buffer, str);
        if (lives == 3) { strcat(buffer, "    # # #"); }
        else if (lives == 2) { strcat(buffer, "    # #"); }
        else if (lives == 1) { strcat(buffer, "    #"); }
        drawFilledRect(dotX+xMod, dotY+yMod, dotX+3, dotY+3); // Draw the character square
        drawBorder();
        draw_string(0, 0, buffer);
        _delay_ms(1000/MOVE_DELAY);
        show_screen();

        if (readSwitch(2)) {
            _delay_ms(250);
            if (readSwitch(2)) {
                PORTC ^= _BV(PC7);
            }
        }

	}
}