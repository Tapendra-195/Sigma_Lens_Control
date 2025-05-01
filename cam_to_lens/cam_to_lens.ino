#include "Config.h"
#include "Constants.h"
//#include "DebugTools.h"
#include "Message.h"
#include "Message03.h"
#include "Message04.h"
#include "Message05.h"
#include "Message06.h"

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

int bodyToLensBufferPosition = INVALID_POSITION;
int lensToBodyBufferPosition = INVALID_POSITION;
int packetLength = INVALID_POSITION;
byte bodyToLensBuffer[MAX_BUFFER_SIZE] = {0};
byte lensToBodyBuffer[MAX_BUFFER_SIZE] = {0};


byte inited = 0;
int unusedClockWindows = 0;
bool highspeedMode = false;

void startMessage() {
    //wait for lens_cs to go LOW
    while (digitalRead(PIN_LENS_CS_BODY)) {
        delayMicroseconds(1);
    }
    digitalWrite(PIN_BODY_CS_LENS, HIGH);
    printLenCS(true);
    delayMicroseconds(40);  
}

void finishMessage() {
    Serial1.flush();
    delayMicroseconds(40);
    digitalWrite(PIN_BODY_CS_LENS, LOW);
    printLenCS(false);
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
    /*
    if (inited == 1 && highspeedMode){
      delayMicroseconds(6);
      digitalWrite(PIN_LENS_CS_BODY, HIGH);
      delayMicroseconds(1088);
      digitalWrite(PIN_LENS_CS_BODY, LOW);
      inited++;
    }
  */
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
    printLenCS(digitalRead(PIN_LENS_CS_BODY));
}

void printLenCS(bool val) {
    if (inited < INIT_COMPLETE || DEBUG_TIMING) {
        Serial.print(val ? "[L:" : "[l:");
        Serial.print(micros());
        Serial.println("]");
    }
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
    //Serial.print("inside nonzero inited \n");
    if (Serial1.available() > 0) {
        //Serial.print("inside serial2 available \n");
        processByte(Serial1.read(), lensToBodyBuffer, lensToBodyBufferPosition, lensToBody);
    }
  /*
    if (Serial2.available() > 0) {
        processByte(Serial2.read(), bodyToLensBuffer, bodyToLensBufferPosition, bodyToLens);
    }
  */
  }

}


void setLowspeedMode(){
  if(highspeedMode == false) {
    return;
  }
  Serial1.begin(750000, SERIAL_8N1);
  //Serial2.begin(750000, SERIAL_8N1);
  highspeedMode = false;
}

void setHighspeedMode(){
  if(highspeedMode) {
    return;
  }
  Serial1.begin(1500000, SERIAL_8N1);
  //Serial2.begin(1500000, SERIAL_8N1);
  highspeedMode = true;
}

void processMessage(Message *input) {
    Serial.print("Process message called  with message type ");
    //, input->messageType);
    if (LISTEN_ONLY) {
      unusedClockWindows = 0;
      if(input->messageType == 0x0C ){
        // We'll do this on the body message to the lens so we'll not see the reply from the lens
        setHighspeedMode(); 
      }
      return;
    }

    switch (input->messageType) {
        case 0x05:
            //message05->updateBasedOn03(input);
            break;
        case 0x06:
            // Send both 0x05 and 0x06 back
            startMessage();
            //message03->sequenceNumber = input->sequenceNumber + 1;
            // Simulate a changing aperture
            //message05->setAperture(message05->aperture + 1);
            count++;
            if(count%20 == 0){
              //message03->incrementAperture();
              message04->moveFocus(0xF63F);//0x09C1);//0xF63F);//0x0008);//0xFFF8);
            }
            message03->update();
            message03->prepForSending();

            Serial1.write(message03->messageBuffer, message03->messageLength);
            finishMessage();
            //flushDebugOutputBuffer();

            delayMicroseconds(10);
            

            startMessage();
            //message04->sequenceNumber = input->sequenceNumber + 1;
            message04->update();
            message04->prepForSending();
            Serial1.write(message04->messageBuffer, message04->messageLength);
            finishMessage();
            break;

        //INIT MESSAGES
        case 0x01:
            startMessage();
            Serial1.write(init07, sizeof(init07));
            //writeSerial1Debuggable(init07, sizeof(init07));
            finishMessage();
            break;
        case 0x07:
            //Serial.print("inside 07 \n");
            startMessage();
            Serial1.write(init0C, sizeof(init0C));
            //writeSerial1Debuggable(init0C, sizeof(init0C));
            finishMessage();
            break;
        case 0x0C:
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

        //High speed mode set, Now do initialization
        case 0x0B:
            startMessage();
            Serial1.write(init3F, sizeof(init3F));
            //writeSerial1Debuggable(init3F, sizeof(init3F));
            finishMessage();
            break;

        case 0x3F:
            startMessage();
            Serial1.write(init3D, sizeof(init3D));
            //writeSerial1Debuggable(init3D, sizeof(init3D));
            finishMessage();
            break;

        case 0x3D:
            startMessage();
            Serial1.write(init08, sizeof(init08));
            //writeSerial1Debuggable(init08, sizeof(init08));
            finishMessage();
            break;

        case 0x08:
            startMessage();
            Serial1.write(init09, sizeof(init09));
            //writeSerial1Debuggable(init09, sizeof(init09));
            finishMessage();
            break;

        case 0x09:
            startMessage();
            Serial1.write(init0D, sizeof(init0D));
            finishMessage();
            break;

        case 0x0D:
            startMessage();
            Serial1.write(init10, sizeof(init10));
            finishMessage();
            
            break;

        case 0x10:
            startMessage();
            Serial1.write(init0A, sizeof(init0A));
            //writeSerial1Debuggable(init0A, sizeof(init0A));
            finishMessage();
        
            break;

        case 0x0A:
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

            if (direction == lensToBody) {
                Serial.print(LENS_TO_BODY);
            } else {
                Serial.print(BODY_TO_LENS);
            }
  
            position = INVALID_POSITION;
        } else {
            //Error
            position = INVALID_POSITION;
        }
    }
}
