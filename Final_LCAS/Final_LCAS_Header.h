/*Libraries*/
#include <Keypad.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
#include <EEPROM.h>
/*===========================================================================================================================================================================*/

/*Alarm LEDs and Buzzers*/
#define redLEDPin1 2
#define redLEDPin2 3
#define redLEDPin3 4

/*Siren and Beep*/
#define siren 5
#define tempBeep 7

/*System State LEDs*/
#define blue 6
#define orange 12
#define green 13

/*GSM Power*/
#define GSMpowerFeedBack 7
#define GSMpowerKey 9

/*Ultrasonic Sensor 1*/
#define trigPin1 23 // Echo Pin
#define echoPin1 25 // Trigger Pin

/*Ultrasonic Sensor 2*/
#define trigPin2 29 // Echo Pin
#define echoPin2 27 // Trigger Pin

/*Ultrasonic Sensor 3*/
#define trigPin3 33 // Echo Pin
#define echoPin3 31 // Trigger Pin

/*Relay Module*/
#define Relay1 47
#define Relay2 49
#define Relay3 51
#define Relay4 53
/*===========================================================================================================================================================================*/

/*Proximity Ranges, US Calculations, Password Arrays, Main Variables*/
float maximumRange1 = 10; // Maximum range needed sensor 1
float maximumRange2 = 10; // Maximum range needed sensor 2
float maximumRange3 = 10; // Maximum range needed sensor 3

float duration1, duration2, duration3, distance1, distance2, distance3; // Duration used to calculate distance
char password[4];
char ultraPassword[4];
char pass[4], pass1[4];
char GSMpassword[4];

int i = 0;
int j = 0;
int k = 0;
int escape = 0;
int wrongUltraPin = 0;
char initial = 'usinguserpin';
char keyPressed = 0;
char leavedelay = 'D';
char ultraKeyPressed = 0;

int SmsContentFlag = 0;
String msg = String("");

/*4x4 Membrane*/
const byte numRows = 4; //Number of rows on the keypad
const byte numCols = 4; //Number of columns on the keypad

/*Keymap defines the ey pressed according to the rows and columns just as appears on the keypad*/
char keymap[numRows][numCols] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

byte rowPins[numRows] = {38, 40, 42, 44}; //Columns 0 to 3
byte colPins[numCols] = {46, 48, 50, 52}; //Rows 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
/*===========================================================================================================================================================================*/

