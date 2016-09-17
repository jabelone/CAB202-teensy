// All mah functions

// This sets every pin to the correct direction (INPUT or OUTPUT)
void setup() {
    // B REGISTER
    DDRB &= (0 << 0); //Set SWCENTER as INPUT
    DDRB &= (0 << 1); //Set SWB as INPUT
    DDRB |= (1 << 2); //Set LED0 as OUTPUT
    DDRB |= (1 << 3); //Set LED1 as OUTPUT
    DDRB |= (1 << 4); //Set LCD_RST as OUTPUT
    DDRB |= (1 << 5); //Set LCD_DC as OUTPUT
    DDRB |= (1 << 6); //Set LCD_DIN as OUTPUT
    DDRB |= ~(1 << 7); //Set SWA as INPUT

    // C REGISTER
    //DDRC &= ~(1<<6); //Set S0 as INPUT (expansion header)
    DDRC |= (1<<7); //Set LCD_LED_CTL as OUTPUT

    // D REGISTER
    DDRD &= (0<<0); //Set SWD as INPUT
    DDRD &= (0<<1); //Set SWC as INPUT
    //DDRD &= ~(1<<2); DON'T TOUCH!! (RX)
    //DDRD &= ~(1<<3); DON'T TOUCH!! (TX)
    DDRD |= (1<<7); //Set LCD_SCE as OUTPUT

    // F REGISTER
    DDRF &= (0<<0); //Set ADC0 (Pot0) as INPUT
    DDRF &= (0<<1); //Set ADC1 (Pot1) as INPUT
    //DDRF &= ~(1<<4); //Set S1 as INPUT (expansion header)
    DDRF &= (0<<5); //Set switch 3 as INPUT
    DDRF &= (0<<6); //Set switch 2 as INPUT
    DDRF |= (1<<6); //Set LCD_SCK as OUTPUT
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