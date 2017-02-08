int ledPin = 2;                     //attach a LED to Digital 2
int temperatureSensorPin = 2;       //temperature sensor is attached to analog 2
int lightSensorPin = 1;             //light sensor is attached to analog 1
int closeIfLess = 300;              //value to turn on or off the LED
int closeIfMore = 600;              //value to turn on or off the LED
int temperatureData;                //not real temperature, just the data received by temperature sensor
int delayTime=1000;                 //duration between temperature readings
float realTemperature;              //real temperature, calculated in loop(), it needs the temperatureData
int thermistor = 3975;              //value of the thermistor and used for calculating the real temperature
float resistance;                   //calculated in loop(), it needs the temperatureData and used for calculating the real temperature
int lightSensorValue;
boolean opened = false;             //motor controller
int opening = 0;                    //motor controller
int closing = 0;                    //motor controller
int lightArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //array with the 10 last light data, it gives more accuracy to the motor controller

void setup()
{
    pinMode(ledPin,OUTPUT);         //set the LED on Digital 2 as an OUTPUT
    Serial.begin(9600);
}

void loop()
{   
    //Temperature data
    temperatureData = analogRead(temperatureSensorPin);          
    resistance = (float)(1023-temperatureData)*10000/temperatureData;
    realTemperature = 1/(log(resistance/10000)/thermistor+1/298.15)-273.15;
    Serial.print("Temperatura: ");
    Serial.println(realTemperature);
    
    //End of temperature data

    //Light data
    if (opening == 0 && closing == 0) //it'll just read the sensor if the motor is turned off
    {
      lightSensorValue = analogRead(lightSensorPin); 
      Serial.print("Luminosidade: ");
      Serial.println(lightSensorValue);
    
      for (int i=9; i>0; i--) //shift right in lightArray
      {
        lightArray[i] = lightArray[i-1];
      }
      lightArray[0] = lightSensorValue; //attach the new value in lightArray
  
      //Printing lightArray
      Serial.print("Vetor de luz: ");
      for (int i = 0; i<10; i++)
      {
        Serial.print(lightArray[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    //End of light data

    //Motor DC controller - works with light sensor data
    if(lightSensorValue<closeIfLess || lightSensorValue>closeIfMore) //light is too high or too low, must be closed
    {
      if ((opened) && //check if the whole array is within the value to be closed
            ((lightArray[0]<closeIfLess && lightArray[1]<closeIfLess && lightArray[2]<closeIfLess && lightArray[3]<closeIfLess
            && lightArray[4]<closeIfLess && lightArray[5]<closeIfLess && lightArray[6]<closeIfLess && lightArray[7]<closeIfLess
            && lightArray[8]<closeIfLess && lightArray[9]<closeIfLess)
            ||
            (lightArray[0]>closeIfMore && lightArray[1]>closeIfMore && lightArray[2]>closeIfMore && lightArray[3]>closeIfMore
            && lightArray[4]>closeIfMore && lightArray[5]>closeIfMore && lightArray[6]>closeIfMore && lightArray[7]>closeIfMore
            && lightArray[8]>closeIfMore && lightArray[9]>closeIfMore)))
      {
        if (closing == 0) //starting closing
        {
            digitalWrite(ledPin,HIGH); //REPLACE THIS LINE WITH THE MOTOR CLOSING ACTIVATE LINE
            Serial.println("Fechando - Aguarde 10 segundos");
            closing++;
         }
          
        else
        {
          if (closing == 10) //finished closing
          {
            Serial.println("Fechado!");
            digitalWrite(ledPin,LOW); //REPLACE THIS LINE WITH THE MOTOR DESACTIVATE LINE
            closing = 0;
            opened = false;
          }
          else //still closing
          {
            Serial.print("Fechando - Aguarde ");
            Serial.print(10 - closing);
            Serial.println(" segundo(s)");
            closing++;
          }
        }
      }
    }
    else //sensor value is an intermediary value, must be opened 
    {
      if ((!opened) && //check if the whole array is within the value to be opened
            ((lightArray[0]>=closeIfLess && lightArray[1]>=closeIfLess && lightArray[2]>=closeIfLess && lightArray[3]>=closeIfLess
            && lightArray[4]>=closeIfLess && lightArray[5]>=closeIfLess && lightArray[6]>=closeIfLess && lightArray[7]>=closeIfLess
            && lightArray[8]>=closeIfLess && lightArray[9]>=closeIfLess)
            &&
            (lightArray[0]<=closeIfMore && lightArray[1]<=closeIfMore && lightArray[2]<=closeIfMore && lightArray[3]<=closeIfMore
            && lightArray[4]<=closeIfMore && lightArray[5]<=closeIfMore && lightArray[6]<=closeIfMore && lightArray[7]<=closeIfMore
            && lightArray[8]<=closeIfMore && lightArray[9]<=closeIfMore)))
      {
        if (opening == 0) //starting opening
        {
          digitalWrite(ledPin,HIGH); //REPLACE THIS LINE WITH THE MOTOR OPENING ACTIVATE LINE
          Serial.println("Abrindo - Aguarde 10 segundos");
          opening++;
        }
        else
        {
          if (opening == 10) //finished opening
          {
            Serial.println("Aberto!");
            digitalWrite(ledPin,LOW); //REPLACE THIS LINE WITH THE MOTOR DESACTIVATE LINE
            opening = 0;
            opened = true;
          }
          else //still opening
          {
            Serial.print("Abrindo - Aguarde ");
            Serial.print(10 - opening);
            Serial.println(" segundo(s)");
            opening++;
          }
        }
      }
    }
    //End of motor DC controller
    
    delay(delayTime);
}
