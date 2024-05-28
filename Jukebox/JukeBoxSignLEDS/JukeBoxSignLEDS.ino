/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include "Arduino_GigaDisplay_GFX.h"

GigaDisplay_GFX tft;
SensirionI2CScd4x scd4x;

#define GC9A01A_CYAN    0x07FF
#define GC9A01A_RED     0xf800
#define GC9A01A_BLUE    0x001F
#define GC9A01A_GREEN   0x07E0
#define GC9A01A_MAGENTA 0xF81F
#define GC9A01A_WHITE   0xffff
#define GC9A01A_BLACK   0x0000
#define GC9A01A_YELLOW  0xFFE0
float oldco2;
float oldhum;
float oldtemp;
float temperature2;

void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

void setup() {

    Serial.begin(115200);


    Wire.begin();
    Wire1.begin();
    uint16_t error;
    char errorMessage[256];

    scd4x.begin(Wire);
    tft.begin();
    //tft.setRotation(2); //uncomment this line to rotate 180 //recomment to rotate again
    tft.fillScreen(GC9A01A_BLACK);
    tft.setTextColor(GC9A01A_RED);  tft.setTextSize(6);
    tft.setCursor(180, 0);
    tft.print("LCTI");
    tft.setTextColor(GC9A01A_BLUE);  tft.setTextSize(6);
    tft.setCursor(35, 50);
    tft.print("ELECTRONICS");
    tft.setTextColor(GC9A01A_MAGENTA);  tft.setTextSize(6);
    tft.setCursor(0, 750);
    tft.print("Jacob Goodwin");
    tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(5);
        tft.setCursor(180, 100);
        tft.println("CO2: ");
        tft.setCursor(160, 210);
        tft.print("Temp: ");
        tft.setCursor(345, 265); tft.setTextSize(3);
        tft.print((char)247);
        tft.setCursor(375, 270); tft.setTextSize(5);
        tft.print("F");
        tft.setCursor(180, 330);
        tft.print("Hum: ");
        tft.setCursor(320, 390);
        tft.print(" %");
    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        printSerialNumber(serial0, serial1, serial2);
    }

    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println("Waiting for first measurement... (5 sec)");
}

void loop() {
    uint16_t error;
    char errorMessage[256];

    delay(100);

    // Read Measurement
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        Serial.print("Error trying to execute getDataReadyFlag(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }
    if (!isDataReady) {
        return;
    }
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        temperature2 = temperature * 9/5 + 32;
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
        tft.setTextColor(GC9A01A_BLACK);  tft.setTextSize(7);
        tft.setCursor(166, 150);
        tft.print(oldco2);
        tft.setCursor(130, 260);
        tft.print(oldtemp);
        tft.setCursor(120, 380);
        tft.print(oldhum);
        tft.setTextColor(GC9A01A_WHITE);  tft.setTextSize(7);
        tft.setCursor(166, 150);
        tft.print(co2);
        tft.setCursor(130, 260);
        tft.print(temperature2);
        tft.setCursor(120, 380);
        tft.print(humidity);


        oldco2 = co2;
        oldhum = humidity;
        oldtemp = temperature2;
    }
}
