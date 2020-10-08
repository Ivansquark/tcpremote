#include "mouse.h"

Mouse::Mouse()
{

}

void Mouse::getPos()
{
    pos.x = QCursor::pos().x();
    pos.y = QCursor::pos().y();
}
