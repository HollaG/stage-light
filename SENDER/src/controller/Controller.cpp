#include "Controller.h"


void Controller::updateLight(int red, int green, int blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void Controller::refreshPage(Adafruit_SSD1306* display)
{
    // depending on the current page index, refresh the page by calling the relevant functions
    switch (currentPageIndex)
    {
    case 0:
    {
        baseDisplay->updateHomePage(display, red, green, blue, 0, 0, "");
        break;
    };

    default:
    {
    }
    }
}