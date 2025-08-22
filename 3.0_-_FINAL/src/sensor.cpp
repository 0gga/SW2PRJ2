#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"
#include "systemControl.h"

#define PIR_PIN 7
#define FLED_PIN A0

extern Sensor sensor;
extern SystemControl systemControl;
extern TimerControl timerControl;
extern Light light;

Sensor::Sensor() {
    pinMode(FLED_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
}

size_t Sensor::readFLED() {
    int FLEDraw = analogRead(FLED_PIN);
    Serial.println("FLED Raw: " + String(FLEDraw));
    FLEDraw = constrain(FLEDraw, 530, 920);
    size_t FLEDmapped = map(FLEDraw, 530, 920, 0, 100);
    Serial.println("FLED Mapped: " + String(FLEDmapped) + "%");
    return FLEDmapped;
}

bool Sensor::readPIR() {
    PIRraw = digitalRead(PIR_PIN);
    Serial.println("PIR Raw: " + String(PIRraw));
    return PIRraw;
}