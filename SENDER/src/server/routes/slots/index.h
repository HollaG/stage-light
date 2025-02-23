#pragma once
#include "server/routes/base.h"
#include "server/ServerController.h"

class SlotsRoute : public BaseRoute
{
public:
    explicit SlotsRoute(ServerController *serverController) : BaseRoute(serverController) {}

    std::string getRoute()
    {
        return "/slots";
    }

    void GET(AsyncWebServerRequest *request); // select a specific slot (in query params)
    // void POST(AsyncWebServerRequest *request); // create a specific slot
    // void PUT(AsyncWebServerRequest *request);  // update a specific slot
    // void DELETE(AsyncWebServerRequest *request); // delete a specific slot

};