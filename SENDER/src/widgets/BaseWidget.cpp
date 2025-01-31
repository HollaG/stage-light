#include "BaseWidget.h"

// Constructor definition
BaseWidget::BaseWidget(int x, int y) : x(x), y(y) {}

void BaseWidget::updatePos(int x, int y)
{
    this->x = x;
    this->y = y;
}