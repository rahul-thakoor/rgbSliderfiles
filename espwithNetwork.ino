#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
WebSocketsServer webSocket = WebSocketsServer(81);
const char* ssid     = "your ssid";
const char* password = "your password";
const int bluePin = 13;
const int redPin = 15;
const int greenPin = 12;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            
            break;
        case WStype_CONNECTED:
         {
                IPAddress ip = webSocket.remoteIP(num);
                
                
            }
            break;
        case WStype_TEXT:
        {
            
            String text = String((char *) &payload[0]);
          if(text=="LED"){
           
            digitalWrite(13,HIGH);
            delay(500);
            digitalWrite(13,LOW);
            Serial.println("led just lit");
            webSocket.sendTXT(num, "led just lit", lenght);
            }
            
           if(text.startsWith("x")){
            
            String xVal=(text.substring(text.indexOf("x")+1,text.length())); 
            int xInt = xVal.toInt();
            analogWrite(redPin,xInt); 
            Serial.println(xVal);
            webSocket.sendTXT(num, "red changed", lenght);
           }


           if(text.startsWith("y")){
            
            String yVal=(text.substring(text.indexOf("y")+1,text.length())); 
            int yInt = yVal.toInt();
            analogWrite(greenPin,yInt); 
            Serial.println(yVal);
            webSocket.sendTXT(num, "green changed", lenght);
           }

           if(text.startsWith("z")){
            
            String zVal=(text.substring(text.indexOf("z")+1,text.length())); 
            int zInt = zVal.toInt();
            analogWrite(bluePin,zInt); 
            Serial.println(zVal);
            webSocket.sendTXT(num, "blue changed", lenght);
           }
           if(text=="RESET"){
           
           analogWrite(bluePin,LOW);
           analogWrite(redPin,LOW);
            analogWrite(greenPin,LOW);
            Serial.println("reset");
            
            }
            

                            
                   }
            
           
           webSocket.sendTXT(num, payload, lenght);
            webSocket.broadcastTXT(payload, lenght);
            break;
        
        case WStype_BIN:
     
            hexdump(payload, lenght);

            // echo data back to browser
            webSocket.sendBIN(num, payload, lenght);
            break;
    }

}


void setup() {
   
    Serial.begin(115200);
    pinMode(13,OUTPUT);
    pinMode(15,OUTPUT);
    pinMode(12,OUTPUT);
   WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
  Serial.println(WiFi.localIP());
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}
