#pragma once
#include <Arduino.h>

class TimerControl {
public:
    // Timer1 Channel A - Auto mode revert timer
    static void startAutoModeTimer(unsigned long minutes);
    static void handleAutoModeOverflow();
    
    // Timer1 Channel B - PIR timeout timer
    static void startPIRTimer(unsigned long minutes);
    static void handlePIROverflow();

    unsigned long getPIROverflow();
    unsigned long getPIRTargetOverflow();

private:
    static volatile unsigned long autoModeOverflowCount;
    static volatile unsigned long autoModeTargetOverflows;
    
    static volatile unsigned long pirOverflowCount;
    static volatile unsigned long pirTargetOverflows;
};