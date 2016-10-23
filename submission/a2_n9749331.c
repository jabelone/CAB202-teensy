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

#define maxSnekz 40
#define startSnekz 2

float minSpeedModifier = 0.2;
float speedScaleFactor = 1500.0;
int lives = 5; // Lives
int score = startSnekz - 1;  // Our score
int scoreModifier = 1;
int snekTrail[maxSnekz][2]; // Trailing snekz history
int snekBodies = startSnekz; // Number of trailing snekz
int randomX; // Temp to hold random x
int randomY; // Temp to hold random y
unsigned long millis; // Hold an approximate number of millis since power on
unsigned long lastInput; // Hold the time we had the last input

Sprite heart[5];
Sprite snekz[maxSnekz];
Sprite foodz;
Sprite ff;

unsigned char foodzbm[3] = {0x40, 0xA0, 0x40}; // Our food bitmap
unsigned char snekzbm[2][3] = {{0xE0, 0xA0, 0xE0}, {0xE0, 0xE0, 0xE0}}; // Our snek bitmaps (1 for head, 1 for body)
unsigned char heartbm[5] = {0x36, 0x7F, 0x3E, 0x1C, 0x08}; // Our heart bitmap
unsigned char ffbm[7] = {0x88, 0xCC, 0xEE, 0xFF, 0xEE, 0xCC, 0x88}; // Our heart bitmap

//All my helper functions
#include "my_functions.h"

int main() {
    initADC();
    float gameSpeed = 0;
    char buf[10];

    srand(readADC(1));
    randomX = rand() % 84;
    randomY = (rand() % (48 + 1 - 8)) + 8;
    set_clock_speed(CPU_8MHz);
    setupIO();
    initScreen(0);
    setupSprites();
    foodz.x = randomX; // Spawn the foodz randomly
    foodz.y = randomY;

    while (1) {
        if (1) {
            _delay_ms(50);
            if (lives < 1) {
                gameOver();  // If we die, end the game - does a software reset so it starts again.
            }

            gameSpeed = readADC(1) / speedScaleFactor + minSpeedModifier;
            foodCollision();
            clear_screen(); // Clear the last frame so we can draw the next one

            drawHearts(lives); // Update the hearts (lives left)
            sprintf(buf, "%d", score);
            draw_string(0, 0, buf);
            snprintf(buf, 3, "%f", gameSpeed * 100);
            draw_string(30, 0, buf);
            draw_sprite(&ff);
            moveSnek();
            drawSprites(); // Update all the sprites

        }

        if (gameSpeed * 100 > 88) {

            ff.is_visible = !ff.is_visible;
            invertScreen(1);
            draw_string(42, 0, "TimeWarp ");

        }
        else if (millis % 10 == 0) {
            ff.is_visible = !ff.is_visible;
            invertScreen(0);
        }
        else invertScreen(0);

        show_screen();
    }
}

ISR(TIMER0_OVF_vect) {
    millis += 1;
    processInputs();
}
