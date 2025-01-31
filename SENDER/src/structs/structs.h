#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

struct Light {
    int light__type; // 0: static, etc
    int r;
    int g;
    int b;
    int w;  // not used
    int ww; // not used
    int light__transition;
};

struct HomePageData {
    Light light;
    int curIndex;
    int nextIndex;
    int connectedCount;
    std::string presetName;
};

struct Slot { 
    // no need for unique ID?
    Light light; 
};

struct Group { 
    Slot slots[100]; 
    // bool slotExists[100] = {false};
    char name[16];
    int slotCount;
};

#endif // STRUCTS_H