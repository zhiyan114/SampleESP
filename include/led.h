#include <stdio.h>


// Available LED Type for operation (This is a bitwise flag)
enum LED_Type {
    wifiLED = 1 << 0,
    sensorLED = 1 << 1,
};

void led_init();
void setLedStatus(int LedType, bool enabled);