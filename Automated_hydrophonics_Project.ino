#include "pump_activation.h"
#include "read_sensor.h"
#include <string>

// CONSTANTS
const int NUM_READINGS = 10;
const int POLL_RATE = 115200;
const int FLOAT_SWITCH = 4;
const int SOLENOID_VALVE = 7;
const long SOLENOID_VALVE_INTERVAL = 1000;
const unsigned char FLOW_SENSOR = 5; // Sensor Input
const int TIME_THRESHOLD = 5000;

// Readings
float phReadings[NUM_READINGS];      // the readings from the analog input
float ecReadings[NUM_READINGS];      // the readings from the analog input
float waterTempReadings[NUM_READINGS];
float ambientTempReadings[NUM_READINGS];
float humidityReadings[NUM_READINGS];

// Variables
int buttonState = 1; // reads pushbutton status
volatile int flow_frequency; // Measures flow sensor pulses

// Calculated values
float ecTotal = 0;                  // the running total
float ecAverage = 8.0;                // the average
float phTotal = 0;                  // the running total
float phAverage = 5.5;                // the average
unsigned int l_hour; // Calculated litres/hour

// Timekeeping
unsigned long floatTime = 0;
unsigned long currentTime;
unsigned long cloopTime;

// Indexes
int phReadIndex = 0;              // the index of the current reading
int ecReadIndex = 0;              // the index of the current reading


void flow () { // Interrupt function
   flow_frequency++;
}

void initialisePins() {
  pinMode(FLOAT_SWITCH, INPUT_PULLUP);
  pinMode(SOLENOID_VALVE, OUTPUT);
  pinMode(FLOW_SENSOR, INPUT);
  digitalWrite(FLOW_SENSOR, HIGH); // Optional Internal Pull-Up
}

void initTimeKeeping() {
  currentTime = millis();
  cloopTime = currentTime;
}

void resetPhReadings() {
  for (int phThisReading = 0; phThisReading < NUM_READINGS; phThisReading++) {
    phReadings[phThisReading] = 0;
  }
}

void resetEcReadings() {
  for (int ecThisReading = 0; ecThisReading < NUM_READINGS; ecThisReading++) {
    ecReadings[ecThisReading] = 0;
  }
}

/**
 * Prepares the device for reading
 */
void setup() {
  Serial.begin(POLL_RATE);
  Pump::initialise();
  Sensors::initialise();
  sei(); // Enable interrupts
  initTimeKeeping();
  resetEcReadings();
  resetPhReadings();
}

void sendData(string description, float value) {
  Serial.print(description + ": ");
  Serial.println(value);
  Serial.print('\0');
}

void toggleSolenoidValve(unsigned long *startTime) {
  buttonState = digitalRead(FLOAT_SWITCH);
  if (millis() - *startTime < SOLENOID_VALVE_INTERVAL) {
    return;
  }
  digitalWrite(SOLENOID_VALVE, !buttonState);
  *startTime = millis();
}

void getflowMeterReading() {
  // TODO
  /*
    // flow meter sensor readings
    if(currentMillis >= (cloopTime + 1000)){
      // Every second, calculate and print litres/hour
      cloopTime = currentMillis; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
    }
    */
}

void loop() {

    sendData("temperature", readTemperature());
    sendData("ambientTemp", readAmbientTemperature());
    sendData("humidity", readAmbientTemperature());
    sendData("ec", readEcSensor());
    sendData("pH", readPhSensor());

    toggleSolenoidValve(&currentTime);



    // Activate and deactivate pump
    pumpVal = 0.0;
    if (currentMillis - previousMillis >= pumpShutOffInterval) {
      previousMillis, pumpVal = check_activate_pump(currentMillis, phAverage, ecAverage, phLowerThreshold, phUpperThreshold, ecLowerThreshold );

    }
    if (currentMillis - previousMillis >= pumpOnInterval) {
      deactivate_pump();
    }
  }
}
