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

    void GET(AsyncWebServerRequest *request);                                                         // select a specific slot (in query params)
    void POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total); // create a specific slot
    // void PUT(AsyncWebServerRequest *request);  // update a specific slot
    void DELETE(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total); // delete a specific slot
};