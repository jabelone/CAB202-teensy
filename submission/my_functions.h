/////////////////////////////////////////////////  HELPER FUNCTIONS   /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The two functions below disable the watchdog timer after a reset stopping infinite resets
// Function Prototype
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

// Function Implementation
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}

// This sets every pin to the correct direction (INPUT or OUTPUT)
void setupIO() {
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

    // Initialise the ADC with a prescaler of 128
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

    sei(); // Globally enables interrupts.
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

void drawRect(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2-1, y1); // Draw top
    draw_line(x2-1, y1, x2-1, y2-1); // Draw right
    draw_line(x2-1, y2-1, x1, y2-1); // Draw bottom
    draw_line(x1, y2-1, x1, y1); // Draw left
}

void drawBorder() { drawRect(0, 10, 84, 48); } // Draw a border
void drawDankBorder() { drawRect(0, 0, 84, 48); } // Draw a dank border

// Draws a string that's centred in x direction
void centerString (unsigned char y, char* string) {
    unsigned char n = 0;
    while (string[n] != '\0') { n++; }
    char x = 42-(n*5/2);
    draw_string((x > 0) ? x : 0, y, string);
}

// Flashes LEDs alternatively, x times
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

// Init all the screen stuffs
void initScreen(int flash) {
    lcd_init(LCD_DEFAULT_CONTRAST); // Initialise the LCD with default contrast
    clear_screen(); // Clear the screen

    if (flash) {
        centerString(5, "CAB202 Snek");
        centerString(15, "Jaimyn Mayer");
        centerString(25, "n9749331");
        centerString(35, "Sem2 2016");
        drawRect(0, 0, 84, 48); // Draw a dank border
        show_screen();
        flashLeds(3);
        led0(0);
        led1(0);
        _delay_ms(2000);
    }

    lcdLight(1);
    clear_screen();
}

// Inverts the display mode
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

// Init the ADC
void initADC()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);

    // ADC Enable and pre-scaler of 128
    // 8000000/128 = 62500
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Read the ADC value
uint16_t readADC(uint8_t ch)
{
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

void draw_sprite2(Sprite* sprite, int width) {
    // Do nothing if not visible
    if (!sprite->is_visible) {
        return;
    }

    // Loop through the bit-packed bitmap data, drawing each individual bit
    // (assume that the bitmap size is h * ceil(w/8))
    unsigned char dx, dy, byte_width = ceil(sprite->width / 8.0f);
    for (dy = 0; dy<sprite->height; dy++) {
        for (dx = 0; dx<sprite->width; dx++) {
            set_pixel(
                    (unsigned char) sprite->x+dx,
                    (unsigned char) sprite->y+dy,
                    (sprite->bitmap[(int) (dy*byte_width+floor(dx/width))] >> (7 - dx%8)) & 1 // ouch...
            );
        }
    }
}

////////////////////////////////////////////////////// GAME LOGIC //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set up and initialise all of our sprites
void setupSprites() {
    // Our foodz
    init_sprite(&foodz, 10, 15, 8, 3, foodzbm);
    foodz.dx = 0;
    foodz.dy = 0;
    foodz.is_visible = 1;

    // Our snekz
    init_sprite(&snekz[0], 5, 10, 8, 3, snekzbm[0]);
    for (int i = 1; i < 25; i++) {
        init_sprite(&snekz[i], i * 4, 10, 8, 3, snekzbm[1]);
        snekz[i].dx = 0;
        snekz[i].dy = 0;
        snekz[i].is_visible = 0;
    }
    snekz[0].is_visible = 1;

    // Our heart/lives indicator
    for (int i = 0; i < 5; i++) {
        init_sprite(&heart[i], 76-i*8, 0, 8, 5, heartbm);
        heart[i].dx = 0;
        heart[i].dy = 0;
        heart[i].is_visible = 1;
    }
}

// Reset etc when it's game over
void gameOver() {
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

// Draw all of our sprites
void drawSprites() {

    // Our snekz
    for (int i = 0; i < 24; i++) {
        draw_sprite2(&snekz[i], 3);
    }

    // Our heart/lives indicator
    for (int i = 0; i < 5; i++) {
        draw_sprite(&heart[i]);
    }
    // Our foodz
    draw_sprite2(&foodz, 3);
}

// Draw our hearts (lives left)
void drawHearts(int lives) {
    for (int i = 0; i < lives; i++) {
        heart[i].is_visible = 1;
    }
    for (int i = lives; i < 5; i++) {
        heart[i].is_visible = 0;
    }
}

void collideFood() {
}

// Move snek according to dy dx values
void moveSnek(int gameSpeed) {
    snekz[0].x += snekz[0].dx * gameSpeed;
    snekz[0].y += snekz[0].dy * gameSpeed;

    if (snekz[0].x >= 84) { snekz[0].x = 0; }
    else if (snekz[0].x < 1) { snekz[0].x = 83; }

    if (snekz[0].y >= 45) { snekz[0].x = 6; }
    else if (snekz[0].y <= 6) { snekz[0].x = 45; }
}