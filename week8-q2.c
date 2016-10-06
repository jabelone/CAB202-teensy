/*	CAB202: Tutorial 8
*	Question 2 - Template
*
*	B.Talbot, April 2016
*	Queensland University of Technology
*/
#include <avr/io.h>
#include <util/delay.h>

#include "cpu_speed.h"
#include "graphics.h"
#include "lcd.h"
#include "sprite.h"


/*
* Function declarations - you only have to touch the first two!!!
*/
unsigned char set_contrast(unsigned char current);
unsigned char set_inversion(unsigned char current);

void create_screen();
void inverted(int);
void contrast(int);

int invert_status = 0;

/*
* Main function - this is all done for you... no need to modify
*/
int main(void) {
    // Set the CPU speed to 8MHz (you must also be compiling at 8MHz)
            set_clock_speed(CPU_8MHz);

    // Get our data directions set up as we please (everything is an output unless specified...)
    DDRB = 0b01111100;
    DDRF = 0b10011111;
    DDRD = 0b11111100;

    // Initialise the LCD screen
    lcd_init(LCD_DEFAULT_CONTRAST);

    // Let's draw our pretty little set up...
    create_screen();

    // Run our main loop (buttons can change contrast or invert)
    unsigned char btns, mode_inversion = 0, mode_contrast = 1;
    while (1) {
        // Check if we've got a button press
        btns = (PINF >> PF5) & 0x03;
        if (btns > 0x00) {
            // We have a press, wait until it's done
            _delay_ms(50);
            while (((PINF >> PF5) & 0x03) > 0x00);
            _delay_ms(50);

            // Process the press
            if (btns > 0x01) {
                // Left press - change the contrast
                mode_contrast = set_contrast(mode_contrast);
            } else {
                // Right press - toggle the inversion setting
                mode_inversion = set_inversion(mode_inversion);
            }
        }
    }

    // End the program (the screen will remain...)
    return 0;
}

/*
* Function implementations
*/
unsigned char set_contrast(unsigned char current) {
    if (current < 2) { // If we aren't at max contrast
        current += 1; // Increase it
    }
    else { // If we are at max contrast
        current = 0; // Reset to min
    }

    contrast(current); // Set the contrast
    return current; // Return the (new) set value
}

unsigned char set_inversion(unsigned char current) {
    if (current == 1) { // If we are inverted already
        lcd_write(0, 0b00001100);  // Set it not inverted
        return 0; // Return the new state
    }
    else  { // If we are not inverted
        lcd_write(0, 0b00001101); // Set it inverted
        return 1; // Return the new state
    }
}

void create_screen() {
    // Text
    draw_string(LCD_X/2-20, LCD_Y/2-3, "Hi CAB202");
    show_screen();

    // Top row
    lcd_position(0, 0);
    for (unsigned char i = 0; i<LCD_X; i++) {
        lcd_write(LCD_D, 0xFF);
    }

    // Bottom row
    lcd_position(0, 5);
    for (unsigned char i = 0; i<LCD_X; i++) {
        lcd_write(LCD_D, 0xFF);
    }

    // Left row and right 'rows'
    for (unsigned char i = 1; i<LCD_Y/8-1; i++) {
        for (unsigned char ii = 0; ii < 8; ii++) {
            lcd_position(ii, i);
            lcd_write(LCD_D, 0xFF);
            lcd_position(LCD_X-1-ii, i);
            lcd_write(LCD_D, 0xFF);
        }
    }
}

void contrast(int input) {
    lcd_write(LCD_C, 0x21); // Enable LCD extended command set
    switch(input) {
        case 0:
            lcd_write(LCD_C, 0x80 | 0x2F ); // Set LCD Vop (Contrast)
            break;
        case 1:
            lcd_write(LCD_C, 0x80 | 0x3F ); // Set LCD Vop (Contrast)
            break;
        case 2:
            lcd_write(LCD_C, 0x80 | 0x4F ); // Set LCD Vop (Contrast)
            break;
    }
    lcd_write(LCD_C, 0x20); // Enable LCD basic command set
}