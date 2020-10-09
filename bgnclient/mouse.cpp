#include "mouse.h"

Mouse::Mouse()
{
}

uint8_t Mouse::isMouseLeftButtonPressed()
{
    leftButPressed = QApplication::mouseButtons();
    return leftButPressed;
}

void Mouse::getPos()
{
    pos.x = QCursor::pos().x();
    pos.y = QCursor::pos().y();
}
