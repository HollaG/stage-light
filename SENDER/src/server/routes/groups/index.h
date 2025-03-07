#pragma once

#include "server/routes/base.h"
#include "server/ServerController.h"

class GroupsRoute : public BaseRoute
{
public:
    explicit GroupsRoute(ServerController *serverController);

    virtual std::string getRoute() override;
    virtual void GET(AsyncWebServerRequest *request) override;
    virtual void POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override;

    void GET_all(AsyncWebServerRequest *request);
    void GET_by_id(AsyncWebServerRequest *request);
    void GET_change_group(AsyncWebServerRequest *request); // ?select={index}
};
