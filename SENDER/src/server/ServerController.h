#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "controller/Controller.h"

class ServerController
{
public:
  ServerController();
  void begin(Controller *controller);
  void registerRoutes();

private:
  AsyncWebServer server;
  Controller *controller;
};
