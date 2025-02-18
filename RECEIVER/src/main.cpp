/*
 Rui Santos & Sara Santos - Random Nerd Tutorials
 Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <Arduino.h>
#include <espnow.h>
#include <esp8266wifi.h>
#include <structs/structs.h>

#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

CHANGE_MESSAGE change;

// callback function that will be executed when data is received

int r, g, b;
bool hasChanged = true;
LightType lightType = LightType::STATIC;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&change, incomingData, sizeof(change));

  Serial.printf("Random Test Case:\n");
  Serial.printf("  Light Type: %d\n", change.light__type);
  Serial.printf("  R: %d, G: %d, B: %d\n", change.r, change.g, change.b);
  Serial.printf("  Light Transition: %d\n", change.light__transition);
  Serial.printf("  W: %d, WW: %d\n", change.w, change.ww);

  r = change.r;
  g = change.g;
  b = change.b;

  lightType = change.light__type;

  hasChanged = true;

  // memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);
  // Serial.print("Int: ");
  // Serial.println(myData.b);
  // Serial.print("Float: ");
  // Serial.println(myData.c);
  // Serial.print("Bool: ");
  // Serial.println(myData.d);
  Serial.println();
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}
void staticLight()
{
  for (int i = 0; i < NUM_LEDS / 3; i++)
  {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

void chaseLight()
{
  for (int i = 0; i < NUM_LEDS / 3; i = i + 1)
  {
    leds[i] = CRGB(r, g, b);
    leds[i + 1 % (NUM_LEDS / 3)] = CRGB(r, g, b);
    leds[i + 2 % (NUM_LEDS / 3)] = CRGB(r, g, b);
    FastLED.show();
    delay(60);
    leds[i] = CRGB::Black;
    leds[i + 1 % (NUM_LEDS / 3)] = CRGB::Black;
    leds[i + 2 % (NUM_LEDS / 3)] = CRGB::Black;
  }
}
void loop()
{
  switch (lightType)
  {
  case LightType::STATIC:
    staticLight();
    break;
  case LightType::CHASE:
    chaseLight();
    break;
  }
  // leds[0] = CRGB::Red;
  // FastLED.show();
  // delay(30);

  // if (hasChanged)
  // {
  //   hasChanged = false;

  // update the colors
  // for (int i = 0; i < NUM_LEDS / 3; i++)
  // {
  //   leds[i] = CRGB(r, b, g);
  // }

  // chase
  // for (int i = 0; i < NUM_LEDS / 3; i = i + 1)
  // {
  //   leds[i] = CRGB(r, g, b);
  //   leds[i + 1 % (NUM_LEDS / 3)] = CRGB(r, g, b);
  //   leds[i + 2 % (NUM_LEDS / 3)] = CRGB(r, g, b);
  //   FastLED.show();
  //   delay(60);
  //   leds[i] = CRGB::Black;
  //   leds[i + 1 % (NUM_LEDS / 3)] = CRGB::Black;
  //   leds[i + 2 % (NUM_LEDS / 3)] = CRGB::Black;

  // for (int j = i; j < i + 3; j++)
  // {
  //   leds[j] = CRGB(r, g, b);
  // }

  // FastLED.show();

  // for (int j = i; j < i + 3; j++)
  // {
  //   leds[j] = CRGB::Black;
  // }

  // delay(30);
  // }

  // FastLED.show();
  // };
}
