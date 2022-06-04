
// ESP32 - Webserver für LED Steuerung
// Autor: Frederik Poeppel
// Datum: 05.05.2022

// SEKTION 1: Bibliothekten

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// SEKTION 2: Variablen

// WLAN Initialisierungsdaten: 
const char* ssid = "MY SSID";
const char* password = "MY PASSWORD";

// Sonstige Variablen
String pwmSliderValue = "100";
const char* PWM_INPUT = "value";

int pinWarmWhite = 13;
int intensityWarmWhite = 128;
int pinColdWhite = 14;
int intensityColdWhite = 128;

// WebServer an Port 80 starten
AsyncWebServer server(80);

// Variable für EIN/AUS
bool lampOn = false;

// SEKTION 3: Methoden

// Methode für LED Farbe Setzen
void setLEDs(int pWarmWhite, int pColdWhite)
{
    analogWrite(pinWarmWhite, pWarmWhite);
    analogWrite(pinColdWhite, pColdWhite);
}

// Methode für Slider-Update 
String updateSlider(const String& var)
{
  if (var == "SLIDERVALUE")
  {
    return pwmSliderValue;
  }
  return String();
}

// SEKTION 4: Setup-Methode

void setup()
{
    // Starte Serielle Schnittstelle für Debugging
    Serial.begin(115200);

    // initialisiere Output Pins
    pinMode(pinColdWhite, OUTPUT);
    pinMode(pinWarmWhite, OUTPUT);
    analogWrite(pinColdWhite, 0);
    analogWrite(pinWarmWhite, 0);

    // Initialisiere SPIFFS
    if(!SPIFFS.begin(true))
    {
        Serial.println("Es gibt einen Fehler beim einbinden von SPIFFS!");
        return;
    }

    // Mit WLAN verbinden
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Mit WLAN " + String(ssid) + " verbinden...");
    }

    // Lokale IP - Adresse ausgeben
    Serial.println(WiFi.localIP());

    // Webseite  HTML an Client senden
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.html", String(), false, updateSlider);
    });

    // Webseite CSS an Client senden
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.css", "text/css");
    });

    // HTTP-GET Anfragen vom Webserver
    // ON Button
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // BUTTON ON/OFF Gedrückt 
        Serial.println("Message ON/OFF recieved!");
        if(lampOn == false)
        {
            lampOn = true;
            setLEDs(intensityWarmWhite, intensityColdWhite);
            Serial.println("LAMP ON");
        }
        else
        {
            lampOn = false;
            setLEDs(0, 0);
            Serial.println("LAMP OFF");
        } 
        request->send(SPIFFS, "/index.html", String(), false, updateSlider);
    });
    // Button 1
    server.on("/1", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // BUTTON 1 Gedrückt
        Serial.println("Button 1 gedrückt");
        intensityWarmWhite = 235;
        intensityColdWhite = 20;
        setLEDs(intensityWarmWhite, intensityColdWhite);
        request->send(SPIFFS, "/index.html", String(), false, updateSlider);
    });
    // Button 2
    server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // BUTTON 2 Gedrückt 
        Serial.println("Button 2 gedrückt");
        intensityWarmWhite = 141;
        intensityColdWhite = 114;
        setLEDs(intensityWarmWhite, intensityColdWhite);
        request->send(SPIFFS, "/index.html", String(), false, updateSlider);
    });
    // Button 3
    server.on("/3", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // BUTTON 3 Gedrückt 
        Serial.println("Button 3 gedrückt");
        intensityWarmWhite = 47;
        intensityColdWhite = 208;
        setLEDs(intensityWarmWhite, intensityColdWhite);
        request->send(SPIFFS, "/index.html", String(), false, updateSlider);
    });
    // Slider
    server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String inputFromSlider;
        // Slider bewegt
        if (request->hasParam(PWM_INPUT)) 
        {
            inputFromSlider = request->getParam(PWM_INPUT)->value();
            pwmSliderValue = inputFromSlider;
        }
        Serial.println(inputFromSlider);
        
        setLEDs((intensityWarmWhite / 100) * inputFromSlider.toInt(), (intensityColdWhite / 100) * inputFromSlider.toInt());

        request->send(SPIFFS, "/index.html");
    });

    // Start server
    server.begin();

}

// SEKTION 5: wird nicht benutzt

void loop(){}
