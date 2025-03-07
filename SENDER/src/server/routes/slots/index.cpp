#include "index.h"
#include "ArduinoJson.h"

void SlotsRoute::GET(AsyncWebServerRequest *request)
{
    if (request->hasParam("select"))
    {
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

/**
 * Body:
 * {
 *   "groupIndex": number,
 *   "slotIndex": number,
 *   "slot": Slot
 *
 * }
 **/
void SlotsRoute::POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    Serial.println("[webserver] Adding slot...");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, (char *)data);
    if (!error)
    {
        // Validation: all keys must be present
        if (!doc.containsKey("groupIndex") || !doc.containsKey("slotIndex") || !doc.containsKey("slot"))
        {
            request->send(400, "application/json", "{\"error\": \"Missing keys in request body.\"}");
            return;
        }

        int groupIndex = doc["groupIndex"];
        int slotIndex = doc["slotIndex"];

        int r = doc["slot"]["light"]["r"];
        int g = doc["slot"]["light"]["g"];
        int b = doc["slot"]["light"]["b"];

        int w = doc["slot"]["light"]["w"];   // not used
        int ww = doc["slot"]["light"]["ww"]; // not used

        int light__type = doc["slot"]["light"]["light__type"];
        int light__transition = doc["slot"]["light"]["light__transition"];

        Light light = {light__type, r, g, b, w, ww, light__transition};
        Slot slot = {light}; // TODO: have ID for slot

        Serial.printf("[webserver] Saving R: %d, G: %d, B: %d in group index %d at slot index %d\n", r, g, b, groupIndex, slotIndex);

        this->serverController->controller->createSlot(slot, groupIndex, slotIndex);
        // Group *groups = this->serverController->controller->groups;
        // groups[groupIndex].slots[groups[groupIndex].slotCount] = slot;
        // groups[groupIndex].slotCount = groups[groupIndex].slotCount + 1;
    }
    else
    {
        Serial.println(error.c_str());
    }

    request->send(200, "application/json", "{\"message\": \"Changes applied.\"}");
}

/**
 * Body:
 * {
 *   "groupIndex": number,
 *   "slotIndex": number,
 * }
 **/
void SlotsRoute::DELETE(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    Serial.println("[webserver] Deleting slot...");
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, (char *)data);
    if (!error)
    {
        // Validation: all keys must be present
        if (!doc.containsKey("groupIndex") || !doc.containsKey("slotIndex"))
        {
            request->send(400, "application/json", "{\"error\": \"Missing keys in request body.\"}");
            return;
        }

        int groupIndex = doc["groupIndex"];
        int slotIndex = doc["slotIndex"];

        this->serverController->controller->deleteSlot(groupIndex, slotIndex);

        request->send(200, "application/json", "{\"message\": \"Slot deleted.\"}");
    }
}
