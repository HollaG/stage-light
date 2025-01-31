#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
class BaseScreen
{
public:
    int startX;
    int startY;
    Adafruit_SSD1306* display;  // ✅ Store a pointer

    virtual void draw() = 0;

    BaseScreen(int x, int y, Adafruit_SSD1306* display)  // ✅ Pass pointer
    {
        startX = x;
        startY = y;
        this->display = display;
    }
};