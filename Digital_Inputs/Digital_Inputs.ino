#include <KerbalSimpit.h>
#include <PayloadStructs.h>
#include <LiquidCrystal.h>
#include <LedControl.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //define pins for lcd-display
int ledPin = 5; //define pin for brake-pin
int conPin = 6; //define pin for status-pin

//DataIn    pin 4
//CLK       pin 3
//CS        pin 2
LedControl lc = LedControl(4,3,2,1);  //define pins for four-seven-segment display (1 isn't a pin)

float heading;
byte currentActionStatus = 0;
byte leds = 0;
KerbalSimpit mySimpit(Serial);  //initialise simpit library

void setup() 
{
  lcd.begin(16, 2);
  lcd.print("Bonjour");
  delay(1000);
  lcd.clear();
  lcd.print("KSP loading...");
  lc.shutdown(0, false);  lc.setIntensity(0, 8);  lc.clearDisplay(0);
  pinMode(conPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(conPin, HIGH);
  Serial.begin(115200);
  while (!mySimpit.init()) {  //wait for confirmed handshake signal
    delay(100);
  }
  digitalWrite(conPin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  lcd.clear();  delay(400);
  lcd.print("Connection");  lcd.setCursor (0, 2);  lcd.print("established!");
  mySimpit.inboundHandler(myCallbackHandler); //define inboundHandler function
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
  mySimpit.registerChannel(AIRSPEED_MESSAGE);
  mySimpit.registerChannel(ALTITUDE_MESSAGE);
  mySimpit.registerChannel(VELOCITY_MESSAGE);
  mySimpit.registerChannel(ROTATION_DATA_MESSAGE); //register incoming data channels
}

void loop()
{
  mySimpit.update();  //call an update on all incoming channels
}

void myCallbackHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case ACTIONSTATUS_MESSAGE:
    if (msgSize = 1){
      currentActionStatus = msg[0];
      if (currentActionStatus & BRAKES_ACTION) {
        digitalWrite(ledPin, HIGH);
      }
      else {
        digitalWrite(ledPin, LOW);
      }
    }
  case AIRSPEED_MESSAGE:
    if (msgSize = 1){
      airspeedMessage airspeed;
      airspeed = parseMessage<airspeedMessage>(msg);

      double integerPart, fractionalPart;
      fractionalPart = modf(airspeed.IAS, &integerPart);

      fractionalPart = floor(fractionalPart * 10000) / 10000;

      int fdig1 = (int)(fractionalPart * 1000);
      int fdig2 = (int)((fractionalPart * 100) - (fdig1 * 10));
      int fdig3 = (int)((fractionalPart * 10) - (fdig1 * 100) - (fdig2 * 10));
      int fdig4 = (int)(fractionalPart - (fdig1 * 0.001) - (fdig2 * 0.01) - (fdig3 * 0.1));

      if (fractionalPart<1234.8) {
        lcd.setCursor(0, 1);   lcd.print("Airspeed:");  lcd.setCursor (0, 2);  lcd.print((fractionalPart*3.6),"km/h");
      }
      if (fractionalPart>1234.8) {
        lcd.setCursor(0, 1);   lcd.print("Airspeed:");  lcd.setCursor (0, 2);  lcd.print((fractionalPart/343), "MACH");
      }
    }
  case ROTATION_DATA_MESSAGE:
    if (msgSize = 1){
      vesselPointingMessage orientation;
      orientation = parseMessage<vesselPointingMessage>(msg);
      heading = orientation.heading;
      int num1, num2, num3;
      num1 = floor(heading/100);
      num2 = floor((heading - num1*100)/10);
      num3 = floor(heading - num1*100 - num2*10);
      lc.setDigit(0, 0, num1, false); lc.setDigit(0, 1, num2, false); lc.setDigit(0, 2, num3, false);
    }
  case ALTITUDE_MESSAGE:
    if (msgSize = 1) {
      altitudeMessage altitude;
      altitude = parseMessage<altitudeMessage>(msg);
      altitudeSealevel = altitude.sealevel;
    }
  case VELOCITY_MESSAGE:
    if (msgSize = 1) {
      velocityMessage velocity;
      velocity = parseMessage<velocityMessage>(msg);
      // velocity.orbital
      // velocity.surface
      if (altitudeSealevel>60000) {
        // -- print velocity.surface to lcd 
      }
      else {
        // -- print velocity.orbital to lcd
      }
    }  
  }
}"# KSP-Arduino-Controller" 
