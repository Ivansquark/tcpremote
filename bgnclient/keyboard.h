#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

class Keyboard
{
public:
    Keyboard();
    uint8_t whatKeyPressed();
    uint8_t keyPressedNumber=0;
};

#endif // KEYBOARD_H
