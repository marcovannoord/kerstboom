#include <Arduino.h>
#include <avr/power.h>
#define PIN_LED_PWM 10
#define PIN_CURRENT_DRAIN 2             // deze pin verbinden via een 10k weerstand met de Drain van de mosfet
#define PIN_MEASURE_VOLTAGE_DROP A1     // Deze pin via een 10k weerstand verbinden met de drain van de mosfet, en met een 10k weerstand verbinden met de GND
#define MINIMUM_VOLTAGE_DROP_VALUE 15  // minimum waarde die gelezen moet worden over de sense-weerstand, voordat we vinden dat de ledstrip aan mag gaan

bool led_staat_aan = false;

void setup() {
    // put your setup code here, to run once:
    clock_prescale_set(clock_div_4);
    pinMode(PIN_LED_PWM, OUTPUT);
    pinMode(PIN_CURRENT_DRAIN, OUTPUT);
    pinMode(PIN_MEASURE_VOLTAGE_DROP, INPUT);
    Serial.begin(9600);
}

void loop() {
    if (meetAanwezigheidLedstrip() == true) {  // als de functie aangeeft dat er een ledstrip aanwezig is
        if (led_staat_aan == false) {
            Serial.println("Ledstrip gedetecteerd, ik ga langzaam aan");
            gaLangzaamAan();
        }
    } else {
        Serial.println("Ledstrip weggehaald, ik ga nu uit");
        digitalWrite(PIN_LED_PWM, LOW);  // zet de led uit
        led_staat_aan = false;
    }
    delay(500);  // meet de aanwezigheid van de ledstrip elke 500 ms
}
bool meetAanwezigheidLedstrip() {
    // eerst de ledstrip uit zetten voordat we kunnen meten, indien deze aan stond
    if (led_staat_aan) {
        digitalWrite(PIN_LED_PWM, LOW);
    }
    digitalWrite(PIN_CURRENT_DRAIN, LOW);  // er gaat nu stroom lopen vanaf de ledstrip naar de arduino, als die aanwezig is. De ledstrip zal een klein beetje oplichten
    delayMicroseconds(10); // delay a very very short time, for the voltage to stabilize
    int input = analogRead(PIN_MEASURE_VOLTAGE_DROP);  // lees de spanning over de sense-weerstand
    digitalWrite(PIN_CURRENT_DRAIN, HIGH);             // stop de stroom naar de arduino
    if (led_staat_aan)                                 // als de led hiervoor aanstond, zet deze weer aan
    {
        digitalWrite(PIN_LED_PWM, HIGH);
    }
    Serial.print("Gemeten waarde: ");
    Serial.println(input);
    if (input >= MINIMUM_VOLTAGE_DROP_VALUE) {  // als de gemeten waarde hoger is dan MINIMUM_VOLTAGE_DROP_VALUE
        return true;                            // return "true" om terug te geven dat er een ledstrip aanwezig is
    } else {
        return false;  // geen ledstrip aanwezig, return false
    }
}

void gaLangzaamAan() {
    for (size_t i = 0; i <= 255; i++) {
        analogWrite(PIN_LED_PWM, i);
        delay(25);  // speel hiermee om in te stellen hoe snel hij aan moet gaan
    }
    Serial.println("Led staat nu volledig aan");
    led_staat_aan = true;  // onthoud dat de led aan staat, zodat we niet opnieuw gaan faden
}