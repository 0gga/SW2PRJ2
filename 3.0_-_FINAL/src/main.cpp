#include <Arduino.h>
#include "sensor.h"
#include "light.h"
#include "timerControl.h"
#include "systemControl.h"

// A0 / PF0 = FLED
// 7 / PH4 = PIR
// 6 / PH3 = LightSource

Sensor sensor;
Light light;
SystemControl systemControl;
TimerControl timerControl;

void handleUARTCommand() {
    if (Serial.available()) {
        String receivedString = Serial.readStringUntil('\n');
        receivedString.trim();

        if (receivedString.length() > 0) {
            String input = receivedString;
            if (input.startsWith("test")) {
                light.turnOff();
                Serial.println("PWM disabled on PH2");
            }

            if (input.startsWith("on")) {
                light.turnOn();
                Serial.println("PWM enabled on PH2");
            }

            else if (input.startsWith("off")) {
                light.turnOff();
                Serial.println("PWM disabled on PH2");
            }

            else if (input.startsWith("manual_on")) {
                Serial.println("System has been set to manual");
                systemControl.autoModeOff();
            }

            else if (input.startsWith("manual_off")) {
                Serial.println("System has been set to auto");
                systemControl.autoModeOn();
            }

            else if (input.startsWith("brightness:")) { //brightness:100
                size_t level = input.substring(11).toInt();
                Serial.println("Brightness has been adjusted to " + String(level) + "%");
                light.PWMmanual = level;
            }

            else if (input.startsWith("autoBrightness:")) { //autoBrightness:100 autoBrightness:50
                size_t level = input.substring(15).toInt();
                Serial.println("Brightness for automode has been adjusted to " + String(level) + "%");
                light.PWMauto = level;
            }

            else if (input.startsWith("autoModeTimer:")) { //autoModeTimer:0
                size_t level = input.substring(16).toInt();
                Serial.println("Automode will reactivate in " + String(level) + " minutes");
                systemControl.setAutoModeTimer(level);
            }

            else if (input.startsWith("pir_timeout:")) { //pir_timeout:1
                if (!systemControl.toggleAuto){
                    Serial.println("PIR is currently deactivated");
                }
                size_t delay = input.substring(12).toInt();
                systemControl.setPIRtimer(delay);
                Serial.println("PIR timeout has been set to " + String(delay) + " minutes");
            }

            else {
                Serial.println("Invalid command");
            }
        }
    }
}

void setup() {
    Serial.begin(115200);  // Initialize Serial at 115200 baud
    sei();
}

unsigned long lastSensorRead = 0;
const unsigned long SENSOR_READ_INTERVAL = 10; // 0.01 second

void loop() {
    handleUARTCommand();
    
    if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = millis();
        
        if(systemControl.toggleAuto){
        Serial.println("\n--- FLED readings ---");        
        systemControl.ctrlFLED();
        Serial.println("\n--- PIR readings ---");
        systemControl.ctrlPIR();
        }
        
        Serial.println("\n--- AutoMode Timer Readings ---");
        timerControl.handleAutoModeOverflow();
    }
}