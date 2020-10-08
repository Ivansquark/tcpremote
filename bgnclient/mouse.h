#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <QCursor>

class Mouse
{
public:
    Mouse();
    struct Pos {
        uint16_t x=0;
        uint16_t y=0;
    };
    uint8_t leftButPressed = 0;
    Pos pos;
    /*!< записываем позицию курсора >*/
    void getPos();
};

#endif // MOUSE_H
