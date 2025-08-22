#pragma once
#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"

class SystemControl {
public:
    SystemControl();

    void autoModeOn();
    void autoModeOff();

    void ctrlFLED();
    void ctrlPIR();

    void setPIRtimer(size_t timer);
    void setAutoModeTimer(size_t timer);
    
    bool toggleAuto;
    size_t PIRtimer;
    size_t autoModeTimer;
};