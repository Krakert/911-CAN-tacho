#include <main.h>

// Function prototypes
double linterp(double value, double in_min, double in_max, double out_min, double out_max);

double interpolate2d(long value, const long x[], const double y[], size_t arraySize);

void updateTacho(long rpm);

bool toggleWifi();

long rpm = 0;
bool wifiState = false;
bool reached = false;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0; // Variable to store the last time the loop ran
const unsigned int delayInterval = 100; // Delay interval in milliseconds

void setup(void) {
    Serial.begin(115200);
    Serial.println("Booting");
    // Initialize CAN bus communication
    CAN.reset();
    CAN.setBitrate(CAN_500KBPS, MCP_8MHZ);
    CAN.setNormalMode();

    // Set pin modes for tachometer
    pinMode(TACHO_OUTPUT_PIN, OUTPUT);
}

void loop(void) {

    blinker.readInputs();
    blinker.blink();

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
               rpm = canMsg.data[1] << 8 | canMsg.data[0];
               if (rpm >= WIFI_RPM) {
                   reached = true;
               } else {
                   updateTacho(rpm);
               }
               if (rpm < WIFI_RPM && reached) {
                   reached = false;
                   wifiState = toggleWifi();
               }
               break;
           case CAN_ID_REV:
               // Extract rev limit from the message
               blinker.setRevState((canMsg.data[4] & 0x1) != 0);
               break;
           default:
               break;
       }
    }
}

// Linear interpolation function
double linterp(double value, double in_min, double in_max, double out_min, double out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// 2D interpolation function
double interpolate2d(long value, const long x[], const double y[], size_t size) {
    // Check if RPM is below the lowest RPM in the table
    if (value <= x[0]) {
        return y[0];
    }

    // Check if RPM is above the highest RPM in the table
    if (value >= x[size - 1]) {
        return y[size - 1];
    }

    // Find the last index less than the searched value
    size_t idx;
    for (idx = 0; idx < size - 1; idx++) {
        if (x[idx + 1] > value) {
            break;
        }
    }
    // Perform linear interpolation
    return linterp(value, x[idx], x[idx + 1], y[idx], y[idx + 1]);
}

// Update tachometer based on RPM
void updateTacho(long rpmIn) {
    // Calculate PWM value for the tachometer based on RPM and interpolation
    int pwmValue = int(linterp(rpmIn, 0, MAX_RPM + 100, 0, MAX_PWM) * interpolate2d(rpm, rpms, multiplierTacho, arraySizeRpm));

    // Limit PWM value within the range of steps
    if (pwmValue > MAX_PWM) {
        analogWrite(TACHO_OUTPUT_PIN, MAX_PWM);
    } else {
        analogWrite(TACHO_OUTPUT_PIN, pwmValue);
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
                analogWrite(TACHO_OUTPUT_PIN, count);
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