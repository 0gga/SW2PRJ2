#pragma once
#include <Arduino.h>

class Light{
	public:
	Light();

	void turnOn();
	void turnOff();
	void adjustBrightness(size_t level);
	void adjustAutoBrightness(size_t level);
	bool lightToggle;

	void ctrlLight(size_t PWM);
	size_t PWMmanual;
    size_t PWMauto;
};