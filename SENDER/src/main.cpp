#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FastLED.h>

#include <time.h>

// #include "display/MenuScreen.cpp"
#include "display/BaseDisplay.h"
#include "controller/Controller.h"
#include "server/ServerController.h"

#define REFORMAT true
#define FORMAT_LITTLEFS_IF_FAILED true

#include "LittleFS.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>

// RECEIVER(s) MAC ADDRESS
// uint8_t broadcastAddress[] = {0x30, 0xae, 0xa4, 0x6a, 0x30, 0xe0}; // 30:ae:a4:6a:30:e0
// uint8_t broadcastAddress[] = {0x94, 0x54, 0xc5, 0x75, 0x7a, 0x38}; // 94:54:c5:75:7a:38
uint8_t broadcastAddress1[] = {0x24, 0xd7, 0xeb, 0xee, 0xae, 0xf9}; // 24:d7:eb:ee:ae:f9 4m strip
uint8_t broadcastAddress2[] = {0x24, 0xd7, 0xeb, 0xee, 0xdc, 0x95}; // 24:d7:eb:ee:dc:95

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BUTTON_DOWN_PIN 14
#define BUTTON_UP_PIN 2
#define BUTTON_SEND_PIN 4
#define BUTTON_SCREEN_LEFT_PIN 13
#define BUTTON_SCREEN_RIGHT_PIN 12

#define RED_POT 32
#define GREEN_POT 35
#define BLUE_POT 34

#define NUM_LEDS 6
#define DATA_PIN 33

// Web Server for control
const char *ssid = "ESP-NOW";
const char *password = "esp32controller";
// const char *ssid = "SohFamily";
// const char *password = "Soh14185";
/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);

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
// ServerController serverController(controller);

ServerController serverController;

void setup()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    // NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

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
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    // --------------------------
    // WiFi setup (AP mode)
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    // WiFi setup (STA mode)
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     Serial.print(".");
    //     delay(100);
    // }

    // Serial.println("\nConnected to the WiFi network");
    // Serial.print("Local ESP32 IP: ");
    // Serial.println(WiFi.localIP());
    // --------------------------

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    // Register peer1
    memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    // Register peer2
    memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    // Add indicator LEDs
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    controller.load();

    // Start the web server using the ServerController class
    serverController.begin(&controller);

    Serial.println("Setup completed");
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
    controller.refreshPage(&display);
    delay(30); // ~30fps

    int down = digitalRead(BUTTON_DOWN_PIN);
    int up = digitalRead(BUTTON_UP_PIN);
    int send = digitalRead(BUTTON_SEND_PIN);
    int screenLeft = digitalRead(BUTTON_SCREEN_LEFT_PIN);
    int screenRight = digitalRead(BUTTON_SCREEN_RIGHT_PIN);

    int red = 255 - map(analogRead(RED_POT), 0, 4095, 0, 255);
    int green = 255 - map(analogRead(GREEN_POT), 0, 4095, 0, 255);
    int blue = 255 - map(analogRead(BLUE_POT), 0, 4095, 0, 255);

    // Serial.printf("Red: %d, Green: %d, Blue: %d\n", red, green, blue);
    // Serial.printf("Down: %d, Up: %d, Send: %d, ScreenLeft: %d, ScreenRight: %d\n", down, up, send, screenLeft, screenRight);

    if (down != prevDownState)
    {
        prevDownState = down;

        changed = true;

        if (down == HIGH)
        {
            // int numChanges = (int)(sizeof(changes) / sizeof(changes[0]));
            // perfIndex = (perfIndex - 1 + numChanges) % numChanges;

            // button pressed
            Serial.println("[debug] Down Button Pressed");
            controller.onDown();

            if (send != HIGH)
            {
                // button pressed
                // controller.onSend();
                Serial.println("[debug] Send Button Pressed");
                Light displayLight = controller.getLight();
                // Send message via ESP-NOW
                esp_err_t result = esp_now_send(0, (uint8_t *)&displayLight, sizeof(displayLight));

                if (result == ESP_OK)
                {
                    Serial.println("Sent with success");
                }
                else
                {
                    Serial.println("Error sending the data");
                }
            }
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
            Serial.println("[debug] Up Button Pressed");
            controller.onUp();

            if (send != HIGH)
            {
                // button pressed
                // controller.onSend();
                Serial.println("[debug] Send Button Pressed");

                // Send message via ESP-NOW
                Light displayLight = controller.getLight();

                esp_err_t result = esp_now_send(0, (uint8_t *)&displayLight, sizeof(displayLight));

                if (result == ESP_OK)
                {
                    Serial.println("Sent with success");
                }
                else
                {
                    Serial.println("Error sending the data");
                }
            }
        }
    }

    if (send != prevSendState)
    {
        prevSendState = send;

        changed = true;
        if (send == LOW)
        {
            // button pressed
            controller.onSend();
            Serial.println("[debug] Send Button Pressed");

            // Send message via ESP-NOW
        }
    }

    if (screenLeft != prevPrevState)
    {
        prevPrevState = screenLeft;

        changed = true;
        if (screenLeft == HIGH)
        {
            // button pressed
            Serial.println("[debug] Screen Left Button Pressed");
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
            Serial.println("[debug] Screen Right Button Pressed");
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
}