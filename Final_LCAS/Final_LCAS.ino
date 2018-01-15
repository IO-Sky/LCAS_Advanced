/*Software done by: IO Sky*/
/*Date: 31/05/2017*/
#include "Final_LCAS_Header.h"

/*Program Setup*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);  //Initialize board rate
  mySerial.begin(19200); //Default serial port setting for the GPRS modem is 19200bps 8-N-1
  mySerial.print("\r");
  delay(1000); //Wait for a second while the modem sends an "OK"

  //Ultrasonic sensor initializations
  pinMode(trigPin1, OUTPUT); //Initialize Ultrasonic sensor, sends trigger and receives echo
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); //Initialize Ultrasonic sensor, sends trigger and receives echo
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); //Initialize Ultrasonic sensor, sends trigger and receives echo
  pinMode(echoPin3, INPUT);

  //Relays
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);
  
   //LEDs and other GPIO
  pinMode(redLEDPin1, OUTPUT); // Use LED indicator (if required)
  pinMode(redLEDPin2, OUTPUT); // Use LED indicator (if required)
  pinMode(redLEDPin3, OUTPUT); // Use LED indicator (if required)
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(orange, OUTPUT);
  pinMode(tempBeep, OUTPUT);
  pinMode(GSMpowerKey, OUTPUT);
  pinMode(siren, OUTPUT);
  digitalWrite(green, HIGH);
  digitalWrite(GSMpowerKey, LOW);

 // initial = EEPROM.read(4);
  if (initial == 'reset') //Load default memory location, which is '5'
  {
    Serial.print("yes");
    for (int j = 0; j < 4; j++) { //Write default '1234' on EEPROM memory
      EEPROM.write(j, j + 49);
    }
    for (int j = 0; j < 4; j++) { //Load '1234' from EEPROM and put into pass[j]
      pass[j] = EEPROM.read(j);
      Serial.print("Characters in EEPROM: "); //Should print '1234'
      Serial.println(pass[j]);
      //  EEPROM.write(4, 50); //Change memory condition to '1' to remember after
    }
  }
  else {
    Serial.print("no"); //Otherwise read whatever is stored on EEPROM and load it to pass[j]
    for (int j = 0; j < 4; j++) {
      pass[j] = EEPROM.read(j);
      Serial.print("Characters in EEPROM: ");
      Serial.println(pass[j]);
    }
  }
  if(pass[0] == '1'){if(pass[1] == '2'){if(pass[2] == '3'){if(pass[3] == '4'){
    digitalWrite(orange, HIGH);digitalWrite(green, HIGH);digitalWrite(blue, HIGH);
  }}}}
  else{
    digitalWrite(green, HIGH);
  }
  Serial.println("Enter Password: ");
  
}
/*===========================================================================================================================================================================*/

/*Functions*/
void sendsmstext()
{
  mySerial.print("AT+CMGF=1\r"); //Because we want to send the SMS in text mode
  delay(1000);
  mySerial.print("AT+CMGS=\"+27796140566\"\r"); //Start accepting the text for the message
  delay(1000);
  mySerial.print("Intrusion Detected, Alarm System Activated \r"); //The text for the message
  Serial.print("Sent message to phone..");
  delay(1000);
  mySerial.write(0x1A); //Equivalent to sending Ctrl+Z, executes sending the message
  mySerial.println("AT+CNMI=2,2,0,0,0\r");
  delay(200);
}


//*********SMS Read from Cellphone Functions*********/
void GprsReadSmsStore( String SmsStorePos ){
  // Serial.print( "GprsReadSmsStore for storePos " );
  // Serial.println( SmsStorePos );
  mySerial.print( "AT+CMGR=" );
  mySerial.println( SmsStorePos );
}
void ClearGprsMsg(){
  msg = "";
}
void ProcessGprsMsg()
{
  // EN: SMS store readed through UART (result of GprsReadSmsStore request)
  if( msg.indexOf( "+CMGR:" ) >= 0 )
  {
    // EN: Next message will contains the BODY of SMS
    SmsContentFlag = 1;
    // EN: Following lines are essentiel to not clear the flag!
    ClearGprsMsg();
    return;
  }
  ClearGprsMsg();
  SmsContentFlag = 0;
}

void smsRead()
{
            Serial.print("mySerial.available():");
            Serial.println(mySerial.available());
            if(mySerial.available()>=0)
            {
              Serial.println("mySerial Available ");
              // n = 1;
              /* digitalWrite(ledPin, HIGH);
              delay(150);
              digitalWrite(ledPin, LOW);
              delay(150);
              Serial.println("mySerial Available: ");
              */
              char SerialInByte;
              SerialInByte = (unsigned char)mySerial.read();
              //Serial.print("mySerial.available(): ");
              //Serial.println(mySerial.available());
             // Serial.print("SerialInByte: ");
              Serial.print(SerialInByte);
              if(SerialInByte == 13)
              {
                //digitalWrite(ledPin, HIGH);
                Serial.println("Buffer Complete");
                ProcessGprsMsg();
              }
              if( SerialInByte == 10 ){
                // EN: Skip Line feed
              }
              else
              {
                // EN: store the current character in the message string buffer
                msg += String(SerialInByte);
              }
              //Serial.println("Loading msg: " );
              //Serial.print(msg);

              if(msg.indexOf("DISARM") >= 0 )
              {
                Serial.println( "escape = 1");
                greenLight();
                escape = 1;
                delay(2000);
                Serial.println( "Remotely Disarmed");
                Serial.println( "Enter Password: ");
                return;
              }
            }
}

void twominutetimer()
{
     unsigned long time;
     time = millis();
     while(millis() - time <= 10000) //while time is less than 10 seconds
     {
        digitalWrite(siren, HIGH);
        if(time == 999 || time == 1999)
        {
          time = time + 1;
        }
        //prints time since program started
        Serial.println(millis() - time);
     }
}

void beeper()
{
  digitalWrite(redLEDPin1, HIGH);
  digitalWrite(redLEDPin2, HIGH);
  digitalWrite(redLEDPin3, HIGH);
  digitalWrite(tempBeep, HIGH);
  delay(150);
  digitalWrite(redLEDPin1, LOW);
  digitalWrite(redLEDPin2, LOW);
  digitalWrite(redLEDPin3, LOW);
  digitalWrite(tempBeep, LOW);
}

void greenLight()
{
  digitalWrite(green, HIGH);
  digitalWrite(orange, LOW);
  digitalWrite(blue, LOW);
}

void lightOrange()
{
  digitalWrite(green, LOW);
  digitalWrite(orange, HIGH);
  digitalWrite(blue, LOW);
}

void lightBlue()
{
  digitalWrite(green, LOW);
  digitalWrite(orange, LOW);
  digitalWrite(blue, HIGH);
}

void Buzzer1()
{
    digitalWrite(redLEDPin1, LOW);
    digitalWrite(redLEDPin1, HIGH);
}

void Buzzer2()
{
    digitalWrite(redLEDPin2, LOW);
    digitalWrite(redLEDPin2, HIGH);
}

void Buzzer3()
{
    digitalWrite(redLEDPin3, LOW);
    digitalWrite(redLEDPin3, HIGH);
}

void sensorCalculations()
{
  //Initiating and Calculating Sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance1 = duration1/58.2;
  delayMicroseconds(8); //Preventing Interference, to remove soon

  //Initiating and Calculating Sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance2 = duration2/58.2;
  delayMicroseconds(8); //Preventing Interference, to remove soon

  //Initiating and Calculating Sensor 2
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance3 = duration3/58.2;

  Serial.print("Intruder1: ");
  Serial.println(distance1);

  Serial.print("Intruder2: ");
  Serial.println(distance2);

  Serial.print("Intruder3: ");
  Serial.println(distance3);
}

void change() {
  int j = 0;
  lightOrange();
  Serial.print("Current Password: ");
  while (j < 4)
  {
    char keypin = myKeypad.getKey();
    if (keypin != NO_KEY)
    {
      beeper();
      pass1[j++] = keypin;
      Serial.print(keypin);
    }
    keypin = 0;
  }
  delay(500);

  if ((strncmp(pass1, pass, 4)))
  {
    Serial.println("Wrong Passkey...");
    Serial.println("Contact Manufacturer...");
    beeper();
    delay(2000);
  }
  else
  {
    j = 0;
    Serial.print("New Password: ");
    digitalWrite(green, HIGH);
    beeper();
    delay(250);
    beeper();
    delay(250);
    beeper();
    while (j < 4)
    {
      char keypin = myKeypad.getKey();
      if (keypin != NO_KEY)
      {
        beeper();
        pass[j] = keypin;
        Serial.print(keypin);
        EEPROM.write(j, keypin);
        j++;
      }
    }
    if(j==4)
    {
    Serial.println("Password Reset");
    beeper();
    digitalWrite(green, HIGH);
    digitalWrite(orange, HIGH);
    digitalWrite(blue, HIGH);
    delay(350);
    digitalWrite(orange, LOW);
    digitalWrite(blue, LOW);
    }
  }
    Serial.println("Enter Password: ");
}

void doorDelay()
{
               unsigned long time;
               int delayCount = 0;
               while(delayCount <= 30 && wrongUltraPin <= 2)
               {
                  time = millis();
                  while(millis() - time <= 1000) //while time is less than 10 seconds
                  {
                    //digitalWrite(siren, HIGH);
                    if(time == 999)
                    {
                       time = time + 1;
                    }
                    ultraKeyPressed = myKeypad.getKey();
                    if (ultraKeyPressed != NO_KEY)
                    {
                      ultraPassword[k++] = ultraKeyPressed;
                      Serial.print(ultraKeyPressed);
                    }
                  }
                  delayCount = delayCount + 1;
                  beeper(); //temporary
                  Serial.print("delayCount: ");
                  Serial.println(delayCount);
                  Serial.print("wrongUltraPin: ");
                  Serial.println(wrongUltraPin);
                  if (k == 4)
                  {
                    delay(200);
                    for (int j = 0; j < 4; j++)
                    pass[j] = EEPROM.read(j);

                    if (!(strncmp(ultraPassword, pass, 4)))
                    {
                      delayCount = 110; //Escape loop
                      k = 0;
                    }
                    else if((strncmp(ultraPassword, pass, 4)))
                    {
                      wrongUltraPin = wrongUltraPin + 1;
                      delayCount = 0;
                      k = 0;
                    }
                    //To include alarm set if failed attemp 3 times
                  }
               }
               Serial.print("Wrong Ultra Pin: ");
               Serial.println(wrongUltraPin);
               if(wrongUltraPin > 2)
               {
                    Buzzer1();
                    sendsmstext();
                    twominutetimer();
                    digitalWrite(siren, LOW);
                    delayCount = 0;
                    k = 0;
                    wrongUltraPin = 0;
               }
}
/*===========================================================================================================================================================================*/
void loop() {
  // put your main code here, to run repeatedly:
  //smsRead();
  keyPressed = myKeypad.getKey();
  if (keyPressed == '#') {
    keyPressed = 0;
    beeper();
    Serial.println("Password Changing");
    change();
  }

  if (keyPressed != NO_KEY)
  {
    beeper();
    password[i++] = keyPressed;

    Serial.print(keyPressed);
  }

  if (i == 4)
  {
    delay(200);
    for (int j = 0; j < 4; j++)
      pass[j] = EEPROM.read(j);
    if (!(strncmp(password, pass, 4)))
    {
      Serial.print("Grant Access");
      lightBlue();
      beeper();
      delay(250);
      beeper();
      i = 0;
      escape = 0;
     // doorDelay();
      //Progressive code, EEPROM ->HCSR04 -> GSM SIM900
      while(escape == 0) //while escape is NOT acive
      {
            digitalWrite(blue, HIGH);
            char keypin = myKeypad.getKey();
            if (keypin == NO_KEY)
            {
            sensorCalculations();
            //smsRead();
            if (distance1 <= maximumRange1)
            {
              doorDelay();
            }

            else if (distance2 <= maximumRange2)
            {
              Buzzer2();
              sendsmstext();
              twominutetimer();
              digitalWrite(siren, LOW);
            }

            else if(distance3 <= maximumRange3)
            {
              Buzzer3();
              sendsmstext();
              twominutetimer();
              digitalWrite(siren, LOW);
            }
          }
          keyPressed = myKeypad.getKey();

          if(keyPressed == 'A')
          {
            Buzzer1();
            Buzzer2();
            Buzzer3();
            sendsmstext();
            twominutetimer();
            beeper();
            Serial.print("Panic Alarm");
          }

          else if (keyPressed != NO_KEY)
          {
            beeper();
            password[k++] = keyPressed;
            Serial.print(keyPressed);
          }

          if (k == 4)
          {
            delay(200);
            for (int j = 0; j < 4; j++)
            pass[j] = EEPROM.read(j);

            if (!(strncmp(password, pass, 4)))
            {
              Serial.print("Disarmed");
              escape = 1;
              beeper();
              delay(250);
              beeper();
              k = 0;
              Serial.println("Enter Password: ");
              greenLight();
              }

              else
              {
                Serial.print("B Password Incorrect, Not disarmed. ");
                k = 0;
              }
          }
      }
    }
    else
    {
      Serial.print("A Password Incorrect, Nah fam..");
      digitalWrite(redLEDPin1, HIGH);digitalWrite(redLEDPin2, HIGH);digitalWrite(redLEDPin3, HIGH);digitalWrite(tempBeep, HIGH);
      delay(500);
      digitalWrite(redLEDPin1, LOW);digitalWrite(redLEDPin2, LOW);digitalWrite(redLEDPin3, LOW);digitalWrite(tempBeep, LOW);
      i = 0;
    }
  }
}

