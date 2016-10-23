// Jaimyn Mayer (n9749331) 2016 for QUT CAB202

//Other libraries
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//CAB202 Libraries
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

#define maxSnekz 30
#define startSnekz 3

float minSpeedModifier = 0.2;
float speedScaleFactor = 1500.0;
int lives = 5; // Lives
int score = startSnekz - 2;  // Our score
int scoreModifier = 1;
int snekTrail[maxSnekz+1][2]; // Trailing snekz history
int snekBodies = startSnekz; // Number of trailing snekz
int randomX; // Temp to hold random x
int randomY; // Temp to hold random y
unsigned long millis; // Hold an approximate number of millis since power on
unsigned long lastInput; // Hold the time we had the last input
unsigned long runNow;
float gameSpeed = 0;
int warp = 0;

Sprite heart[5];
Sprite snekz[maxSnekz+1];
Sprite foodz;
Sprite ff;

unsigned char foodzbm[3] = {0x40, 0xA0, 0x40}; // Our food bitmap
unsigned char snekzbm[2][3] = {{0xE0, 0xA0, 0xE0},
                               {0xE0, 0xE0, 0xE0}}; // Our snek bitmaps (1 for head, 1 for body)
unsigned char heartbm[5] = {0x36, 0x7F, 0x3E, 0x1C, 0x08}; // Our heart bitmap
unsigned char ffbm[7] = {0x88, 0xCC, 0xEE, 0xFF, 0xEE, 0xCC, 0x88}; // Our heart bitmap
const int line1[4] = {4, 10, 4, 28};
const int line2[4] = {0, 45, 42, 45};
const int line3[4] = {84, 15, 60, 15};
int walls = 0;

//All my helper functions
#include "my_functions.h"

int main() {
    initADC();
    char buf[10];

    srand(6);
    randomX = rand() % 84;
    randomY = (rand() % (48 + 1 - 8)) + 8;
    set_clock_speed(CPU_8MHz);
    setupIO();
    initScreen(0);
    setupSprites();
    foodz.x = randomX; // Spawn the foodz randomly
    foodz.y = randomY;

    while (1) {
        if (score >= maxSnekz-1) winnerChickenDinner();
        processInputs();
        if (runNow == 1) {
            runNow = 0;
            if (lives < 1) {
                gameOver();  // If we die, end the game - does a software reset so it starts again.
            }

            clear_screen(); // Clear the last frame so we can draw the next one

            snekCollision();
            foodCollision();
            drawWalls();
            drawHearts(lives); // Update the hearts (lives left)
            sprintf(buf, "%d", score);
            draw_string(0, 0, buf);
            snprintf(buf, 3, "%f", gameSpeed * 100);
            draw_string(26, 0, buf);
            draw_sprite(&ff);
            moveSnek();
            drawSprites(); // Update all the sprites
            if (warp) draw_string(38, 0, "WarpSpeed ");
            show_screen();
            led1(1);
        }
        led1(0);
    }
}

ISR(TIMER0_OVF_vect) {
    millis += 1;
    if (millis % (int)(50.0/gameSpeed) == 0) runNow = 1;
    if (gameSpeed * 100 > 88) {
        warp = 1;
        if (millis % 100 == 0) {
            ff.is_visible = !ff.is_visible;
            invertScreen(1);
        }

    }
    else if (millis % (int)(100.0/gameSpeed) == 0) {
        ff.is_visible = !ff.is_visible;
        warp = 0;
        invertScreen(0);
    }
}
