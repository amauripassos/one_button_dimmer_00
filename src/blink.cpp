#include "blink.hpp"
#include "mbed.h"

void blink(){
    myleds[blinkld]=!myleds[blinkld];
}