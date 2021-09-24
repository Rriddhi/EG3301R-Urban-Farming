int FloatSensor=2;   
int led=13;           
int buttonState = 1; //reads pushbutton status 

void setup() 
{ 
Serial.begin(9600); 
pinMode(FloatSensor, INPUT_PULLUP); //Arduino Internal Resistor 10K
pinMode (led, OUTPUT); 
} 
void loop() 
{ 
buttonState = digitalRead(FloatSensor); 
  if (buttonState == LOW) 
  { 
    digitalWrite(led, HIGH);
    Serial.println( "WATER LEVEL - HIGH"); 
  } 
  else 
  { 
    digitalWrite(led, HIGH);
    Serial.println( "WATER LEVEL - LOW" ); 
  } 
delay(1000); 
}


/*Initially, the sensor is connected to Pin 2 and LED in pin 13 that is the built-in LED on the Arduino board.
Initialize serial communication.
Now the digital values are read from the sensor and stored in the variable.
When variable “buttonState” is in high state, it denotes that the water level is low and yet to reach the destined limit. Now Arduino prints the message “WATER LEVEL – LOW” and turns LED off.
When water level reaches sensor level “buttonState” sensor output goes low and Arduino prints the message “WATER LEVEL – HIGH” message in serial monitor and simultaneously LED glows.
The program repeats reading the button state with a delay of 1 second.
 */
