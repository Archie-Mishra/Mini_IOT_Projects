#include <SoftwareSerial.h>  
#include <stdlib.h>

#include <DHT.h> 

#include <LiquidCrystal.h>

#define DHTPIN A0 
#define DHTTYPE  DHT11 

DHT dht(DHTPIN, DHTTYPE); 

String apiKey = "FZI2UO2TIP6OUYVC"; 
SoftwareSerial ser(2, 3);
 
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

float humidity ;
float temperature ;
float smoke ;
float moisture ;
 
void setup() 
{                
 
  Serial.begin(9600);       
  ser.begin(115200);   
  
  dht.begin();
  
  lcd.begin(16, 2); 
 
  pinMode(A1 , INPUT);
  pinMode(A2, INPUT);
  pinMode(4, OUTPUT); //buzzer
  pinMode(5, OUTPUT); //motor
  pinMode(6, OUTPUT); //motor
  
  //Wifi module
  ser.println("AT");
  delay(1000);
  ser.println("AT+RST");    
  delay(1000);
  ser.println("AT+CWMODE=1");   
  delay(1000);
  ser.println("AT+CWJAP=\"AndroidAP\",\"wuoe4856\"");
  delay(10000);

  lcd.print("PROJECT NAME :-");
  lcd.setCursor(0, 1);
  lcd.print("SMART FARMING");
  lcd.setCursor(0, 0);
  delay(5000);  
  lcd.clear();
  
  
  lcd.print("PROJECT BY :- ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ARCHIE MISHRA");
  lcd.setCursor(0, 0);
  delay(5000);  
  lcd.clear();

  lcd.print("PARAMETERS VAL :");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);  
}

 
void loop() 
{
  humidity = dht.readHumidity();
  Serial.print("Humidity = ");
  Serial.println(humidity);
  temperature = dht.readTemperature();
  Serial.print("Temperature = ");
  Serial.println(temperature);
 
  lcd.print("TEMP. :");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("HUMDT :");
  lcd.print(humidity);
  lcd.setCursor(0, 0);
  delay(2000); 

  lcd.clear();
  
  smoke = analogRead(A1);
  Serial.print("Methane Quantity = ");
  Serial.println(smoke);
  lcd.print("METH :");
  lcd.print(smoke);
  lcd.setCursor(0, 1);
  
  moisture = analogRead(A2);
  Serial.print("Moisture = ");
  Serial.println(moisture);
  lcd.print("MOIST :");
  lcd.print(moisture);
  lcd.setCursor(0, 0);
  delay(2000);

  lcd.clear();
  
  String field1=String(temperature); 
  String field2=String(humidity);  
  
  //TCP Connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";  
  cmd += "184.106.153.149"; 
  cmd += "\",80";
  ser.println(cmd); 
  Serial.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  //GET https://api.thingspeak.com/update?api_key=TMWR47CL9G5PTZHM&field1=0&field2=0
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(field1);
  getStr +="&field2=";
  getStr += String(field2);
  getStr += "\r\n\r\n";
  
  cmd = "AT+CIPSEND=";        
  cmd += String(getStr.length()); 
  ser.println(cmd);
  Serial.println(cmd);

 
  if(ser.find(">"))
  {
    ser.print(getStr);
    Serial.print(getStr);
  }
 
  else
  {
    ser.println("AT+CIPCLOSE");  
   
    Serial.println("AT+CIPCLOSE");
  }
 
  delay(1000);  

  digitalWrite(4,LOW);

  if(analogRead(A2) > 200 ) //moisture
      {
        digitalWrite(5,HIGH); //motor
        digitalWrite(6,LOW); //motor 
        Serial.println("LED1 High");     
      }

  if(analogRead(A1) > 100 ) //methane
      {
        digitalWrite(4,HIGH);  //buzzer   
        Serial.println("LED2 High"); 
      }

  if(humidity > 90 )
      {
        digitalWrite(4,HIGH); //buzzer
        Serial.println("LED2 High"); 
      }
  
  if(temperature > 60 )
      {
        digitalWrite(4,HIGH); //buzzer 
        Serial.println("LED2 High"); 
      }
      
  else
     {
       digitalWrite(4,LOW); 
       digitalWrite(5,LOW); 
       digitalWrite(6,LOW); 
     }
 
}
