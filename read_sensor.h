#include "DFRobot_PH.h"
#include "DFRobot_EC.h"
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

#define PH_PIN A1
#define EC_PIN A2
#define ONE_WIRE_BUS 26

#define DHTPIN 8    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHTPIN, DHTTYPE);
DFRobot_PH ph;
DFRobot_EC ec;

float voltage,phSensorValue, ecSensorValue,h,t,temperature = 25;
namespace Sensors {
  void initialise() {
    ph.begin();
    ec.begin();
    dht.begin();
  }
}

float readTemperature() {
   sensors.requestTemperatures(); // Send the command to get temperature readings
   temperature = sensors.getTempCByIndex(0);
   return temperature;
}

float readPhSensor() {
  temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
  voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
  phSensorValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
//  Serial.print("temperature:");
//  Serial.print(temperature,1);
  // Serial.print("^C  pH:");
  // Serial.println(phSensorValue,2);
  return phSensorValue;
}

float readEcSensor() {
  voltage = analogRead(EC_PIN)/1024.0*5000;  // read the voltage
  temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
  ecSensorValue =  ec.readEC(voltage,temperature) - 0.5 ;  // convert voltage to EC with temperature compensation
  // Serial.print("temperature:");
  // Serial.print(temperature,1);
  // Serial.print("^C  EC:");
  // Serial.print(ecSensorValue,2);
  // Serial.println("ms/cm");
  return ecSensorValue;
}

// DHT11 sensor values
float ambient_temp_and_humidity() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  else{
    return h,t;
  }
}
  //Serial.print(F(" Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.println(F("°C "));
