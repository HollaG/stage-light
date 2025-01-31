#include <string>
#include "BaseScreen.cpp"
class MenuScreen : public BaseScreen
{
public:
    int curIndex;
    int nextIndex;
    std::string presetName;
    int connectedCount;

    MenuScreen(int x, int y, Adafruit_SSD1306* display)  // ✅ Pass pointer
        : BaseScreen(x, y, display), curIndex(0), nextIndex(0), connectedCount(0) {}

    virtual void draw()
    {
        display->clearDisplay();
        display->setCursor(startX, startY);
        display->printf("%d -> %d", curIndex, nextIndex);
        // draw horizontal 1px line
        display->drawFastHLine(0, startY + 10, 128, SSD1306_WHITE);
        display->display();
    }
};