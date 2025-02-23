#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "controller/Controller.h"


#include "server/routes/index.h" // /
#include "server/routes/groups/index.h" // /groups

class ServerController
{
public:
  ServerController();
  void begin(Controller *controller);
  void registerRoutes();

  AsyncWebServer server;
  Controller *controller;
  
private:

  // route registrtation callbacks

  // GET /
  // void GET_home(AsyncWebServerRequest *request);

  // // GET /groups
  // void GET_groups(AsyncWebServerRequest *request);

  // // POST /groups/ --> create new group (optionally specify group id)
  // void POST_groups(AsyncWebServerRequest *request);

  // // GET /groups/:id
  // void GET_groups_id(AsyncWebServerRequest *request);

  // // DELETE /groups/:id --> delete group
  // void DELETE_groups_id(AsyncWebServerRequest *request);

  // // POST /groups/:id/slots --> create new slot (optionally specify slot id)
  // void POST_groups_id_slots(AsyncWebServerRequest *request);

  // // DELETE /groups/:id/:slotId --> delete slot
  // void DELETE_groups_id_slotId(AsyncWebServerRequest *request);
};
