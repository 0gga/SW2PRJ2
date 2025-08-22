#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"
#include "systemControl.h"

#define TOP 1023  // 10-bit PWM maximum value

extern Sensor sensor;
extern SystemControl systemControl;
extern TimerControl timerControl;
extern Light light;

Light::Light()
{
    // Opsætning af PWM til lys
    pinMode(6, OUTPUT);        // Pin 6 / PH3 er udgang    

    TCCR4A = 0b10100011;        // PWM, Phase Correct, 10-bit, (NON-INVERTING - Compare match når der tælles ned)
                                // 0x00FF (1023) som TOP med A- og B-kanaler aktiveret
    TCCR4B = 0b00000010;        // Prescaler '8'
    
    OCR4A = 0;
    PWMmanual = 50;
    PWMauto = 100;
}

void Light::turnOn() {
    lightToggle = true;
}

void Light::turnOff() {
    lightToggle = false;
}

void Light::ctrlLight(size_t PWM) {
    OCR4A = PWM;
}

void Light::adjustBrightness(size_t level) {
    if (!lightToggle){
        OCR4A = 0;
        return;
    }
    
    if (level >= 0 && level <= 100){
        ctrlLight(map(level, 0, 100, 0, 1023));
        Serial.println("Manual brightness set to " + String(level) + "%");
    }
    else {
        Serial.println("Fatal error: Invalid manual brightness level");
    }
}

void Light::adjustAutoBrightness(size_t level) {
    if (!lightToggle){
        OCR4A = 0;
        return;
    }
    
    if (systemControl.toggleAuto){
        if (level >= 0 && level <= 100){
            size_t curvedoutput = pow(map(level, 0, 100, 0, 1023) / 1023.0, 2.2) * 1023;
            ctrlLight(curvedoutput);
            Serial.println("Auto brightness mapped: " + String(map(level, 0, 100, 0, 1023)) + ", Curved: " + String(curvedoutput));   //map(level, 0, 100, 0, 1023)
            Serial.println("Auto brightness set to " + String(level) + "%");
        }                                            
        else {
            Serial.println("Fatal error: Invalid auto brightness level");
        }
    }
}