int sentido1 = 4;
int sentido2 = 5;
int sensorluz = 6;

void setup()
{
    pinMode(sensorluz, INPUT);
    pinMode(sentido1, OUTPUT);
    pinMode(sentido2, OUTPUT);
}

void loop() 
{
    
  
    entradasensorluz = analogRead(sensorluz);
    if (entradasensorluz > X) //X é o limiar para abertura da persiana
    {
      //Ativa em um sentido para abrir
        digitalWrite(sentido1, HIGH);
        digitalWrite(sentido2, LOW);
        delay(3000); //Funciona por 3s
        //Desliga os dois
        digitalWrite(sentido1, LOW);
        digitalWrite(sentido2, LOW);
    }
 if (entradasensorluz < Y) //Y é o limiar para fechamento da persiana
    {
      //Ativa em um sentido para fechar
        digitalWrite(sentido1, HIGH);
        digitalWrite(sentido2, LOW);
        delay(3000); //Funciona por 3s
        //Desliga os dois
        digitalWrite(sentido1, LOW);
        digitalWrite(sentido2, LOW);
    }      
    
      

