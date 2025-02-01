#include "widgets/BaseWidget.h"
#include "widgets/Save/SaveMenuItem/SaveMenuItem.h"
class CommonMenu : public BaseWidget {

    SaveMenuItemWidget* prevItem;
    SaveMenuItemWidget* currentItem;
    SaveMenuItemWidget* nextItem;
};