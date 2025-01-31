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