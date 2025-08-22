#pragma once
#include <Arduino.h>

class Sensor {
public:
	Sensor();
	bool readPIR();
	size_t readFLED();
	
private:
	size_t FLEDraw;  // Raw analog reading from FLED sensor
	size_t PIRraw;
};