//Nick Turner
//LED Control 
//3-28-2016
//Ver:1.0

//serialinput to string
const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data


// Set the initial state of the LED cathodes.  
int ledState = LOW;
int ledState2 = LOW;
int ledState3 = LOW;
int flag = 0;
// Initialize the LED cathodes to specific PWM pins on the Lilypad.
int ledR = 3;
int ledG = 5;
int ledB = 4;
int ledW = 2;

int brightness = 0;    // how bright the LED is
int color = 0;  //the hex command converted to binary
int fadeAmount = 5;    // how many points to fade the LED by
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;  
unsigned long previousMillis3 = 0;


//The following are commands received from the smartphone, transmitted commands,and stored commands.
unsigned long incomingCommand = 0xFFFFFFFF;
unsigned long outgoingCommand = 0xFFFFFFFF;
unsigned long oldCommand = 0x00000000;
unsigned long newCommand = 0xFFFF05C0;

//Least significant character is is blink rate (0 = no blink, 1 = 100ms blink rate, F = 1.6 second blink rate) 
//2nd character is color (0 = none, 1 = white, 2 = blue, 4 = green, 8 = red, A = purple, C = yellow, F = all on)
//3rd and 4th character is brightness (0 to 255) 
//unsigned long newCommand = Serial.read();
int colorCommand = 0;
int brightCommand = 0;
int blinkCommand = 0;


//Initialize the color variables.
int colorR = 0;
int colorG = 0;
int colorB = 0;
int colorW = 0;

void setup() {
  
  pinMode(ledW, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);   

  Serial.begin(9600);
}

void loop() {

//serial function calls
  recvWithEndMarker();
  showNewData();
  // put your main code here, to run repeatedly:

unsigned long currentMillis = millis();
//Receive: Receives command from smartphone

/*
Serial.println(newCommand);
Serial.println(oldCommand);
delay(1000);*/


/*
//ParseCommand: Check to see if command is new and break the command into the blink, brightness, and color commands. Also stores the current command to check against incoming commands.
//char *ptr;
unsigned long newCommand = Serial.read();
//const char *s = str1.c_str();
//unsigned long newCommand = strtol(s,&ptr,10);
          Serial.println(" ");
          Serial.println(newCommand);
          delay(1000);
*/


if (newCommand >= oldCommand and newCommand != -1){
  oldCommand = newCommand;
  blinkCommand = newCommand%16;
  newCommand = newCommand/16;
  colorCommand = newCommand%16;
  newCommand = newCommand/16;
  brightCommand = newCommand%256;
  newCommand = oldCommand;
  //delay(1000);
}


/*Serial.println("space");
Serial.println(newCommand);
Serial.println(oldCommand);
delay(1000);*/


//-----------------------------------------------------------------------------------------------
//ExecuteCommand: Execute the command





//Set the interval to blink, max of 1.6 seconds, min of 100ms
int interval = blinkCommand*100;
    
//Check to see if LED needs to blink before setting color and brightness
if (currentMillis - previousMillis >= interval and blinkCommand != 0) {
//Color: Changes the color of the LED according to the instructions received. Also sets the desired brighness.
    if(colorCommand == 0x1)
    {

        if (ledState == LOW) {
          analogWrite(ledW, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledW,0);
          ledState = LOW;
        }

    }      
    
    else if (colorCommand == 0x2)
    {
        if (ledState == LOW) {
          analogWrite(ledB, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledB,0);
          ledState = LOW;
        }
    }
    else if (colorCommand == 0x4)
    {
        if (ledState == LOW) {
          analogWrite(ledG, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledG,0);
          ledState = LOW;
        }
    }
    else if (colorCommand == 0x8)
    {
        if (ledState == LOW) {
          analogWrite(ledR, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledR,0);
          ledState = LOW;
        }
    }
    else if (colorCommand == 0xA)
    {
        if (ledState == LOW) {
          analogWrite(ledR, brightCommand);
          analogWrite(ledB, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledR,0);
          analogWrite(ledB,0);
          ledState = LOW;
        }
    }
    else if (colorCommand == 0xC)
    {
        if (ledState == LOW) {
          analogWrite(ledR, brightCommand);
          analogWrite(ledG, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledG,0);
          analogWrite(ledR,0);
          ledState = LOW;
        }
    }
    else if (colorCommand == 0xF)
    {
        if (ledState == LOW) {
          analogWrite(ledR, brightCommand);
          analogWrite(ledG, brightCommand);
          analogWrite(ledB, brightCommand);
          analogWrite(ledW, brightCommand);
          ledState = HIGH;
        } else {
          analogWrite(ledR, 0);
          analogWrite(ledG, 0);
          analogWrite(ledB, 0);
          analogWrite(ledW, 0);
          ledState = LOW;
        }
    }
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  }

  else if (blinkCommand == 0)
  {
    if(colorCommand == 0x1)
    {
          analogWrite(ledW, brightCommand);
    }      
    
    else if (colorCommand == 0x2)
    {
          analogWrite(ledB, brightCommand);
    }
    else if (colorCommand == 0x4)
    {
          analogWrite(ledG, brightCommand);
    }
    else if (colorCommand == 0x8)
    {
          analogWrite(ledR, brightCommand);
    }
    else if (colorCommand == 0xA)
    {
          analogWrite(ledR, brightCommand);
          analogWrite(ledB, brightCommand);
    }
    else if (colorCommand == 0xC)
    {
          analogWrite(ledR, brightCommand);
          analogWrite(ledG, brightCommand);
    }
    else if (colorCommand == 0xF)
    {
          analogWrite(ledR, brightCommand);
          analogWrite(ledG, brightCommand);
          analogWrite(ledB, brightCommand);
          analogWrite(ledW, brightCommand);

    }
  }
}

//-----------------------------------------------------------------------------------------------------------------

//Transmit: This will package the new instruction and transmit it to other devices in range.

//Sleep? Sleeps for a specified amount of time in between commands.

//SERIAL COMS
void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  
  // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.println(receivedChars);
    newData = false;
  }
}
