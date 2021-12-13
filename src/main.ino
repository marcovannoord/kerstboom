#include <Arduino.h>
#define PIN_LED_PWM 11
#define PIN_CURRENT_DRAIN 2 // deze pin verbinden via een 10k weerstand met de Drain van de mosfet
#define PIN_MEASURE_VOLTAGE_DROP A1 // Deze pin via een 10k weerstand verbinden met de drain van de mosfet, en met een 10k weerstand verbinden met de GND
#define MINIMUM_VOLTAGE_DROP_VALUE 170 // minimum waarde die gelezen moet worden over de sense-weerstand, voordat we vinden dat de ledstrip aan mag gaan


void setup() {
  // put your setup code here, to run once:
    pinMode(PIN_LED_PWM, OUTPUT);
    pinMode(PIN_CURRENT_DRAIN, OUTPUT);
    pinMode(PIN_MEASURE_VOLTAGE_DROP, INPUT);
    Serial.begin(9600);
}

void loop() {
  if(meetAanwezigheidLedstrip() == true){ // als de functie aangeeft dat er een ledstrip aanwezig is
    Serial.println("Ledstrip gedetecteerd, ik ga langzaam aan");
    gaLangzaamAan();

  }
  delay(500); // meet de aanwezigheid van de ledstrip elke 500 ms

}
bool meetAanwezigheidLedstrip(){
  digitalWrite(PIN_CURRENT_DRAIN, LOW); // er gaat nu stroom lopen vanaf de ledstrip naar de arduino, als die aanwezig is. De ledstrip zal een klein beetje oplichten
  // delay(1);
  int input = analogRead(PIN_MEASURE_VOLTAGE_DROP); // lees de spanning over de sense-weerstand
  digitalWrite(PIN_CURRENT_DRAIN, HIGH ); // stop de stroom naar de arduino
  Serial.print("Gemeten waarde: ");
  Serial.println(input);
  if(input >= MINIMUM_VOLTAGE_DROP_VALUE){ // als de gemeten waarde hoger is dan MINIMUM_VOLTAGE_DROP_VALUE
    return true;          // return "true" om terug te geven dat er een ledstrip aanwezig is
  }else{
    return false;         // geen ledstrip aanwezig, return false
  }
}

void gaLangzaamAan(){
    for (size_t i = 0; i <= 255; i++) {
    analogWrite(PIN_LED_PWM, i);
    delay(100); // speel hiermee om in te stellen hoe snel hij aan moet gaan
  }
}