#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);
float t=0;
char data = 0;
#define DEBUG true
#define RX 10
#define TX 11

String HOST = "api.thingspeak.com";
String PORT = "80";
String AP = "JioFi2 SUMIT";
String PASS = "sumitbera1999";


String API = "IB4OUV0UF8SEQOT7";
String field = "field1";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX,TX); 
void setup() {

Serial.begin(9600);
esp8266.begin(115200);
lcd.init();  // to intialize LCD
  lcd.backlight();// To power on lcd back light

  lcd.setCursor(0,0); 

  lcd.print("    Welcome");

  lcd.setCursor(0,1);

  lcd.print("       To        "); 

  delay(3000);


  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("     AIR"); 

  lcd.setCursor(0,1);

  lcd.print("QUALITY MONITOR");  

  delay(3000); 

sendCommand("AT",5,"OK");
sendCommand("AT+CWMODE=1",5,"OK");
sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",15,"OK");
countTrueCommand = 0;
delay(1000);


  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("      WIFI");

  lcd.setCursor(0,1);

  lcd.print("   CONNECTED");


}

void loop() {

String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
switch(countTrueCommand) {

case 0:  
case 1: sendCommand("AT+CIPMUX=1",5,"OK"); break;
case 2: sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,20,"OK"); break;
case 3: sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); break;
case 4: esp8266.println(getData);delay(1500);countTrueCommand++; break;
case 5: sendCommand("AT+CIPCLOSE=0",5,"OK"); break;
case 6:
t = analogRead(A0);
Serial.print("Air Quality= ");
Serial.println(t);

valSensor = analogRead(A0); // random value, change with sensor value if using sensor
countTrueCommand = 0;
lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print ("Air Qual: ");
  lcd.print (t);
  lcd.print (" PPM ");
  lcd.setCursor (0,1);
  if (t<=500)
   {
   lcd.print("Fresh Air");
   Serial.print("Fresh Air ");
   
}
  else if( t>=500 && t<=1000 )
{
   lcd.print("Poor Air");
   Serial.print("Poor Air");
  
}
  else if (t>=1000 )
{
  lcd.print("Very Poor");
  Serial.print("Very Poor");
  
}
  //lcd.scrollDisplayLeft();
  delay(10000);

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("  SENDING DATA");

  lcd.setCursor(0,1);

  lcd.print("    TO CLOUD");

delay(10000);
break;

}

}
void sendCommand(String command, int maxTime, char readReplay[]) {

Serial.print(countTrueCommand);
Serial.print(". at command => ");
Serial.print(command);
Serial.print(" ");
while(countTimeCommand < (maxTime*1))

{

esp8266.println(command);//at+cipsend
if(esp8266.find(readReplay))//ok
{

found = true;
break;

}
countTimeCommand++;

}
if(found == true)
{

Serial.println("OYI");

countTrueCommand++;
countTimeCommand = 0;

}
if(found == false)
{

Serial.println("Fail");
countTrueCommand = 0;
countTimeCommand = 0;

}
found = false;
}
