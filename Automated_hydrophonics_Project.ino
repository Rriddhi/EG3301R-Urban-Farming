#include "pump_activation.h"
#include "read_sensor.h"

const int numReadings = 10;

float phReadings[numReadings];      // the readings from the analog input
int phReadIndex = 0;              // the index of the current reading
float phTotal = 0;                  // the running total
float phAverage = 5.5;                // the average

float ecReadings[numReadings];      // the readings from the analog input
int ecReadIndex = 0;              // the index of the current reading
float ecTotal = 0;                  // the running total
float ecAverage = 8.0;                // the average

//Float switch and solenoid vaalve
const int float_switch = 4;
const int solenoid_valve = 7;          
int buttonState = 1; //reads pushbutton status 

unsigned long float_time = 0;
const long solenoid_valve_interval = 1000;

//Water Flow sensor 
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 5; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

void setup() {
  pinMode(float_switch, INPUT_PULLUP);
  pinMode(solenoid_valve, OUTPUT);
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  Serial.begin(115200);
  Pump::initialise();
  Sensors::initialise();
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
 
  for (int phThisReading = 0; phThisReading < numReadings; phThisReading++) {
    phReadings[phThisReading] = 0;
  }
  for (int ecThisReading = 0; ecThisReading < numReadings; ecThisReading++) {
    ecReadings[ecThisReading] = 0;
  }
}


void loop() {
  // read sensors ~ EC and pH
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 5000) {
    timepoint = millis();
    
    temperature = readTemperature();
    //ambient_temp_and_humidity();

    ecTotal = ecTotal - ecReadings[ecReadIndex];
    ecReadings[ecReadIndex] = read_ecSensor();
    ecTotal = ecTotal + ecReadings[ecReadIndex];
    ecReadIndex = ecReadIndex + 1;
    
    if (ecReadIndex >= numReadings) {
      // ...wrap around to the beginning:
      ecReadIndex = 0;
    }

    ecAverage = ecTotal / numReadings;
    //      Serial.print("average EC:");
    //      Serial.println(ecAverage);

    phTotal = phTotal - phReadings[phReadIndex];
    phReadings[phReadIndex] = read_phSensor();
    phTotal = phTotal + phReadings[phReadIndex];
    phReadIndex = phReadIndex + 1;

    if (phReadIndex >= numReadings) {
      // ...wrap around to the beginning:
      phReadIndex = 0;
    }

    phAverage = phTotal / numReadings;
    //      Serial.print("average pH:");
    //      Serial.println(phAverage);

    unsigned long currentMillis = millis();

    //Activate and de-activaete solenoid valve
    buttonState = digitalRead(float_switch);  
    if (currentMillis - float_time >= solenoid_valve_interval){
       
       if (buttonState == LOW){
          digitalWrite(solenoid_valve, HIGH);
          //Serial.println( "WATER LEVEL - LOW"); //switch solenoid on
       }
       else {
          digitalWrite(solenoid_valve,LOW); //Switch solenoid off
          //Serial.println( "WATER LEVEL - HIGH" );
       }   
       float_time = currentMillis;
    }

    //flow meter sensor readings
    if(currentMillis >= (cloopTime + 1000)){
      // Every second, calculate and print litres/hour
      cloopTime = currentMillis; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
    }

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
