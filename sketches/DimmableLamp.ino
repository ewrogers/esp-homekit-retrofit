/*********************************************************************************
 *  DimmableLamp.ino
 *
 *  Copyright (c) 2023 Erik Rogers
 *
 *  https://github.com/ewrogers/esp-homekit-retrofit/sketches/DimmableLamp.ino
 *
 *  This example demonstrates how to create a dimmable lamp using HomeSpan.
 *
 *  It is a single accessory with a single service, a LightBulb.
 *  The LightBulb service has two characteristics, On and Brightness.
 *
 *  The On characteristic is a boolean value that turns the lamp on or off.
 *  The Brightness characteristic is an integer value that controls the brightness of the lamp.
 *
 *  The lamp is controlled by a PWM signal on a pin.
 *  The PWM signal is generated using the ESP32's LEDC peripheral, which is a hardware PWM generator.
 *
 *  The lamp is connected to pin 7, but you can change this to any pin you like.
 *
 *  It is assumed that the lamp output trigger signal is connected to a MOSFET (or relay) that is normally open.
 *
 ********************************************************************************/

#include "HomeSpan.h"         // include the HomeSpan library

#define PWM_CHANNEL 0           // PWM channel to use for the lamp
#define PWM_RESOLUTION 10       // PWM resolution in bits
#define PWM_FREQUENCY 5000      // PWM frequency in Hz
#define LAMP_PIN 7              // output pin for the lamp relay trigger signal

struct DimmableLamp : Service::LightBulb {

    int lampPin;                        // output in for the lamp relay trigger signal
    SpanCharacteristic *power;          // store a reference to the On Characteristic
    SpanCharacteristic *brightness;     // store a reference to the Brightness Characteristic

    DimmableLamp(int lampPin) : Service::LightBulb() {

        // The LightBulb service has two characteristics, On and Brightness.
        // The On characteristic is a boolean value that turns the lamp on or off.
        // The Brightness characteristic is an integer value that controls the brightness of the lamp.
        // The second argument indicates that the value should be saved/restored from non-volatile memory.
        power = new Characteristic::On(0, true);
        brightness = new Characteristic::Brightness(50, true);
        brightness->setRange(0, 100, 1);

        this->lampPin=lampPin;

        // configure the lamp pin as an output
        pinMode(lampPin, OUTPUT);

        // configure the PWM channel (ESP32)
        ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(lampPin, PWM_CHANNEL);

        // restore initial value on startup
        if (power->getVal() <= 0) {
            this->setBrightness(0);
        } else {
            this->setBrightness(brightness->getVal());
        }
    }

    // update() is called when the Home App requests a change to the lamp's state for either Characteristic
    // it returns true if the update was successful, false otherwise
    boolean update() {

        // get the new values for the On and Brightness Characteristics
        int powerValue = power->getNewVal();
        int brightnessValue = brightness->getNewVal();

        // if the lamp is off, set the brightness to 0
        if (powerValue <= 0) {
            this->setBrightness(0);
            return (true);
        }

        this->setBrightness(brightnessValue);
        return (true);
    }

    void setBrightness(int percentage) {
        if (percentage <= 0) {
            ledcWrite(PWM_CHANNEL, 0);
            return;
        }

        // convert the brightness value from a percentage to a value between 0 and 255
        int dutyCycle = (int) (percentage / 100.0 * 255.0);

        // set the apparent brightness of the lamp via PWM duty cycle
        ledcWrite(PWM_CHANNEL, dutyCycle);
    }
};

void setup() {

    Serial.begin(115200);

    homeSpan.begin(Category::Lighting, "Dimmable Lamp");

    // create a new Accessory with the required service and identify characteristics
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
    new Characteristic::Name("Dimmable Lamp");

    new DimmableLamp(LAMP_PIN);
}

void loop() {
    homeSpan.poll();
}
