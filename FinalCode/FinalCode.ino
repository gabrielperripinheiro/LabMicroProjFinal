int relayPin = 6;                   //attach a LED to Digital 6
int ledPin = 2;                     //attach a LED to Digital 2
int temperatureSensorPin = 2;       //temperature sensor is attached to Analog 2
int lightSensorPin = 1;             //light sensor is attached to Analog 1
int closeIfLess = 300;              //value to turn on or off the LED
int closeIfMore = 600;              //value to turn on or off the LED
int temperatureData;                //not real temperature, just the data received by temperature sensor
int delayTime=1000;                 //duration between temperature readings
float realTemperature;              //real temperature, calculated in loop(), it needs the temperatureData
int thermistor = 3975;              //value of the thermistor and used for calculating the real temperature
float resistance;                   //calculated in loop(), it needs the temperatureData and used for calculating the real temperature
int lightSensorValue;
boolean opened = false;             //motor1 controller
int opening = 0;                    //motor1 controller
int closing = 0;                    //motor1 controller
int lightArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};          //array with the 10 last light data, it gives more accuracy to the motor1 controller
float temperatureArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //array with the 10 last temperature data, it gives more accuracy to the motor2 controller
float toleranceTemperature = 27.0;  //above this temperature the motor2 turns on
int timeNecessaryToOpenOrClose = 5;//time in seconds that the motor keeps turned on to the opening or closing proccess
int sentido_anti_horario = 4;                    //motor1 controller (left) attached to Digital 4
int sentido_horario = 3;                    //motor1 controller (right) attached to Digital 3
int motorDC = 10;                    //motor1 controller attached to Digital 10
void setup()
{
    pinMode(ledPin, OUTPUT);         //set the LED on Digital 2 as an OUTPUT
    pinMode(relayPin, OUTPUT);       //set the relay on Digital 6 as an OUTPUT
    Serial.begin(9600);
    pinMode(sentido_anti_horario, OUTPUT);
    pinMode(sentido_horario, OUTPUT);
    pinMode(motorDC, OUTPUT);
}

void loop()
{   
    //------------------------------------------------------//
    //Temperature data
    
    temperatureData = analogRead(temperatureSensorPin);          
    resistance = (float)(1023-temperatureData)*10000/temperatureData;
    realTemperature = 1/(log(resistance/10000)/thermistor+1/298.15)-273.15;
    
    Serial.print("Temperatura: ");
    Serial.print(realTemperature);
    Serial.println(" graus Celsius");
    
    for (int i=9; i>0; i--) //shift right in temperatureArray
    {
      temperatureArray[i] = temperatureArray[i-1];
    }
    temperatureArray[0] = realTemperature; //attach the new value in temperatureArray

    //Printing temperatureArray
      Serial.print("Vetor de temperatura: ");
      for (int i = 0; i<10; i++)
      {
        Serial.print(temperatureArray[i]);
        Serial.print(" ");
      }
      Serial.println();
    
    //End of temperature data
    //------------------------------------------------------//
    

    
    //------------------------------------------------------//
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
    //------------------------------------------------------//


    
    //------------------------------------------------------//
    //Motor DC controller - works with light sensor data (motor1)

    //if the lightSensorValue is under or above the two determinated value, motor1 is activate in one direction (opening direction) if it wasn't opened yet
    //else if the lightSensorValue is within the  two determinated value, motor1 is activate in other direction (closing direction) if it wasn't closed yet
    //the motor1 is turned off when its timeNecessaryToOpenOrClose is reached
    
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
            digitalWrite(ledPin,HIGH); //LED
            analogWrite(motorDC,255);
            digitalWrite(sentido_anti_horario,HIGH); //MOTOR CLOSING ACTIVATE LINE
            digitalWrite(sentido_horario,LOW); //MOTOR CLOSING ACTIVATE LINE
            
            Serial.print("ANTI HORARIO!!! ");
            Serial.print("Fechando - Aguarde ");
            Serial.print(timeNecessaryToOpenOrClose);
            Serial.println(" segundos");
            closing++;
         }
          
        else
        {
          if (closing == timeNecessaryToOpenOrClose) //finished closing
          {
            Serial.println("Fechado!");
            digitalWrite(ledPin,LOW); //REPLACE THIS LINE WITH THE MOTOR DESACTIVATE LINE
            analogWrite(motorDC,0);
            digitalWrite(sentido_anti_horario,LOW); //MOTOR CLOSING DESACTIVATE LINE
            digitalWrite(sentido_horario,LOW); //MOTOR CLOSING DESACTIVATE LINE
            closing = 0;
            opened = false;
          }
          else //still closing
          {
            Serial.print("Fechando - Aguarde ");
            Serial.print(timeNecessaryToOpenOrClose - closing);
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
          analogWrite(motorDC,255);
          digitalWrite(sentido_anti_horario,LOW); //MOTOR OPENING ACTIVATE LINE
          digitalWrite(sentido_horario,HIGH); //MOTOR OPENING ACTIVATE LINE 
          Serial.print(" HORARIO!!! ");
          Serial.print("Abrindo - Aguarde ");
          Serial.print(timeNecessaryToOpenOrClose);
          Serial.println(" segundos");
          opening++;
        }
        else
        {
          if (opening == timeNecessaryToOpenOrClose) //finished opening
          {
            Serial.println("Aberto!");
            digitalWrite(ledPin,LOW); //REPLACE THIS LINE WITH THE MOTOR DESACTIVATE LINE
            analogWrite(motorDC,0);
            digitalWrite(sentido_anti_horario,LOW); //MOTOR OPENING DESACTIVATE LINE
            digitalWrite(sentido_horario,LOW); //MOTOR OPENING DESACTIVATE LINE
            opening = 0;
            opened = true;
            
          }
          else //still opening
          {
            Serial.print("Abrindo - Aguarde ");
            Serial.print(timeNecessaryToOpenOrClose - opening);
            Serial.println(" segundo(s)");
            opening++;
          }
        }
      }
    }
    
    //End of motor DC controller (motor1) (light)
    //------------------------------------------------------//

    
    
    //------------------------------------------------------//
    //Motor controller - works with temperature sensor data (motor2)
    
    //if the whole array is above the toleranceTemperature, motor2 is turned on
    //else if the whole array is under or equal to the toleranceTemperature, motor2 is turned off
    
    if ((temperatureArray[0]>toleranceTemperature && temperatureArray[1]>toleranceTemperature 
            && temperatureArray[2]>toleranceTemperature && temperatureArray[3]>toleranceTemperature
            && temperatureArray[4]>toleranceTemperature && temperatureArray[5]>toleranceTemperature 
            && temperatureArray[6]>toleranceTemperature && temperatureArray[7]>toleranceTemperature
            && temperatureArray[8]>toleranceTemperature && temperatureArray[9]>toleranceTemperature))
    {
      //INSERT HERE THE MOTOR2 ACTIVATE LINE
      digitalWrite(relayPin,HIGH);
      Serial.println("Ventilador ligado!");
    }
    else if ((temperatureArray[0]<=toleranceTemperature && temperatureArray[1]<=toleranceTemperature 
            && temperatureArray[2]<=toleranceTemperature && temperatureArray[3]<=toleranceTemperature
            && temperatureArray[4]<=toleranceTemperature && temperatureArray[5]<=toleranceTemperature 
            && temperatureArray[6]<=toleranceTemperature && temperatureArray[7]<=toleranceTemperature
            && temperatureArray[8]<=toleranceTemperature && temperatureArray[9]<=toleranceTemperature))
    {
      //INSERT HERE THE MOTOR2 DESACTIVATE LINE
      digitalWrite(relayPin,LOW);
      Serial.println("Ventilador desligado!");
    }
    
    //End of motor DC controller (motor2) (temperature)
    //------------------------------------------------------//
    Serial.println();
    delay(delayTime);
}
