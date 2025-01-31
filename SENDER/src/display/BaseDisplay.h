#pragma once
#include "pages/BasePage.h"
#include "pages/HomePage/HomePage.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class BaseDisplay
{
public:
    HomePage homePage;

    // Constructor
    BaseDisplay();

    // Home page controls
    void showHomePage(Adafruit_SSD1306 *display);
    void updateHomePage(Adafruit_SSD1306 *display, int red, int green, int blue, int index, int connectedCount, std::string presetName);
    


    void showXXPage();
    void showYYPage();
};
