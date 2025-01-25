
#include<ESP8266WiFi.h>
#include<SoftwareSerial.h>

SoftwareSerial bluetooth(2,3); //Rx=pin no 2, Tx=pin no 3

char ssid[] = "";  //wifi name
char password[] = ""; //wifi password

int status = WL_IDLE_STATUS;
WiFiClient client;
char server[] = ""; //Server
String data;
String str1="Region:Tier-1 Node ID:2";
String str2="Region:Tier-2 Node ID:1";
String str3="Alert! Motional activity detected in Tier-1 region of Node ID: 2";
String str4="Alert! Motional Activity detected in Tier-2 Region of Node ID: 1";

const int red=5; //Connect RED LED to Pin No 5
const int yellow=6; //Connect YELLOW LED to Pin No 6
const int green=7;// //Connect GREEN LED to Pin No 7

void setup()
{
  Serial.begin(9600);
  pinMode(red,OUTPUT);
  pinMode(yellow,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(green,HIGH);
  connectWifi(); 
}

void connectWifi() // Attempt to connect to wifi network
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(2000);
}

void loop()
{
  if(bluetooth.available()){
    String st=bluetooth.readString();
    if(st.equals(str1)){
      data=str3;
      digitalWrite(red,HIGH);
      digitalWrite(yellow,LOW);
      digitalWrite(green,LOW);
      ConnectToServer();
    }
    else{
      if(st.equals(str2)){
        data=str4;
        digitalWrite(red,LOW);
        digitalWrite(yellow,HIGH);
        digitalWrite(green,LOW);
        ConnectToServer();
      }
    }
  }
  delay(100);
}

void ConnectToServer(){
  
  if(client.connect(server, 80)){  
            Serial.println("\nconnected...");
            Serial.println("ARDUINO: forming HTTP request message");    
            client.print("GET ");
            client.print("/sensor_data.php?");//php code for database
            client.print(data);
            client.print(" HTTP/1.1");
            client.print("\r\n");
            client.print("Host: \r\n\r\n");
            Serial.println("ARDUINO: HTTP message sent");
            delay(3000);
            if(client.available())
            {
                Serial.println("ARDUINO: HTTP message received");
                Serial.println("ARDUINO: printing received headers and script response...\n");
                while(client.available())
                {
                    char c = client.read();
                    Serial.print(c);
                }
                delay(2000);
            }
            else
            {
                Serial.println("ARDUINO: no response received / no response received in time");
            }
            client.stop();
  }
  else{
            Serial.println("connection failure");
            ConnectToServer();
  }
}
