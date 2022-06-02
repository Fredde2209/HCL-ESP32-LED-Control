# HCL-ESP32-LED-Control

- what does it do?
Human Centric Lighting
ESP32 with webserver 

- how does it work? 
warm white LED stripe controll circuit controlled by Pin 13 (ESP32)
cold white LED stripe controll circuit controlled by Pin 14 (ESP32)
-> recommended to use buck converter ciruit!

import provided folders to your PlatformIO project

enter WiFi data 

import librarys (if not done already). You will need the following:
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

upload file system image
upload code

DONE! 
