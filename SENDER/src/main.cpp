#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include <FastLED.h>

#include <time.h>

// #include "display/MenuScreen.cpp"
#include "display/BaseDisplay.h"
#include "controller/Controller.h"

// RECEIVER(s) MAC ADDRESS
uint8_t broadcastAddress[] = {0x30, 0xae, 0xa4, 0x6a, 0x30, 0xe0}; // 30:ae:a4:6a:30:e0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BUTTON_DOWN_PIN 13
#define BUTTON_UP_PIN 14
#define BUTTON_SEND_PIN 12
#define BUTTON_SCREEN_LEFT_PIN 4
#define BUTTON_SCREEN_RIGHT_PIN 2

#define RED_POT 35
#define GREEN_POT 34
#define BLUE_POT 32

#define NUM_LEDS 6
#define DATA_PIN 19

const char *ssid = "";
const char *password = "";

void readMacAddress()
{
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK)
    {
        Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                      baseMac[0], baseMac[1], baseMac[2],
                      baseMac[3], baseMac[4], baseMac[5]);
    }
    else
    {
        Serial.println("Failed to read MAC address");
    }
}

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

CRGB leds[NUM_LEDS];

// ----------- TUTORIAL SETUP
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

typedef struct change_message
{
    // light
    int light__type; // 0: static, etc
    int r;
    int g;
    int b;
    int w;  // not used
    int ww; // not used
    int light__transition;

    // motor
};

change_message changes[] = {

    {0, 255, 0, 0, 0, 0, 1}, // Light type: static, Red: 255, Transition: 1
    // Test Case 2
    {1, 0, 255, 0, 0, 0, 2}, // Light type: dynamic, Green: 255, Transition: 2
    // Test Case 3
    {2, 0, 0, 255, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0} // Light type: another, Blue: 255, Transition: 0
                          // Add more test cases as needed
};

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    // Serial.print("\r\nLast Packet Send Status:\t");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// MenuScreen* menuScreen;
BaseDisplay baseDisplay;
Controller controller(&baseDisplay);
void setup()
{
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_UP_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_SEND_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_SCREEN_LEFT_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_SCREEN_RIGHT_PIN, INPUT_PULLDOWN);
    pinMode(RED_POT, INPUT);
    pinMode(GREEN_POT, INPUT);
    pinMode(BLUE_POT, INPUT);

    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    delay(2000);
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text

    // wifi
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);
    // // Init ESP-NOW
    // if (esp_now_init() != ESP_OK)
    // {
    //     Serial.println("Error initializing ESP-NOW");
    //     return;
    // }

    // esp_now_register_send_cb(OnDataSent);

    // Register peer
    // memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    // peerInfo.channel = 0;
    // peerInfo.encrypt = false;

    // Add peer
    // if (esp_now_add_peer(&peerInfo) != ESP_OK)
    // {
    //     Serial.println("Failed to add peer");
    //     return;
    // }

    // add indicator leds
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    controller.refreshPage(&display);
}

int prevDownState = LOW;
int prevUpState = LOW;
int prevPrevState = LOW;
int prevNextState = LOW;
int prevSendState = LOW;

int prevR = 0;
int prevG = 0;
int prevB = 0;

bool changed = false;

int perfIndex = 0;

change_message randomCase;

void loop()
{
    int down = digitalRead(BUTTON_DOWN_PIN);
    int up = digitalRead(BUTTON_UP_PIN);
    int send = digitalRead(BUTTON_SEND_PIN);
    int screenLeft = digitalRead(BUTTON_SCREEN_LEFT_PIN);
    int screenRight = digitalRead(BUTTON_SCREEN_RIGHT_PIN);

    int red = map(analogRead(RED_POT), 0, 4095, 0, 255);
    int green = map(analogRead(GREEN_POT), 0, 4095, 0, 255);
    int blue = map(analogRead(BLUE_POT), 0, 4095, 0, 255);

    // Serial.printf("Red: %d, Green: %d, Blue: %d\n", red, green, blue);

    if (down != prevDownState)
    {
        prevDownState = down;

        changed = true;

        if (down == HIGH)
        {
            // int numChanges = (int)(sizeof(changes) / sizeof(changes[0]));
            // perfIndex = (perfIndex - 1 + numChanges) % numChanges;

            // button pressed
            controller.onDown();
        }
    }

    if (up != prevUpState)
    {
        prevUpState = up;

        changed = true;
        if (up == HIGH)
        {
            // int numChanges = sizeof(changes) / sizeof(changes[0]);
            // perfIndex = (perfIndex + 1) % (sizeof(changes) / sizeof(changes[0]));

            // button pressed
            controller.onUp();
        }
    }

    if (send != prevSendState)
    {
        prevSendState = send;

        changed = true;
        if (send == HIGH)
        {
            // button pressed
            controller.onSend();
        }
    }

    if (screenLeft != prevPrevState)
    {
        prevPrevState = screenLeft;

        changed = true;
        if (screenLeft == HIGH)
        {
            // button pressed
            controller.onScreenLeft();
        }
    }

    if (screenRight != prevNextState)
    {
        prevNextState = screenRight;

        changed = true;
        if (screenRight == HIGH)
        {
            // button pressed
            controller.onScreenRight();
        }
    }

    // if (abs(red - prevR) >= 5 || red == 0 || red == 255)
    // {
    //     prevR = red;
    //     changed = true;
    // }

    // TODO: find a better solution for this deboucning
    if (abs(red - prevR) >= 5 ||
        abs(green - prevG) >= 5 ||
        abs(blue - prevB) >= 5 || red == 0 || green == 0 || blue == 0 || red == 255 || green == 255 || blue == 255)
    {
        prevR = red;
        prevG = green;
        prevB = blue;

        changed = true;
    }

    if (changed)
    {   
        controller.updateLight(red, green, blue);
        Light displayLight = controller.getLight();
        

        // Serial.print("down = ");
        // Serial.print(down);
        // Serial.print(", up = ");
        // Serial.print(up);
        // Serial.println();
        // display.clearDisplay();

        // display.setCursor(0, 0);

        // display.print("down = ");
        // display.print(down);
        // display.print(", up = ");
        // display.print(up);

        // display.printf("\nCurrent index is %d", perfIndex);

        // display.println();

        // display.printf("Red: %d, Green: %d, Blue: %d", red, green, blue);

        // display.display();

        // baseDisplay.updateHomePage(&display, red, green, blue, perfIndex, 0, "");

        changed = false;

        // change_message randomCase = changes[perfIndex];
        // change_message randomCase = {
        //     .light__type = 0,
        //     .r = red,
        //     .g = green,
        //     .b = blue,
        //     .w = 0,
        //     .ww = 0,
        //     .light__transition = 0};
        // }
        // randomCase.light__type = 0;
        randomCase.r = red;
        randomCase.g = green;
        randomCase.b = blue;

        // update the colors
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB(displayLight.r, displayLight.g, displayLight.b);
        }

        FastLED.show();

        // Send message via ESP-NOW
        // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&randomCase, sizeof(randomCase));

        // if (result == ESP_OK)
        // {
        //     Serial.println("Sent with success");
        // }
        // else
        // {
        //     Serial.println("Error sending the data");
        // }
    }

    controller.refreshPage(&display);
    delay(30); // ~30fps
}