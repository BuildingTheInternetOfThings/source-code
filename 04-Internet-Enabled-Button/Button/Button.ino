
#include <ESP8266WiFi.h>

#define BUTTON      14
#define LIGHT       12

int currentState;

int buttonState = 0;
int lastButtonState =0;

long lastDebounceTime = 0;  
long debounceDelay = 50;    

int count = 0;

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

const char* host = "maker.ifttt.com";
const char* key = "<KEY>";

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(BUTTON, INPUT);
  pinMode(LIGHT, OUTPUT);
  digitalWrite(LIGHT, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  
}

void loop() {
  
  currentState = digitalRead(BUTTON);
  if ( currentState != lastButtonState ) { 
    lastDebounceTime = millis();
  }  
  
  if ((millis() - lastDebounceTime) > debounceDelay) {   
    if ( currentState != buttonState ) { 
      if ( currentState == LOW ) { 
        digitalWrite(LIGHT, LOW);
      } else { 
        count++;
        Serial.print( "Button press (");  
        Serial.print( count );
        Serial.println( ")" );
        digitalWrite(LIGHT, HIGH);
 
        WiFiClient client;
        String url = "/trigger/button_pressed/with/key/";
        url += key;
        url += "?value1=";
        url += count;

        if (client.connect(host, 80)) {
           Serial.print("Requesting URL: http://");
           Serial.print(host);
           Serial.println(url);

           client.println(String("GET http://") + host + url + String(" HTTP/1.1"));
           client.println(String("Host: ") + host);
           client.println("Accept: */*");
           client.println("Connection: close");         
           client.println();
 
           delay(1000);
        } else {
          Serial.println("Connection Failed.");
          return;
        }
        while(client.available()){
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
        Serial.println();
        Serial.println("Closing Connection.");           
 
      }
      buttonState = currentState;
      
    }
  }
  lastButtonState = currentState;
 
}
