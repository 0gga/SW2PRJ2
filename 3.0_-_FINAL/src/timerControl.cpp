#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"
#include "systemControl.h"

extern Sensor sensor;
extern SystemControl systemControl;
extern TimerControl timerControl;
extern Light light;

volatile unsigned long TimerControl::autoModeOverflowCount = 0;
volatile unsigned long TimerControl::autoModeTargetOverflows = 0;
volatile unsigned long TimerControl::pirOverflowCount = 0;
volatile unsigned long TimerControl::pirTargetOverflows = 0;


unsigned long TimerControl::getPIROverflow(){
    return pirOverflowCount;
}
unsigned long TimerControl::getPIRTargetOverflow(){
    return pirTargetOverflows;
}

void TimerControl::startAutoModeTimer(unsigned long minutes) {
    if(minutes == 0){
        return;
    }
    autoModeOverflowCount = millis();
    autoModeTargetOverflows = minutes * 60000UL;

    Serial.print("Auto Mode Timer started: ");
    if (minutes > 1) {
        Serial.print(minutes);
        Serial.print(" minutes");
    }
    else if (minutes == 1){
        Serial.print(minutes);
        Serial.print(" minute");
    }
    else if (minutes < 1) {
        Serial.print(minutes * 60);
        Serial.print(" seconds");
    }
    Serial.println();
}

void TimerControl::handleAutoModeOverflow() {
    if (autoModeTargetOverflows == 0) {
        Serial.println("Auto Mode timer not active");
        return;
    }

    if (systemControl.autoModeTimer == 0){
        Serial.println("Auto Mode timer deactivated");
        autoModeOverflowCount = 0;
        autoModeTargetOverflows = 0;
        return;
    }

    if (systemControl.toggleAuto == true) {
        Serial.println("Auto Mode timer deactivated");
        autoModeOverflowCount = 0;
        autoModeTargetOverflows = 0;
        return;
    }
    

    unsigned long now = millis();
    unsigned long elapsed;

    if (now < autoModeOverflowCount) {
        elapsed = (0xFFFFFFFF - autoModeOverflowCount) + now + 1;
    } else {
        elapsed = now - autoModeOverflowCount;
    }

    unsigned long remaining = autoModeTargetOverflows - elapsed;
    unsigned long remainingSeconds = remaining / 1000;
    unsigned long remainingMinutes = remainingSeconds / 60;
    remainingSeconds %= 60;

    if (remaining > 0) {
    Serial.print("Auto Mode timer Remaining: ");
    if (remainingMinutes > 0) {
        Serial.print(remainingMinutes);
        Serial.print(remainingMinutes == 1 ? " minute " : " minutes ");
    }
    Serial.print(remainingSeconds);
    Serial.print(remainingSeconds == 1 ? " second" : " seconds");
    Serial.println();
}


    if (elapsed >= autoModeTargetOverflows) {
        autoModeTargetOverflows = 0;
        systemControl.autoModeOn();
        Serial.println("System has reverted to auto mode.");
    }
}

void TimerControl::startPIRTimer(unsigned long minutes) {
    pirOverflowCount = millis();
    pirTargetOverflows = minutes * 60000UL;

    Serial.print("PIR Timer started: ");
    if (minutes > 1) {
        Serial.print(minutes);
        Serial.print(" minutes");
    }
    else if (minutes == 1){
        Serial.print(minutes);
        Serial.print(" minute");
    }
    else if (minutes < 1) {
        Serial.print(minutes * 60);
        Serial.print(" seconds");
    }
    Serial.println();
}

void TimerControl::handlePIROverflow() {
    if (pirTargetOverflows == 0) return;

    unsigned long now = millis();
    unsigned long elapsed;

    if (now < pirOverflowCount) {
        elapsed = (0xFFFFFFFF - pirOverflowCount) + now + 1;
    } else {
        elapsed = now - pirOverflowCount;
    }

    unsigned long remaining = pirTargetOverflows - elapsed;
    unsigned long remainingSeconds = remaining / 1000;
    unsigned long remainingMinutes = remainingSeconds / 60;
    remainingSeconds %= 60;

    if (remaining > 0) {
    Serial.print("PIR timer Remaining: ");
    if (remainingMinutes > 0) {
        Serial.print(remainingMinutes);
        Serial.print(remainingMinutes == 1 ? " minute " : " minutes ");
    }
    Serial.print(remainingSeconds);
    Serial.print(remainingSeconds == 1 ? " second" : " seconds");
    Serial.println();
}

    if (elapsed >= pirTargetOverflows) {
        pirTargetOverflows = 0;
        light.turnOff();
        Serial.println("PIR Timer expired - Light turned off");
        
        pirOverflowCount = 0;
    }
}