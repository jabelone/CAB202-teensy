// All mah functions

// This sets every pin to the correct direction (INPUT or OUTPUT)
void setup() {
    // B REGISTER
    DDRB &= ~(1 << 0); //Set SWCENTER as INPUT
    DDRB &= ~(1 << 1); //Set SWB as INPUT
    DDRB |= (1 << 2); //Set LED0 as OUTPUT
    DDRB |= (1 << 3); //Set LED1 as OUTPUT
    //DDRB |= (1 << 4); //Set LCD_RST as OUTPUT
    //DDRB |= (1 << 5); //Set LCD_DC as OUTPUT
    //DDRB |= (1 << 6); //Set LCD_DIN as OUTPUT
    DDRB |= ~(1 << 7); //Set SWA as INPUT

    // C REGISTER
    //DDRC &= ~(1<<6); //Set S0 as INPUT (expansion header)
    DDRC |= (1<<7); //Set LCD_LED_CTL as OUTPUT

    // D REGISTER
    DDRD &= ~(1<<0); //Set SWD as INPUT
    DDRD &= ~(1<<1); //Set SWC as INPUT
    //DDRD &= ~(1<<2); DON'T TOUCH!! (RX)
    //DDRD &= ~(1<<3); DON'T TOUCH!! (TX)
    //DDRD |= (1<<7); //Set LCD_SCE as OUTPUT

    // F REGISTER
    DDRF &= ~(1<<0); //Set ADC0 (Pot0) as INPUT
    DDRF &= ~(1<<1); //Set ADC1 (Pot1) as INPUT
    //DDRF &= ~(1<<4); //Set S1 as INPUT (expansion header)
    DDRF &= ~(1<<5); //Set switch 3 as INPUT
    DDRF &= ~(1<<6); //Set switch 2 as INPUT
    //DDRF |= (1<<6); //Set LCD_SCK as OUTPUT
}

// Control the state of LED0
void led0(int state) {
    if (state) { PORTB |= (1 << 2); }

    else { PORTB &= ~(1 << 2); }
}

// Control the state of LED1
void led1(int state) {
    if (state) { PORTB |= (1 << 3); }

    else { PORTB &= ~(1 << 3); }
}

// Control the state of the LCD backlight
void lcdLight(int state) {
    if (state) { PORTC |= (1 << 7); }

    else { PORTC &= ~(1 << 7); }
}

// Returns the state of the specified switch
int readSwitch(int num) {
    if (num == 2) { return (PINF>>6) & 1; }

    else if (num == 3) { return (PINF>>5) & 1; }

    else { return 0; }
}

#define soft_reset()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)

// Returns the state of the specified direction of the joystick
int readJoy(char * button){
    if (strcmp(button, "up") == 0) {
        return (PIND>>1) & 1;
    }

    else if (strcmp(button, "down") == 0) {
        return (PINB>>7) & 1;
    }

    else if (strcmp(button, "left") == 0) {
        return (PINB>>1) & 1;
    }

    else if (strcmp(button, "right") == 0) {
        return (PIND>>0) & 1;
    }

    else if (strcmp(button, "center") == 0) {
        return (PINB>>0) & 1;
    }

    return 2;
}

// Returns the state of the specified potentiometer
int readPot(int pot) {
    if (pot == 2) { return (PINF>>6) & 1; }

    else if (pot == 3) { return (PINF>>5) & 1; }

    else { return 0; }
}

void drawRect(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2-1, y1); // Draw top
    draw_line(x2-1, y1, x2-1, y2-1); // Draw right
    draw_line(x2-1, y2-1, x1, y2-1); // Draw bottom
    draw_line(x1, y2-1, x1, y1); // Draw left
}

void drawFilledRect(int x1, int y1, int x2, int y2) {
    //int y = y2-y1;
    int a;

    for (a = y1; a < y2; a++) {
        draw_line(x1, a, x2-1, a); // Draw line
    }

}

void drawBorder() { drawRect(0, 10, 84, 48); } // Draw a dank border
void drawDankBorder() { drawRect(0, 0, 84, 48); } // Draw a dank border

void centerString (unsigned char y, char* string) {
    unsigned char n = 0;
    while (string[n] != '\0') { n++; }
    char x = 42-(n*5/2);
    draw_string((x > 0) ? x : 0, y, string);
}


uint16_t adc_read(uint8_t ch) {
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing

    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));

    return (ADC);
}

int potPosition() {
    uint16_t adc = adc_read(1);

    float max_adc = 1023.0;
    long max_lcd_adc = (adc*(long)(LCD_X - 12)) / max_adc;

    return max_lcd_adc + 2;
}

void flashLeds(int times) {
    for (int i = 1; i <= times; i++) {
        led0(1);
        led1(0);
        lcdLight(0);
        _delay_ms(250);
        led0(0);
        led1(1);
        lcdLight(1);
        _delay_ms(250);
    }
}

void initScreen() {
    lcd_init(LCD_DEFAULT_CONTRAST); // Initialise the LCD with low contrast
    clear_screen(); // Clear the screen
    centerString(5, "CAB202 Snek");
    centerString(15, "Jaimyn Mayer");
    centerString(25, "n9749331");
    centerString(35, "Sem2 2016");
    drawRect(0, 0, 84, 48); // Draw a dank border
    show_screen();

    flashLeds(3);

    lcdLight(1);
    led0(0);
    led1(0);
    _delay_ms(2000);
}

int invertStatus = 0;

void invertScreen(int input) {
    if (input) { // If we should write inverted
        invertStatus = 1;
    }

    else {
        lcd_write(0, 0b00001100);
        invertStatus = 0;
    }
}

void setContrast(int input) {
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