#pragma once

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "controller/Controller.h"

class ServerController
{
public:
  ServerController(Controller &controller);

  void registerServer();

private:
  AsyncWebServer server;
  Controller controller;
};