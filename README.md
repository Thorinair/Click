# Click
Click is a simple ESP8266 based power relay. By default, it is powered using mains current. Upon reading a valid value from VariPass, it turns the relay on, letting the power pass through and power whatever is on the other side.

## Build Instructions

Build the project using Arduino IDE using the following settings:

- **ESP8266 Package:** v2.7.4
- **Board:** Generic ESP8266 Module
- **Upload Config:**
    - Upload Speed: *115200*
    - Crystal Frequency: *26 MHz*
    - Debug port: *Disabled*
    - Flash Size: *4MB (FS:1MB OTA:~1019KB)*
    - Exceptions: *Legacy (new can return nullptr)*
    - Flash Frequency: *40MHz*
    - Flash Mode: *DIO*
    - IwIP Variant: *v2 Lower Memory*
    - Builtin Led: *13*
    - Debug Level: *None*
    - Reset Method: *dtr (aka nodemcu)*
    - Espressif FW: *nonos-sdk 2.2.1+100 (190703)*
    - SSL Support: *All SLL ciphers (most compatible)*
    - VTables: *Flash*
    - Erase Flash: *Only Sketch*
    - CPU Frequency: *80 MHz*