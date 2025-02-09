#include "BasePage.h"

// Constructor
BasePage::BasePage()
{
    // Initialize the widget array to NULL
    for (int i = 0; i < 16; i++)
    {
        widgets[i] = NULL;
    }
}

// Draw method: Calls draw() on each non-null widget
void BasePage::draw(Adafruit_SSD1306 *display)
{
    for (int i = 0; i < 16; i++)
    {
        if (widgets[i] != NULL)
        {
            widgets[i]->draw(display);
        }
    }
}

// Add a widget to the array
void BasePage::addWidget(BaseWidget *widget)
{
    for (int i = 0; i < 16; i++)
    {
        if (widgets[i] == NULL)
        {
            widgets[i] = widget;
            break;
        }
    }
}
