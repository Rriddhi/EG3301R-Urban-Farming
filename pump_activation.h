// motor one - Solution A
//int enA = 13;
const int pump_in1_sol_A = 30;
const int pump_in2_sol_A = 31;

// motor two - Solution B
// enB = 12;
const int pump_in1_sol_B = 32;
const int pump_in2_sol_B = 33;

//motor ph up pin
const int pump_in1_ph_up =  34;
const int pump_in2_ph_up =  35;

//motor ph down pin
const int pump_in1_ph_down =  36;
const int pump_in2_ph_down = 37;


//pump_activation parameters
float pumpVal;
float phUpperThreshold = 6.3;
float phLowerThreshold = 0;
float ecLowerThreshold = 1.8;
const long pumpShutOffInterval = 350000;// 5+ mins
const long pumpOnInterval = 7000;

unsigned long previousMillis = 300000;

namespace Pump {
  void initialise() {
  //nutrient dosing 
    pinMode(pump_in1_sol_A, OUTPUT);
    pinMode(pump_in2_sol_A, OUTPUT);
    pinMode(pump_in1_sol_B, OUTPUT);
    pinMode(pump_in2_sol_B, OUTPUT);
  //ph up and down
    pinMode(pump_in1_ph_up, OUTPUT);
    pinMode(pump_in2_ph_up, OUTPUT);
    pinMode(pump_in1_ph_down, OUTPUT);
    pinMode(pump_in2_ph_down, OUTPUT);
  }
}
unsigned long check_activate_pump(unsigned long currentMillis, float phSensorValue, float ecSensorValue , float phLowerThreshold, float phUpperThreshold, float ecLowerThreshold )
//  Serial.print("ecSensorValue:");
//  Serial.print(ecSensorValue,1);
//  Serial.print("  EC Thresh:");
//  Serial.println(ecLowerThreshold,2);
//  Serial.print("  pH:");
//  Serial.print(phSensorValue,2);
//  Serial.print("  pH Thresh:");
//  Serial.println(phUpperThreshold,2);
{
  if (ecSensorValue < ecLowerThreshold) {
    previousMillis = currentMillis;
    digitalWrite(pump_in1_sol_A,HIGH);
    digitalWrite(pump_in2_sol_A,LOW);
    digitalWrite(pump_in1_sol_B,HIGH);
    digitalWrite(pump_in2_sol_B,LOW);
    pumpVal = ecSensorValue;
  }
  if (ecSensorValue > ecLowerThreshold && phSensorValue < phLowerThreshold)  {
    previousMillis = currentMillis;
    digitalWrite(pump_in1_ph_up,HIGH);
    digitalWrite(pump_in2_ph_up,LOW);
    pumpVal = phSensorValue;
  }
  if (ecSensorValue > ecLowerThreshold && phSensorValue > phUpperThreshold)  {
    previousMillis = currentMillis;
    digitalWrite(pump_in1_ph_down,HIGH);
    digitalWrite(pump_in2_ph_down,LOW);
    pumpVal = phSensorValue;
  }
  return previousMillis, pumpVal;
}

void deactivate_pump() {
    digitalWrite(pump_in1_sol_A,LOW);
    digitalWrite(pump_in2_sol_A,LOW);
    digitalWrite(pump_in1_sol_B,LOW);
    digitalWrite(pump_in2_sol_B,LOW);
    digitalWrite(pump_in1_ph_up,LOW);
    digitalWrite(pump_in2_ph_up,LOW);
    digitalWrite(pump_in1_ph_down,LOW);
    digitalWrite(pump_in2_ph_down,LOW);
}
