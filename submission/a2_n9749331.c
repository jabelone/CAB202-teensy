//Other libraries
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//CAB202 Libraries
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

#define debounceDelay 10 // Debounce delay in ms
int lives = 5; // Lives
int score = 1;  // Our score
int snekTrail = 1; // How many trailing snekz
int randomX;
int randomY;

Sprite heart[5];
Sprite snekz[25];
Sprite foodz;

unsigned char foodzbm[3] = {0x40, 0xA0, 0x40}; // Our food bitmap
unsigned char snekzbm[2][3] = {{0xE0, 0xA0, 0xE0}, {0xE0, 0xE0, 0xE0}}; // Our snek bitmaps (1 for head, 1 for body)
unsigned char heartbm[5] = {0x36, 0x7F, 0x3E, 0x1C, 0x08}; // Our heart bitmap

//All my helper functions
#include "my_functions.h"

int main() {
    initADC();
    srand(readADC(0));
    randomX = rand() % 84;
    randomY = (rand() % (48 + 1 - 6)) + 6;

    set_clock_speed(CPU_8MHz);
    setupIO(); // Run meh setup code
    initScreen(0); // Run meh screen setup code
    setupSprites();
    foodz.x = randomX;
    foodz.y = randomY;

    while (1) {

        if (lives < 1) {
            gameOver();  // If we die, end the game - does a soft reset so it starts again.
        }

        if (readJoy("left")) {
            _delay_ms(debounceDelay);
            if (readJoy("left")) {
                snekz[0].dx = -1;
            }
        }
        if (readJoy("right")) {
            _delay_ms(debounceDelay);
            if (readJoy("right")) {
                snekz[0].dx = 1;
            }
        }

        clear_screen(); // Clear the last frame so we can draw the next one

        char int_buf[10];
        //char disp_buf[32];
        itoa(readADC(1)/200+1, int_buf, 10);
        //sprintf(disp_buf,"ADC1 = %s",int_buf);
        //centerString(25, disp_buf);

        collideFood();
        moveSnek(readADC(1)/300+1);
        drawHearts(lives); // Update the hearts (lives left)
        drawSprites(); // Update all the sprites

        show_screen(); // Push everything to the LCD
        _delay_ms(10); // Make it a bit smoother
    }
}