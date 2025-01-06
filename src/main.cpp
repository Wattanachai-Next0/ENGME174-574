#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

int menu = 0;

int sw_next = 15;
int sw_ok = 2;

int leds[] = {23, 19, 18, 5, 17, 16, 4, 0};
const int num_leds = sizeof(leds) / sizeof(leds[0]);

// Function prototypes
void executeMenu(int menu);
void turnOnAllLEDs();
void blinkAlternateLEDs();
void rotateSingleLED();
void blinkInPairs();
void sequentialOnOff();
void blinkAllLEDs();
void turnOffAllLEDs();

void setup() {
    lcd.init(); // initialize the LCD
    lcd.backlight();

    pinMode(sw_next, INPUT_PULLUP);
    pinMode(sw_ok, INPUT_PULLUP);

    for (int i = 0; i < num_leds; i++) {
        pinMode(leds[i], OUTPUT);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menu: 0");
}

void loop() {
    static int last_next_state = HIGH;
    static int current_menu = 0;
    static unsigned long last_action_time = 0;

    int current_next_state = digitalRead(sw_next);

    // Handle menu navigation
    if (current_next_state == LOW && last_next_state == HIGH) {
        menu++;
        if (menu > 6) {
            menu = 0;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Menu: ");
        lcd.setCursor(6, 0);
        lcd.print(menu);

        // Update current menu immediately
        current_menu = menu;
        last_action_time = millis(); // Reset the timer for debounce
    }

    // Execute the current menu
    if (current_menu == 0) {
        turnOffAllLEDs();
    } else {
        executeMenu(current_menu);
    }

    last_next_state = current_next_state;
}

void executeMenu(int menu) {
    switch (menu) {
        case 1:
            turnOnAllLEDs();
            break;
        case 2:
            blinkAlternateLEDs();
            break;
        case 3:
            rotateSingleLED();
            break;
        case 4:
            blinkInPairs();
            break;
        case 5:
            sequentialOnOff();
            break;
        case 6:
            blinkAllLEDs();
            break;
        default:
            turnOffAllLEDs();
            break;
    }
}

void turnOnAllLEDs() {
    for (int i = 0; i < num_leds; i++) {
        digitalWrite(leds[i], HIGH);
    }
}

void turnOffAllLEDs() {
    for (int i = 0; i < num_leds; i++) {
        digitalWrite(leds[i], LOW);
    }
}

void blinkAlternateLEDs() {
    static unsigned long last_time = 0;
    static bool state = false;

    if (millis() - last_time >= 300) {
        last_time = millis();
        state = !state;
        for (int i = 0; i < num_leds; i++) {
            digitalWrite(leds[i], (i % 2 == 0) == state ? HIGH : LOW);
        }
    }
}

void rotateSingleLED() {
    static int current_led = 0;
    static unsigned long last_time = 0;

    if (millis() - last_time >= 200) {
        last_time = millis();
        turnOffAllLEDs();
        digitalWrite(leds[current_led], HIGH);
        current_led = (current_led + 1) % num_leds;
    }
}

void blinkInPairs() {
    static unsigned long last_time = 0;
    static bool state = false;

    if (millis() - last_time >= 500) {
        last_time = millis();
        state = !state;
        if (state) {
            for (int i = 0; i < num_leds; i += 2) {
                digitalWrite(leds[i], HIGH);
                if (i + 1 < num_leds) {
                    digitalWrite(leds[i + 1], HIGH);
                }
            }
        } else {
            turnOffAllLEDs();
        }
    }
}

void sequentialOnOff() {
    static int current_led = 0;
    static bool ascending = true;
    static unsigned long last_time = 0;

    if (millis() - last_time >= 200) {
        last_time = millis();
        turnOffAllLEDs();
        digitalWrite(leds[current_led], HIGH);

        if (ascending) {
            current_led++;
            if (current_led >= num_leds) {
                current_led = num_leds - 1;
                ascending = false;
            }
        } else {
            current_led--;
            if (current_led < 0) {
                current_led = 0;
                ascending = true;
            }
        }
    }
}

void blinkAllLEDs() {
    static unsigned long last_time = 0;
    static bool state = false;

    if (millis() - last_time >= 500) {
        last_time = millis();
        state = !state;
        if (state) {
            turnOnAllLEDs();
        } else {
            turnOffAllLEDs();
        }
    }
}