#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"
#include "systemControl.h"

extern Sensor sensor;
extern SystemControl systemControl;
extern TimerControl timerControl;
extern Light light;

SystemControl::SystemControl() {
    toggleAuto = true; // Default auto mode
    PIRtimer = 1;      // Default PIRtimeout
    autoModeTimer = 1; // Default autoModeTimer
}

void SystemControl::autoModeOn() {
    toggleAuto = true;
}

void SystemControl::autoModeOff() {
    toggleAuto = false;
    timerControl.startAutoModeTimer(autoModeTimer);
}

void SystemControl::setPIRtimer(size_t timer) {
    PIRtimer = timer;
}

void SystemControl::setAutoModeTimer(size_t timer){
    autoModeTimer = timer;
}

void SystemControl::ctrlFLED() {
    if (!toggleAuto) {
        light.adjustBrightness(light.PWMmanual);
        return;
    }
    size_t ambientLight = sensor.readFLED();

    if (ambientLight >= light.PWMauto) {
    light.adjustAutoBrightness(0);
    }

    else {
        size_t brightness = light.PWMauto - ambientLight;
        light.adjustAutoBrightness(brightness);
    }
}

void SystemControl::ctrlPIR() {
    if (toggleAuto){
        bool motionDetected = sensor.readPIR();

        if (motionDetected){
            light.turnOn();
            timerControl.startPIRTimer(PIRtimer);
        }
        else if (timerControl.getPIROverflow() == 0){
            Serial.println("Motion not detected");
        }

        if (!motionDetected && light.lightToggle){
            timerControl.handlePIROverflow();
        }
    }
}