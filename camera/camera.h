#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include <Adafruit_NeoPixel.h> 

esp_err_t camera_init();
void handleCapture();
void handleRoot();
void setup();
void loop();
