int ledPin=2;                //attach a LED to Digital 2
int thresholdvalue=400;      //the threshold to turn on or off the LED
int a;
int del=1000;                //duration between temperature readings
float temperature;
int B=3975;                  //B value of the thermistor
float resistance;

void setup()
{
    pinMode(ledPin,OUTPUT);   //set the LED on Digital 2 as an OUTPUT
    Serial.begin(9600);
}

void loop()
{
    a=analogRead(2);          //the temperature sensor is attached to analog 2
    resistance=(float)(1023-a)*10000/a;
    temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;
    delay(del);
    Serial.println(temperature);
    
    int sensorValue = analogRead(1); //the light sensor is attached to analog 1
    if(sensorValue<thresholdvalue)
    {
        digitalWrite(ledPin,HIGH);
    }
    else
    {
        digitalWrite(ledPin,LOW);
    }
}
