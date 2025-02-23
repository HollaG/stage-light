#include "index.h"

void SlotsRoute::GET(AsyncWebServerRequest *request) {
    if (request->hasParam("select")) { 
        String changeToSlotId = request->getParam("select")->value();

        // try to convert to integer
        int slotId = changeToSlotId.toInt();

        // if conversion fails, return 500
        // TODO: data validation

        this->serverController->controller->changeSlot(slotId);

        // send back 200 OK
        request->send(200, "application/json", "{\"message\": \"Slot changed\"}");


    }
}