#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

// Constants for time and pulses
#define MICROSECONDS_PER_MINUTE (60UL * 1000000UL)
#define PULSES_PER_REVOLUTION 2
#define DUTYCYCLE 10 // Duty cycle in percent

// Maximum PWM value for the tachometer
#define MAX_PWM 255

// RPM limits
#define MAX_RPM 7000
#define MIN_RPM 1000

// Pins for tachometer
#define TACHO_OUTPUT_PIN 3
#define TACHO_INPUT_PIN 7

// Function prototypes
double linterp(double value, double in_min, double in_max, double out_min, double out_max);
double interpolate2d(long rpm, const long x[], const double y[], size_t arraySize);
void updateTacho(long rpm);

// CAN message structure
struct can_frame canMsg;
MCP2515 can0(10); // SPI CS Pin for MCP2515

// Variables for tacho control
size_t prevPwmValueTacho = 0;
size_t pwmValueTacho = 0;

// RPM and corresponding multiplier values
long rpms[] = {MIN_RPM, 2000, 3000, 4000, 5000, 6000, MAX_RPM};
double multipliers[] = {0.65, 0.855, 0.92, 0.9, 0.92, 0.93, 1.0};
size_t arraySize = sizeof(rpms) / sizeof(rpms[0]);

void setup(void)
{
  // Initialize CAN bus communication
  can0.reset();
  can0.setBitrate(CAN_500KBPS, MCP_8MHZ);
  can0.setNormalMode();
  delay(100);

  // Set pin modes for tachometer
  pinMode(TACHO_INPUT_PIN, INPUT);
  pinMode(TACHO_OUTPUT_PIN, OUTPUT);
}

void loop(void)
{
  // Check for incoming CAN messages
  if (can0.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    // Handle different CAN message IDs
    switch (canMsg.can_id)
    {
    case 0x201:                                                      // RPM message ID
      updateTacho((long)canMsg.data[1] << 8 | (long)canMsg.data[0]); // Extract RPM from the message and update tacho
      break;
    default:
      break;
    }
  }
}

// Linear interpolation function
double linterp(double value, double in_min, double in_max, double out_min, double out_max)
{
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// 2D interpolation function
double interpolate2d(long rpm, const long x[], const double y[], size_t arraySize)
{
  // Check if RPM is below the lowest RPM in the table
  if (rpm <= x[0])
  {
    return y[0];
  }

  // Check if RPM is above the highest RPM in the table
  if (rpm >= x[arraySize - 1])
  {
    return y[arraySize - 1];
  }

  // Find the last index less than the searched value
  size_t idx = 0;
  for (idx = 0; idx < arraySize - 1; idx++)
  {
    if (x[idx + 1] > rpm)
    {
      break;
    }
  }
  // Perform linear interpolation
  return linterp(rpm, x[idx], x[idx + 1], y[idx], y[idx + 1]);
}

// Update tachometer based on RPM
void updateTacho(long rpm)
{
  // Calculate PWM value for the tachometer based on RPM and interpolation
  int pwmvalue = int(linterp(rpm, 0, MAX_RPM + 100, 0, MAX_PWM) * interpolate2d(rpm, rpms, multipliers, arraySize));

  // Limit PWM value within the range of steps
  if (pwmvalue > MAX_PWM)
  {
    analogWrite(TACHO_OUTPUT_PIN, MAX_PWM);
  }
  else
  {
    analogWrite(TACHO_OUTPUT_PIN, pwmvalue);
  }
}
