#include <main.h>

// Function prototypes
bool toggleWifi();

bool wifiState = false;
bool reached = false;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0; // Variable to store the last time the loop ran
const unsigned int delayInterval = 100; // Delay interval in milliseconds
//Blinker blinker;

#define NUM_LEDS 2
CRGB leds[NUM_LEDS];

void setup(void) {
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(leds, 2);

    Serial.begin(115200);
    Serial.println("Booting");
    // Initialize CAN bus communication
    CAN.reset();
    CAN.setBitrate(CAN_500KBPS, MCP_8MHZ);
    CAN.setNormalMode();
    tacho.attach(D1);
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Green;
    FastLED.show();
//    blinker.test();
//    blinker.test(tacho.sweep());
}

void loop(void) {

    blinker.readInputs();
    blinker.blink();
    blinker.setRevState(canData.temp, canData.rpm);
    Serial.printf("RPM: %d, Temp: %d\n", canData.rpm, canData.temp);
    tacho.update(canData.rpm);

    if (wifiState) {
        ArduinoOTA.handle();
    }
    // Check for incoming CAN message
    if (CAN.readMessage(&canMsg) == MCP2515::ERROR_OK) {
        // Handle different CAN message IDs
        switch (canMsg.can_id) {
            // RPM message ID
            case CAN_ID_RPM:
                // Extract RPM from the message and update tacho
                canData.rpm = canMsg.data[1] << 8 | canMsg.data[0];
                if (canData.rpm >= WIFI_RPM) {
                    reached = true;
                }
                if (canData.rpm < WIFI_RPM && reached) {
                    reached = false;
                    wifiState = toggleWifi();
                }
                break;
            case CAN_ID_TEMP:
                // Extract CLT from the message
                canData.temp = canMsg.data[2] - 40;
                break;
            default:
                break;
        }
    }
}


bool toggleWifi() {
    if (!wifiState) {
        if (!WiFi.mode(WIFI_STA)) {
            return false;
        }
        WiFi.hostname(HOST_NAME);
        WiFi.begin(SSID, PWD);

        while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
            currentMillis = millis(); // Get the current time
            if (currentMillis - previousMillis >= delayInterval) {
                previousMillis = currentMillis; // Save the last time the loop ran

                static int count = 0; // Static variable to keep count across iterations

                // Increment the count and reset if it reaches 255
                count = count + 25;
                if (count > 255) {
                    count = 0;
                }
                tacho.writeRaw(count);
            }
        }
        ArduinoOTA.begin();
        return true;
    } else {
        WiFi.mode(WIFI_OFF);
        ArduinoOTA.end();
        return false;
    }
}