#include <SPI.h>
#include "RF24.h"
#include <RBD_LightSensor.h>

// LDR Setup
RBD::LightSensor LDR1(A0);
RBD::LightSensor LDR2(A1);

// RF Setup
RF24 radio(9,10);

bool radioNumber = 1;
//bool role = 1; // 1 sender, 0 receiver

byte addresses[][6] = {"1Node","2Node"};

struct dataStruct{
  int ldr;
  int value;
}data1, data2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Setup RF\r\n");

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
}

void loop() {
  radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    Serial.println(F("Now sending"));

    data1.ldr = 1;
    data1.value = LDR1.getRawValue();

    data2.ldr = 2;
    data2.value = LDR2.getRawValue();
    
     if (!radio.write( &data1, sizeof(data1) )){
       Serial.println(F("failed"));
     }

     if (!radio.write( &data2, sizeof(data2) )){
       Serial.println(F("failed"));
     }
//        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
                                                                // Grab the response, compare, and send to debugging spew
        radio.read( &data1, sizeof(data1) );
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(F(", Got response "));
        Serial.print(data1.ldr);
        Serial.print(F(" ldr Value "));
        Serial.println(data1.value);
    }

    // Try again 1s later
    delay(3000);
}
