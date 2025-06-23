#include "../include/Firmware.h"

Firmware::Firmware()
{

  
  pinMode(PIN_LENS_PWR_SW, OUTPUT);
  pinMode(PIN_LOGIC_VCC_SW, OUTPUT);
  pinMode(PIN_LENS_DETECT, INPUT_PULLUP);
  pinMode(PIN_LENS_CS_BODY, INPUT);
  
  //  Serial1.setTX(PIN_TX);
  //Serial1.setRX(PIN_RX);

}

void Firmware::begin()
{
  Serial.begin(115200);
  transitionToIdle();
}

void Firmware::reset()
{

}

// Make these pins input at first so they are high impedence and don't interfere with lens.
void Firmware::transitionToIdle()
{
  Serial1.end();
  pinMode(PIN_TX, INPUT);
  pinMode(PIN_RX, INPUT);
  pinMode(PIN_BODY_POLL_LENS, INPUT);
  pinMode(PIN_BODY_CS_LENS, INPUT);

  digitalWrite(PIN_LOGIC_VCC_SW, LOW);
  digitalWrite(PIN_LENS_PWR_SW, LOW);
  
  mNextState = IDLE;
  mPollLens = true;
  delayMicroseconds(500);//just a random delay.
  
}

void Firmware::transitionToLinkEstablishment()
{
  pinMode(PIN_TX, OUTPUT); //UART hasn't started yet. Still doing link establishment.
  pinMode(PIN_RX, INPUT);
  
  pinMode(PIN_BODY_POLL_LENS, OUTPUT);
  pinMode(PIN_BODY_CS_LENS, OUTPUT);

  digitalWrite(PIN_BODY_POLL_LENS, HIGH);
  digitalWrite(PIN_BODY_CS_LENS, LOW);
  digitalWrite(PIN_TX, LOW);
  digitalWrite(PIN_LOGIC_VCC_SW, HIGH);
  
  mNextState = LINK_ESTABLISHMENT;

  delayMicroseconds(500);//just a random delay.
}

void Firmware::transitionToSpeedNegotiation()
{
  Serial1.setTX(PIN_TX);
  Serial1.setRX(PIN_RX);
  Serial1.begin(750000, SERIAL_8N1);

  mNextState = SPEED_NEGOTIATION;
  delayMicroseconds(500);//just a random delay.

  sendMessage(init01, sizeof(init01));
}

void Firmware::transitionToInitialization()
{
  Serial1.begin(1500000, SERIAL_8N1);
  
  delayMicroseconds(6);
  digitalWrite(PIN_BODY_CS_LENS, HIGH);
  delayMicroseconds(1088);
  digitalWrite(PIN_BODY_CS_LENS, LOW);
  
  //Power Lens motors.
  digitalWrite(PIN_LENS_PWR_SW, HIGH);
  delayMicroseconds(1000);
  
  mNextState = INITIALIZATION;
  
  sendMessage(init0B, sizeof(init0B));
}

void Firmware::transitionToRegularMessaging()
{
  mNextState = REGULAR_MESSAGING;
  mMessage03.reset();
  mMessage04.reset();
  mPollLens = true;
}

void Firmware::transitionToPoweringOff()
{
  mNextState = POWERING_OFF;
  sendMessage(off0A, sizeof(off0A));
}

void Firmware::transitionToOff()
{
  Serial1.end();
  pinMode(PIN_TX, INPUT);
  pinMode(PIN_RX, INPUT);
  pinMode(PIN_BODY_POLL_LENS, INPUT);
  pinMode(PIN_BODY_CS_LENS, INPUT);

  digitalWrite(PIN_LOGIC_VCC_SW, LOW);
  digitalWrite(PIN_LENS_PWR_SW, LOW);

  mPollLens = true;
  mNextState = OFF;

}

void Firmware::run()
{
  switch(mCurrentState)
    {
    case IDLE:
      {
	if(mResetCount > 4)
	  {
	    transitionToOff();
	  }
	else{
	  bool lensDetected = false;
	  if(!digitalRead(PIN_LENS_DETECT))
	    {
	      lensDetected = true;
	      
	    }
	  
	  if(lensDetected)
	    {
	      transitionToLinkEstablishment();
	    }
	}
      }
      break;
    case LINK_ESTABLISHMENT:
      {
	delayMicroseconds(500);//just a random delay.
	//Lens is detected, now pull BODY_CS_LENS HIGH.
	digitalWrite(PIN_BODY_CS_LENS, HIGH);
	
	unsigned long startTime = micros();
	unsigned long delTime = micros() - startTime;
	bool ackReceived = false;
	while(delTime <2000000 && !ackReceived) //wait for 2 sec
	  {
	    //continuously poll lens cs cam. If it goes high, start serial.
	    if(digitalRead(PIN_LENS_CS_BODY))
	      {
		ackReceived = true;
	      }
	    
	    delTime = micros() - startTime;
	  }
	
	if(ackReceived)
	  {
	    digitalWrite(PIN_TX, HIGH);
	    //Ideally BODY_CS_LENS should go low after RX goes high or delTime >2s, but we are not testing thst.
	    
	    //Wait for Rx to go High
	    bool lensUARTOn = false;
	    delTime = micros() - startTime;
	    while(delTime <2000000 && !lensUARTOn) //wait for 2 sec
	      {
		//continuously poll lens cs cam. If it goes high, start serial.
		if(digitalRead(PIN_RX))
		  {
		    lensUARTOn = true;
		  }
		
		delTime = micros() - startTime;
	      }
	    
	    if(lensUARTOn)
	      {
		delayMicroseconds(500);
		digitalWrite(PIN_BODY_CS_LENS, LOW);
		
		//Now wait for lens cs cam to go low
		delTime = micros() - startTime;
		bool readyToTalk = false;
		while(delTime <2000000 && !readyToTalk) //wait for 2 sec
		  {
		    //continuously poll lens cs cam. If it goes high, start serial.
		    if(!digitalRead(PIN_LENS_CS_BODY))
		      {
			readyToTalk = true;
		      }
		    
		    delTime = micros() - startTime;
		  }
		
		if(readyToTalk)
		  {
		    transitionToSpeedNegotiation();
		  }
		else
		  {
		    transitionToIdle();
		    mResetCount ++;
		  }
		
	      }
	    else
	      {
		transitionToIdle();
		mResetCount ++;
	      }
	    
	  }
	else
	  {
	    transitionToIdle();
	    mResetCount ++;
	  }
      }
      break;
    case SPEED_NEGOTIATION:
      {
	if(mMessageFullyRead)
	  {
	    Message input(lensToBodyBuffer);
	    
	    switch (input.getMessageType()) {
	    case 0x01:
	      {
		sendMessage(init07, sizeof(init07));
		break;
	      }
	    case 0x07:
	      {
		sendMessage(init0C, sizeof(init0C));
		break;
	      }
	    case 0x0C:
	      {
		//sets high speed mode (1500000 baud rate)
		transitionToInitialization();
		break;
	      }
	    default:
	      transitionToIdle();
	      mResetCount++;
	      break;
	    }
	    mMessageFullyRead = false;
	  }
      }
      break;
    case INITIALIZATION:
      {
	if(mMessageFullyRead)
	  {
	    Message input(lensToBodyBuffer);
	    
	    switch (input.getMessageType())
	      {
	      case 0x0B:
		{
		  sendMessage(init3F, sizeof(init3F));
		  break;
		}
	      case 0x3F:
		{
		  sendMessage(init3D, sizeof(init3D));
		  break;
		}
	      case 0x3D:
		{
		  sendMessage(init08, sizeof(init08));
		  break;
		}
	      case 0x08:
		{
		  sendMessage(init09, sizeof(init09));
		  break;
		}
	      case 0x09:
		{
		  sendMessage(init0D, sizeof(init0D));
		  break;
		} 
	      case 0x0D:
		{
		  sendMessage(init10, sizeof(init10));
		  break;
		}
	      case 0x10:
		{
		  sendMessage(init0A, sizeof(init0A));
		  break;
		}
	      case 0x0A:
		//Initialization is complete.
		transitionToRegularMessaging();
		break;        
	      default:
		transitionToIdle();
		mResetCount++;
		break;

	      }
	    mMessageFullyRead = false;
	  }
      }
      break;
    case REGULAR_MESSAGING:
      {
	if(mMessageFullyRead)
	  {
	    if(mIsPoweredOn)
	      {
		Message msg(lensToBodyBuffer);
		Message* input = &msg;
		switch (input->getMessageType())
		  {
		  case 0x05:
		    {
		      currentAperture = static_cast<Message05*>(input)->getAperture();
		      uint16_t currentApertureDialValue = static_cast<Message05*>(input)->getApertureDialValue();
		      //If Aperture Dial is Rotated, change Aperture value based on the aperture dial value.
		      if(lastApertureDialValue != currentApertureDialValue)
			{
			  lastApertureDialValue = currentApertureDialValue;
			  mMessage03.setAperture(currentApertureDialValue);
			}
		    }
		    break;
		  case 0x06:
		    {
		      currentLensPos = static_cast<Message06*>(input)->getLensPos();
		      
		      
		      mMessage03.update();
		      mMessage03.prepForSending();
		      sendMessage(mMessage03.mMessageBuffer, mMessage03.getMessageLength());
		      delayMicroseconds(10);
		      		      
		      mMessage04.update();
		      mMessage04.prepForSending();
		      sendMessage(mMessage04.mMessageBuffer, mMessage04.getMessageLength());		      }
		      break;
		    
		  default:
		    transitionToIdle();
		    mResetCount++;
		    break;
		  }
		
	      }
	    else
	      {
		transitionToPoweringOff();
	      }
	    
	    mMessageFullyRead = false;
	  }
      }
      break;
    case POWERING_OFF:
      {
	if(mMessageFullyRead)
	  {
	    Message input(lensToBodyBuffer);
	    
	    switch(input.getMessageType())
	      {
	      case 0x0A:
		sendMessage(off16, sizeof(off16));
		break;
	      case 0x16:
		delayMicroseconds(500);//Random delay.
		//power off power to lens
		transitionToOff();
		break;
	      }
	    
	    mMessageFullyRead = false;
	  }
      }
      break;
    case OFF:
      {
	if(mIsPoweredOn)
	  {
	    transitionToIdle();
	    mResetCount = 0;
	  }
      }
      break;
    }


  if(digitalRead(PIN_LENS_DETECT))
    {
      transitionToIdle();
      mResetCount++;
    }
  
  //Poll Lens
  unsigned long currentTime = micros();
  // Check if it's time to issue the low pulse:
  if (currentTime - lastPulseTime >= totalPeriod) {
    if(mPollLens)
      {
	pollLens();
      }
    // Reset timer
    lastPulseTime = currentTime;
  }
  
  handleFrontEndInput();
  //Read bytes from lens
  if(Serial1.available()>0)
    {
      processByte();
    }
  
  
  mCurrentState = mNextState;
  
}


void Firmware::handleFrontEndInput()
{
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
        mMessage03.setAperture(aperture);
  
      } 
      else if (cmd == "SF") { // Set Focus
        unsigned int targetLensPos = (unsigned int)arg.toInt();
        mMessage04.setLensPos(currentLensPos, targetLensPos);
        Serial.print("ACK: Setting Focus to ");
        Serial.println(targetLensPos); // value after "SA"
      } 
      else if (cmd == "ON") {
	mIsPoweredOn = true;
      } 
      else if (cmd == "OFF") {
	mIsPoweredOn = false;
      } 
      else {
        Serial.println("ACK: Unknown command.");
      }
    }
}

//Sends a polling signal to lens to retrive the current focus and aperture of the lens.
void Firmware::pollLens()
{
  digitalWrite(PIN_BODY_POLL_LENS, LOW);
  delayMicroseconds(lowDuration);
  digitalWrite(PIN_BODY_POLL_LENS, HIGH);
}

void Firmware::sendMessage(const byte* message, int length)
{
  //wait till LENS_CS_BODY goes low
  while (digitalRead(PIN_LENS_CS_BODY)) {
    delayMicroseconds(1);
  }
  digitalWrite(PIN_BODY_CS_LENS, HIGH);
  delayMicroseconds(40);  

  Serial1.write(message, length); //send message

  Serial1.flush();
  delayMicroseconds(40);
  digitalWrite(PIN_BODY_CS_LENS, LOW);
}


void Firmware::processByte()
{
  int read = Serial1.read();
  if (lensToBodyBufferPosition == INVALID_POSITION) 
    {
      if (read == START_BYTE)
	{
	  lensToBodyBufferPosition = 0;
        }
      else
	{
	  return;
	}
    }
  
  lensToBodyBuffer[lensToBodyBufferPosition++] = read;
  if (lensToBodyBufferPosition >= MAX_BUFFER_SIZE)
    {
      //ERROR
      lensToBodyBufferPosition = INVALID_POSITION;
    }
  else if (lensToBodyBufferPosition == 2)
    {
      packetLength = (lensToBodyBuffer[2] << 8) + lensToBodyBuffer[1];
    }
  if (lensToBodyBufferPosition == packetLength)
    {
      if (read == END_BYTE)
	{
	  mMessageFullyRead = true;
	  
	  lensToBodyBufferPosition = INVALID_POSITION;
	}
      else
	{
	  //Error
	  lensToBodyBufferPosition = INVALID_POSITION;
	}
    }
}
