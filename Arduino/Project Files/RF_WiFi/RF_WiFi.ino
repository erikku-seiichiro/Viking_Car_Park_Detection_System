//#include <doxygen.h>
//#include <ESP8266.h>
#include <SPI.h>
#include "RF24.h"
#include "WiFiEsp.h"

// WiFI Setup
#define SSID        "Team Viking"
#define PASSWORD    "DontGiveUp"

int status = WL_IDLE_STATUS;
int reqCount = 0;    
RingBuffer buf(8);

//ESP8266 wifi(Serial1);
WiFiEspServer server(80);

// RF Setup
bool radioNumber = 0; 

RF24 radio(9,10); // RF pins

byte addresses[][6] = {"1Node","2Node"}; // address of RF node
//bool role = 0; // sender 1, receiver 0

struct dataStruct{
  int ldr;
  int value;
}LDR1, LDR2, LDR3, LDR4, temp;

void setup(void)
{
  Serial.begin(115200);   
  Serial1.begin(115200);
  Serial.print("setup begin\r\n");
  
  WiFi.init(&Serial1);
  Serial.print("Attempting to start AP ");
  Serial.println(SSID);
  status = WiFi.beginAP(SSID, 10, PASSWORD, ENC_TYPE_WPA2_PSK);
  Serial.println("Access point started");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
  Serial.println("Server started");

  Serial.print("Setup RF24\r\n");

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  radio.startListening();

  Serial.print("Setup Done\r\n");
}

void loop(void)
{
//  Serial.print(wifi.getJoinedDeviceIP());
//  Serial.print(wifi.getIPStatus());
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }
      }
    }
    
    // give the web browser time to receive the data
    delay(10);

    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }

  if( radio.available()){
                                                           // Variable for the received timestamp
      while (radio.available()) {                          // While there is data ready
        radio.read( &temp, sizeof(temp) );             // Get the payload
        
        switch (temp.ldr){
         case 1 :           
          LDR1.ldr = temp.ldr;          
          LDR1.value = temp.value;
          break;
         case 2 :           
          LDR2.ldr = temp.ldr;          
          LDR2.value = temp.value;
          break;         
         case 3 :           
          LDR3.ldr = temp.ldr;          
          LDR3.value = temp.value;
          break;
         case 4 :           
          LDR4.ldr = temp.ldr;          
          LDR4.value = temp.value;
          break;
        }
      }
     
      radio.stopListening();                               // First, stop listening so we can talk  
//      myData.value += 0.01;                                // Increment the float value
      radio.write( &temp, sizeof(temp) );              // Send the final one back.      
      radio.startListening();                              // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.print(temp.ldr);  
      Serial.print(F(" : "));
      Serial.println(temp.value);

//      Serial.print(F("\n"));
      Serial.print(LDR1.ldr);  
      Serial.print(F(" : "));
      Serial.println(LDR1.value);

//      Serial.print(F("\n"));
      Serial.print(LDR2.ldr);  
      Serial.print(F(" : "));
      Serial.println(LDR2.value);

//      Serial.print(F("\n"));
      Serial.print(LDR3.ldr);  
      Serial.print(F(" : "));
      Serial.println(LDR3.value);

//      Serial.print(F("\n"));
      Serial.print(LDR4.ldr);  
      Serial.print(F(" : "));
      Serial.println(LDR4.value);
   }
}

void sendHttpResponse(WiFiEspClient client)
{
  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"  // the connection will be closed after completion of the response
    "\r\n");
  client.print("<!DOCTYPE HTML>\r\n");
  client.print("<html>\r\n");
//  client.print("<h1>Hello World!</h1>\r\n");
  client.print("<p id='ldr1'>");
  client.print(LDR1.value);
  client.print("</p>");
  client.print("<br>\r\n");
  client.print("<p id='ldr2'>");
  client.print(LDR2.value);
  client.print("</p>");
  client.print("<br>\r\n");
  client.print("<p id='ldr3'>");
  client.print(LDR3.value);
  client.print("</p>");
  client.print("<br>\r\n");
  client.print("<p id='ldr4'>");
  client.print(LDR4.value);
  client.print("</p>");
//  client.print("Requests received: ");
//  client.print(++reqCount);
//  client.print("<br>\r\n");
//  client.print("Analog input A0: ");
//  client.print(analogRead(0));
//  client.print("<br>\r\n");
  client.print("</html>\r\n");
}

void printWifiStatus()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, connect to ");
  Serial.print(SSID);
  Serial.print(" and open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

