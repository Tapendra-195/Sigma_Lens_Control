#include "include/Config.h"
#include "include/Constants.h"
#include "include/Message.h"
#include "include/Message03.h"
#include "include/Message04.h"
#include "include/Message05.h"
#include "include/Message06.h"

int count = 0;

const int PIN_LENS_CS_BODY = 2;
const int PIN_BODY_CS_LENS = 3;
const int PIN_BODY_VD_LENS = 4;
const int PIN_TX = 1;
const int PIN_RX = 0;

bool changeToSerial = false;

// Timing parameters for a 60 Hz pulse
// Total period = 1/60 sec ≈ 16667 microseconds
// Low pulse duration is 63.4 microseconds (rounded to 64 for simplicity)
const unsigned long totalPeriod = 16667UL;   // Total period in microseconds
const unsigned long lowDuration   = 64UL;      // Duration of the LOW pulse in microseconds
const unsigned long highDuration  = totalPeriod - lowDuration; // Remaining HIGH time

// Used for timing the pulse on BODY_VD_LENS_PIN.
unsigned long lastPulseTime = 0;

Message03 *message03 = new Message03();
Message04 *message04 = new Message04();

int lensToBodyBufferPosition = INVALID_POSITION;
int packetLength = INVALID_POSITION;
byte lensToBodyBuffer[MAX_BUFFER_SIZE] = {0};


byte inited = 0;
int unusedClockWindows = 0;
bool highspeedMode = false;

int16_t currentFocus =0x00;
uint16_t currentAperture = 0x00;
uint16_t lastApertureDialValue = 0x00;

void startMessage() {
    //wait for lens_cs to go LOW
    while (digitalRead(PIN_LENS_CS_BODY)) {
        delayMicroseconds(1);
    }
    digitalWrite(PIN_BODY_CS_LENS, HIGH);
    delayMicroseconds(40);  
}

void finishMessage() {
    Serial1.flush();
    delayMicroseconds(40);
    digitalWrite(PIN_BODY_CS_LENS, LOW);
    unusedClockWindows = 0;  // We've sent something back
}

void setup() {
  Serial.begin(115200);


  // Set up pin modes:
  pinMode(PIN_BODY_VD_LENS, OUTPUT);
  pinMode(PIN_TX, OUTPUT);
  pinMode(PIN_BODY_CS_LENS, OUTPUT);
  digitalWrite(PIN_BODY_CS_LENS, LOW);
  //pinMode(LENS_CS_CAM_PIN, INPUT_PULLUP); // Assuming lens pulse is active LOW
  
  // Initial states:
  pinMode(PIN_BODY_VD_LENS, OUTPUT);
  digitalWrite(PIN_BODY_VD_LENS, HIGH);  // Normally HIGH; will be pulsed LOW briefly.
  //digitalWrite(TX_PIN, HIGH);

  pinMode(PIN_LENS_CS_BODY, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_LENS_CS_BODY), lensCsChange, CHANGE);

  Serial1.setRX(PIN_RX);
  //Serial2.begin(750000, SERIAL_8N1);//(750000, SERIAL_8N1);

  /*
    Serial.begin(115200);

    Serial1.setRX(0);
    Serial1.setTX(LISTEN_ONLY ? 5 : 1);
    Serial1.begin(750000, SERIAL_8N1);//(750000, SERIAL_8N1);

    Serial2.setRX(7);
    Serial2.begin(750000, SERIAL_8N1);//(750000, SERIAL_8N1);

    pinMode(PIN_BODY_VD_LENS, INPUT);
    pinMode(5, OUTPUT);


    if(LISTEN_ONLY) {
        pinMode(PIN_LENS_CS_BODY, INPUT);
        attachInterrupt(digitalPinToInterrupt(PIN_LENS_CS_BODY), lenCsChange, CHANGE);
  	} else {
  	    pinMode(PIN_LENS_CS_BODY, OUTPUT);
        digitalWrite(PIN_LENS_CS_BODY, LOW);
  	}
		
    pinMode(PIN_BODY_CS_LENS, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_BODY_CS_LENS), bodyCsChange, CHANGE);

    pinMode(PIN_BODY_VD_LENS, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_BODY_VD_LENS), bodyVdChange, CHANGE);
  */

}

void lensCsChange() {
    if (inited == 0) {
        inited++;
        changeToSerial = true;
    }
}

/*
void bodyVdChange() {
    if(DEBUG_TIMING) {
        Serial.print(digitalRead(PIN_BODY_VD_LENS) ? "C " : "c ");
        Serial.println(micros());
    }
    if ( inited != 0) {
        unusedClockWindows++;
        if (unusedClockWindows > 100) {
            Serial.println(RESETTING);
            unusedClockWindows = 0;
            lensToBodyBufferPosition = INVALID_POSITION;
            bodyToLensBufferPosition = INVALID_POSITION;
            setLowspeedMode();
            if(!LISTEN_ONLY) {
              inited = 0;  // RESET we've got into a bad state
            }
        }
    }
}
*/

void lenCsChange() {
    //printLenCS(digitalRead(PIN_LENS_CS_BODY));
}

/*
void bodyCsChange() {
    if (inited < INIT_COMPLETE || DEBUG_TIMING) {
        Serial.print(digitalRead(PIN_BODY_CS_LENS) ? "[B:" : "[b:");
        Serial.print(micros());
        Serial.println("]");
    }
    if (inited == 0) {
        //Serial.print("initited 0");
        delayMicroseconds(5642);//(990); // MAGIC NUMBER ONE, To the best of our knowledge these delays indicate we are a 750kbaud lens. :WRONG, tAPENDRA
        digitalWrite(PIN_LENS_CS_BODY, HIGH);
        printLenCS(true);
        inited++;
        delayMicroseconds(2031);
        digitalWrite(PIN_LENS_CS_BODY, LOW);
    }

    if (inited == 1 && highspeedMode){
      delayMicroseconds(6);
      digitalWrite(PIN_LENS_CS_BODY, HIGH);
      delayMicroseconds(1088);
      digitalWrite(PIN_LENS_CS_BODY, LOW);
      inited++;
    }
}
*/

void loop() {
    // put your main code here, to run repeatedly
    // --- Generate a 60 Hz negative pulse on Lens VD Cam ---
  unsigned long currentTime = micros();
  
  // Check if it's time to issue the low pulse:
  if (currentTime - lastPulseTime >= totalPeriod) {
    if(inited == 0){
      digitalWrite(PIN_TX, HIGH);            // TX held LOW at startup.
      digitalWrite(PIN_BODY_CS_LENS, LOW);  // Pulled HIGH during initialization.
    }
    // Pulse LOW for the specified duration
    digitalWrite(PIN_BODY_VD_LENS, LOW);
    delayMicroseconds(lowDuration);
    digitalWrite(PIN_BODY_VD_LENS, HIGH);
    
    // Reset timer
    lastPulseTime = currentTime;
    
  }

  if(inited == 0){
    delayMicroseconds(lowDuration);
    digitalWrite(PIN_TX, LOW);            // TX held LOW at startup.
    digitalWrite(PIN_BODY_CS_LENS, HIGH);  // Pulled HIGH during initialization.
  }

  if(changeToSerial && inited !=0){
    delayMicroseconds(15);
    Serial1.setTX(PIN_TX);
    Serial1.begin(750000, SERIAL_8N1);
    
    delayMicroseconds(2031);
    digitalWrite(PIN_BODY_CS_LENS, LOW);
    delayMicroseconds(1000);
    changeToSerial = false;

    startMessage();
    Serial1.write(init01, sizeof(init01));
    finishMessage();
  }


  if(inited != 0){
    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      message.trim();

      int spaceIndex = message.indexOf(' ');
      String cmd, arg;
      
      if (spaceIndex == -1) {
        cmd = message; // Only one word
      } else {
        cmd = message.substring(0, spaceIndex);
        arg = message.substring(spaceIndex + 1);

      }

      if (cmd == "SA") { // Set Aperture
        unsigned int aperture = (unsigned int)arg.toInt();
        Serial.print("ACK: Setting Aperture to ");
        Serial.println(aperture); // value after "SA"
        message03->setAperture(aperture);
  
      } 
      else if (cmd == "SF") { // Set Focus
        unsigned int focus = (unsigned int)arg.toInt();
        message04->setFocus(currentFocus, focus);
        Serial.print("ACK: Setting Focus to ");
        Serial.println(focus); // value after "SA"
      } 
      else if (cmd == "ON") {
        Serial.println("ACK: Lens turned ON.");
        
      } 
      else if (cmd == "OFF") {
        Serial.println("ACK: Lens turned OFF.");
        
      } 
      else {
        Serial.println("ACK: Unknown command.");
      }
    }

    if (Serial1.available() > 0) {
        processByte(Serial1.read(), lensToBodyBuffer, lensToBodyBufferPosition, lensToBody);
    }
  }

}


void setLowspeedMode(){
  if(highspeedMode == false) {
    return;
  }
  Serial1.begin(750000, SERIAL_8N1);
  highspeedMode = false;
}

void setHighspeedMode(){
  if(highspeedMode) {
    return;
  }
  Serial1.begin(1500000, SERIAL_8N1);
  highspeedMode = true;
}

void processMessage(Message *input) {
    switch (input->getMessageType()) {
        case 0x05:
          {
            currentAperture = static_cast<Message05*>(input)->getAperture();
            uint16_t currentApertureDialValue = static_cast<Message05*>(input)->getApertureDialValue();
            //If Aperture Dial is Rotated, change Aperture value based on the aperture dial value.
            if(lastApertureDialValue != currentApertureDialValue)
            {
              lastApertureDialValue = currentApertureDialValue;
              message03->setAperture(currentApertureDialValue);
            }
            break;
          }
        case 0x06:
          {
            currentFocus = static_cast<Message06*>(input)->getFocus();

            startMessage();
            message03->update();
            message03->prepForSending();
            Serial1.write(message03->mMessageBuffer, message03->getMessageLength());
            finishMessage();

            delayMicroseconds(10);
            

            startMessage();
            message04->update();
            message04->prepForSending();
            Serial1.write(message04->mMessageBuffer, message04->getMessageLength());
            finishMessage();

            break;
          }
        //INIT MESSAGES
        case 0x01:
          {
            startMessage();
            Serial1.write(init07, sizeof(init07));
            //writeSerial1Debuggable(init07, sizeof(init07));
            finishMessage();
            break;
          }
        case 0x07:
          {
            //Serial.print("inside 07 \n");
            startMessage();
            Serial1.write(init0C, sizeof(init0C));
            //writeSerial1Debuggable(init0C, sizeof(init0C));
            finishMessage();
            break;
          }
        case 0x0C:
          {
            //sets high speed mode (1500000 baud rate)
            setHighspeedMode();
            
            delayMicroseconds(6);
            digitalWrite(PIN_BODY_CS_LENS, HIGH);
            delayMicroseconds(1088);
            digitalWrite(PIN_BODY_CS_LENS, LOW);
            
            startMessage();
            Serial1.write(init0B, sizeof(init0B));
            //writeSerial1Debuggable(init0B, sizeof(init0B));
            finishMessage();
            //finish message
            break;
          }
        //High speed mode set, Now do initialization
        case 0x0B:
          {
            startMessage();
            Serial1.write(init3F, sizeof(init3F));
            //writeSerial1Debuggable(init3F, sizeof(init3F));
            finishMessage();
            break;
          }
        case 0x3F:
          {
            startMessage();
            Serial1.write(init3D, sizeof(init3D));
            //writeSerial1Debuggable(init3D, sizeof(init3D));
            finishMessage();
            break;
          }
        case 0x3D:
          {
            startMessage();
            Serial1.write(init08, sizeof(init08));
            //writeSerial1Debuggable(init08, sizeof(init08));
            finishMessage();
            break;
          }
        case 0x08:
          {
            startMessage();
            Serial1.write(init09, sizeof(init09));
            //writeSerial1Debuggable(init09, sizeof(init09));
            finishMessage();
            break;
          }
        case 0x09:
          {
            startMessage();
            Serial1.write(init0D, sizeof(init0D));
            finishMessage();
            break;
          } 
        case 0x0D:
          {
            startMessage();
            Serial1.write(init10, sizeof(init10));
            finishMessage();
            
            break;
          }
        case 0x10:
          {
            startMessage();
            Serial1.write(init0A, sizeof(init0A));
            //writeSerial1Debuggable(init0A, sizeof(init0A));
            finishMessage();
        
            break;
          }
        case 0x0A:
          //Initialization is complete.
            /*
            startMessage();
            writeSerial1Debuggable(init0A, sizeof(init0A));
            finishMessage();
            
            //delayMicroseconds(16000);
            //send the current aperture and focal length
            startMessage();
            writeSerial1Debuggable(init05, sizeof(init05));
            finishMessage();

            startMessage();
            writeSerial1Debuggable(init06, sizeof(init06));
            finishMessage();
            */
            break;        
    }
}

void processByte(int read, byte *buffer, int &position, int direction) {
    if (position == INVALID_POSITION) {
        if (read == START_BYTE) {
            position = 0;
            //Serial.print("FB ");  //first byte
            //Serial.println(micros());
        } else {
            return;
        }
    }

    buffer[position] = read;
    position++;
    if (position >= MAX_BUFFER_SIZE) {
        //ERROR
        position = INVALID_POSITION;
    }
    if (position == 2) {
        packetLength = (buffer[2] << 8) + buffer[1];
    }
    if (packetLength == position) {
        if (read == END_BYTE) {
            Message *message = new Message(buffer);
            processMessage(message);
            delete message;

            position = INVALID_POSITION;
        } else {
            //Error
            position = INVALID_POSITION;
        }
    }
}
