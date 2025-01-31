// Controls everything that happens. Has knowledge of all relevant data in the app.
#include "display/BaseDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Controller
{

    BaseDisplay *baseDisplay;

    int currentPageIndex = 0; // hardcoded for now

    int red;
    int green;
    int blue;

    // 0 "home"
    // 1 "save_slot"
    // 2 "new_preset"
public:
    Controller(BaseDisplay *baseDisplay) : baseDisplay(baseDisplay) {}

    void updateLight(int red, int green, int blue)
    {
    }

    void refreshPage(Adafruit_SSD1306 *display)
    {
    }
};