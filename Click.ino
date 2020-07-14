#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <TwiFi.h>
#include <VariPass.h>

#include "Configuration.h"
#include "ConfigurationWiFi.h"
#include "ConfigurationLuna.h"
#include "ConfigurationVariPass.h"



/* PIN Definitions */
#define PIN_LED_R 12
#define PIN_LED_G 13
#define PIN_LED_B 14
#define PIN_RELAY 16

/* RGB LED Statuses */
#define LED_MODE_DIM  -1
#define LED_MODE_OFF  0
#define LED_MODE_ON   1
#define LED_MODE_FAIL 2

/* EEPROM Addresses */
#define EEPROM_DATA  2
#define EEPROM_STATE 3

/* Default Settings */
#define DEFAULT_STATE true

#define URL_RESULT_DONE 0
#define URL_RESULT_FAIL 1

/* Setup Functions */
void setupPins();
void setupSettings();

/* Processing Functions */
void processInterval();
void processRelay();

void setLED(int ledMode);
int openURL(String url);

void connectAttempt(int idEntry, int attempt);
void connectSuccess(int idEntry);
void connectFail(int idEntry);



/* Variables */
bool switchState;
bool blinking;



/* Setup Functions */
void setupPins() { 
    pinMode(PIN_RELAY, OUTPUT); 
    digitalWrite(PIN_RELAY, HIGH); 
    
    pinMode(PIN_LED_R, OUTPUT); 
    pinMode(PIN_LED_G, OUTPUT); 
    pinMode(PIN_LED_B, OUTPUT);    
}

void setupSettings() {    
    EEPROM.begin(512);
    int saved = EEPROM.read(EEPROM_DATA);
    EEPROM.end();

    if (saved == 1) {
        EEPROM.begin(512);
        switchState = (bool) EEPROM.read(EEPROM_STATE);  
        EEPROM.end();
    }
    else {   
        EEPROM.begin(512);
        EEPROM.write(EEPROM_DATA,  1);
        EEPROM.write(EEPROM_STATE, DEFAULT_STATE);  
        EEPROM.end();    
    }
}



/* Processing Functions */
void processInterval() {
    int result;
    bool setNew = false;

    if (VARIPASS_TYPE == "bool") {
        bool value = varipassReadBool(VARIPASS_KEY, VARIPASS_ID, &result);
    
        if (result == VARIPASS_RESULT_SUCCESS) {
            if ((!switchState && value) || (switchState && !value)) {
                switchState = value;
                setNew = true;
            }
        }
    }
    else if (VARIPASS_TYPE == "int") {
        int value = varipassReadInt(VARIPASS_KEY, VARIPASS_ID, &result);
        if (result == VARIPASS_RESULT_SUCCESS) {
            bool compared = value VARIPASS_COMPARATOR VARIPASS_VALUE;
            if ((!switchState && compared) || (switchState && !compared)) {
                switchState = compared;
                setNew = true;
            }
        }
    }
    else if (VARIPASS_TYPE == "float") {
        float value = varipassReadFloat(VARIPASS_KEY, VARIPASS_ID, &result);
        if (result == VARIPASS_RESULT_SUCCESS) {
            bool compared = value VARIPASS_COMPARATOR VARIPASS_VALUE;
            if ((!switchState && compared) || (switchState && !compared)) {
                switchState = compared;
                setNew = true;
            }
        }
    }

    if (setNew) {
        EEPROM.begin(512);
        EEPROM.write(EEPROM_DATA,  1);
        EEPROM.write(EEPROM_STATE, switchState);  
        EEPROM.end();
    }

    if (switchState)
        setLED(LED_MODE_ON);
    else
        setLED(LED_MODE_OFF);
    
    processRelay();
}

void processRelay() {
    if (switchState)
        digitalWrite(PIN_RELAY, LOW);
    else
        digitalWrite(PIN_RELAY, HIGH);
}

void setLED(int ledMode) {
    if (LED_ENABLE) {
        switch (ledMode) {
            case LED_MODE_DIM: 
                analogWrite(PIN_LED_R, 0);
                analogWrite(PIN_LED_G, 0);
                analogWrite(PIN_LED_B, 0);
                break;
            case LED_MODE_OFF: 
                analogWrite(PIN_LED_R, LED_MODE_OFF_R);
                analogWrite(PIN_LED_G, LED_MODE_OFF_G);
                analogWrite(PIN_LED_B, LED_MODE_OFF_B);
                break;
            case LED_MODE_ON: 
                analogWrite(PIN_LED_R, LED_MODE_ON_R);
                analogWrite(PIN_LED_G, LED_MODE_ON_G);
                analogWrite(PIN_LED_B, LED_MODE_ON_B);
                break;
            case LED_MODE_FAIL: 
                analogWrite(PIN_LED_R, LED_MODE_FAIL_R);
                analogWrite(PIN_LED_G, LED_MODE_FAIL_G);
                analogWrite(PIN_LED_B, LED_MODE_FAIL_B);
                break;            
        }
    }
}

int openURL(String url) {
    if (LUNA_DEBUG)
        Serial.println("Opening URL: " + String(LUNA_IP) + url);
        
    WiFiClient client;
    if (!client.connect(LUNA_IP, LUNA_PORT)) {  
        if (LUNA_DEBUG)
            Serial.println("Error connecting!");
        return URL_RESULT_FAIL;
    }

    client.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: " + LUNA_IP + "\r\n" + 
                 "Connection: close\r\n\r\n");
    client.stop();
    
    if (LUNA_DEBUG)
        Serial.println("Connection success.");

    return URL_RESULT_DONE;
}



void connectAttempt(int idEntry, int attempt) {
    if (attempt % 2) {
        setLED(LED_MODE_DIM);
    }
    else {
        if (switchState)
            setLED(LED_MODE_ON);
        else
            setLED(LED_MODE_OFF);
    }
}

void connectSuccess(int idEntry) {
}

void connectFail(int idEntry) {
    setLED(LED_MODE_FAIL);
}

void setup() {
    Serial.begin(9600);

    blinking = false;

    setupPins();
    setupSettings();
    
    processRelay();
        twifiInit(
        wifis,
        WIFI_COUNT,
        WIFI_HOST,
        WIFI_TIMEOUT,
        &connectAttempt,
        &connectSuccess,
        &connectFail,
        WIFI_DEBUG
        );
    twifiConnect(true);
    openURL(String(LUNA_URL_BOOT) + "&key=" + String(LUNA_KEY) + "&device=" + String(WIFI_HOST));
}

void loop() {
    if (!twifiIsConnected()) {
        twifiConnect(true);
    }
    else {
        processInterval();        
    }
    delay(1000 * POLLING_INTERVAL);
}
