#include <KerbalSimpit.h>
#include <PayloadStructs.h>
#include <LiquidCrystal.h>
#include <LedControl.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int ledPin = 5;
int conPin = 6;

//DataIn    pin 4
//CLK       pin 3
//CS        pin 2
LedControl lc = LedControl(4,3,2,1);
unsigned long delaytime = 400;

int Digits[10] = {B01111110, B00110000, B1101101, B01111001, B00110011, B01011011, B01011111, B01110000, B01111111, B01111011};
int dig1;   int dig2;   int dig3;   int dig4;

byte currentActionStatus = 0;
byte leds = 0;
KerbalSimpit mySimpit(Serial);

int iterations = 0;
const int maxIterations = 10;
unsigned long previousMillis = 0;
const long interval = 1000;

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
  while (!mySimpit.init()) {
    delay(100);
  }
  digitalWrite(conPin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  lcd.clear();  delay(400);
  lcd.print("Connection");  lcd.setCursor (0, 2);  lcd.print("established!");
  mySimpit.inboundHandler(myCallbackHandler);
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
  mySimpit.registerChannel(AIRSPEED_MESSAGE);
  //fourSevenSegmentUpdater();
}

void loop()
{

  mySimpit.update();
/*
  fourSevenSegmentUpdater();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    iterations++;
  }
  if (iterations >= maxIterations) {
    iterations = 0;
  }
*/
}
/*
void fourSevenSegmentUpdater() {
  for (int i=0;i<9999;i++) {
    dig1 = (int)i/1000;
    dig2 = ((int)i/100*100-(int)i/1000*1000)/100;
    dig3 = ((int)i/10*10-(int)i/100*100)/10;
    dig4 = i-(int)i/10*10;
    lc.setRow(0, 0, Digits[dig1]);
    lc.setRow(0, 1, Digits[dig2]);
    lc.setRow(0, 2, Digits[dig3]);
    lc.setRow(0, 3, Digits[dig4]);
    delay(delaytime);
  }
}
*/
void myCallbackHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case ACTIONSTATUS_MESSAGE:
    if (msgSize = 1){
      currentActionStatus = msg[0];
      // actionGroup = parseMessage<cagStatusMessage>(msg); <-- doesen't work since 1. cagStatusMessage isn't used 2. I'm not sure is cagStatusMessage even exists and 3. All it would parse is a bit.
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
      if (airspeed.IAS<1234.8) {
        lcd.setCursor(0, 1);   lcd.print("Airspeed:");  lcd.setCursor (0, 12);  lcd.print((airspeed.IAS*3.6),"km/h");
      }
      if (airspeed.IAS>1234.8) {
        lcd.setCursor(0, 1);   lcd.print("Airspeed:");  lcd.setCursor (0, 12);  lcd.print((airspeed.IAS/343), "MACH");
      }
    }
  }
}"# KSP-Arduino-Controller" 
