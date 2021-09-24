int FloatSensor=2;
int solenoidPin=3;
int led=13;           
int buttonState = 1; //reads pushbutton status 

void setup() 
{ 
Serial.begin(9600); 
pinMode(FloatSensor, INPUT_PULLUP); //Arduino Internal Resistor 10K
pinMode(solenoidPin, OUTPUT);
} 
void loop() 
{ 
buttonState = digitalRead(FloatSensor); 
  if (buttonState == LOW) 
  { 
    digitalWrite(solenoidPin, LOW);     //Switch Solenoid OFF
    delay(1000);
    Serial.println( "WATER LEVEL - HIGH"); 
  } 
  else 
  { 
    digitalWrite(solenoidPin, HIGH);    //Switch Solenoid ON
    delay(1000);                      //Wait 1 Second
    Serial.println( "WATER LEVEL - LOW" ); 
  } 
delay(1000); 
}
