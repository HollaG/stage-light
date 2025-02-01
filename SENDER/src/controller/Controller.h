// Controls everything that happens. Has knowledge of all relevant data in the app.
#include "display/BaseDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h> // save
#include "FS.h"
#include <LittleFS.h>
#include "structs/structs.h"

class Controller
{
    Preferences prefs;
    BaseDisplay *baseDisplay;

    Light light = {0, 0, 0, 0, 0, 0}; // light used in EDITING
    // when we're not in EDIT mode, show the light from preset

    // TODO decide if we want to do this
    Light frozenLight = {0, 0, 0, 0, 0, 0}; // When in saving screen, we should disable any modifications

    Page currentPage = HOME_PAGE;

    // always init to R:0 G:0 B:0
    int slotIndex = 0;
    int groupIndex = 0;
    int groupCount = 0;

    Group groups[20];
    bool groupExists[20] = {false};
    // we can have up to 20 "loadouts" of 100 slots each

    // 0 "home"
    // 1 "save_slot"
    // 2 "new_preset"

    int connectedCount = 0;

    int mode = 0; // 0 = "locked, no edit", 1 = "edit"

    int saveInSlotIndex = -1;
    int saveInGroupIndex = -1;
    bool isInsert = false;

    // Loading mode
    bool isLoading = false;
    char loadingLabel[16];
    int loadingIndex = 0;

    TaskHandle_t saveTaskHandle = NULL;
public:
    Controller(BaseDisplay *baseDisplay);

    void updateLight(int red, int green, int blue);
    Light getLight();

    void nextSlot();

    void prevSlot();

    void refreshPage(Adafruit_SSD1306 *display);

    // BUTTON ACTIONS
    void onScreenLeft();
    void onScreenRight();
    void onSend();
    void onDown();
    void onUp();


    void save();
    void load();

    void changePage(Page page);


private: 
    void backgroundSave();
    void backgroundLoad();

    static void saveHelper(void *parameter);
    static void loadHelper(void *parameter);
};
