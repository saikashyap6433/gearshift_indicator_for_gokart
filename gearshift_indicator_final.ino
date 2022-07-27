
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
int encoder_pin = 2;  // The pin the encoder is connected           
unsigned int rpm;     // rpm reading
volatile byte pulses;  // number of pulses
unsigned long timeold; 
// The number of pulses per revolution
// depends on your index disc!!
unsigned int pulsesperturn = 20;

 void counter()
 {
    //Update count
      pulses++;    
 }

void setup()
 {
   Serial.begin(9600);
   lcd.begin(16,2);
     //Use statusPin to flash along with interrupts
   pinMode(encoder_pin, INPUT);
   
   //Interrupt 0 is digital pin 2, so that is where the IR detector is connected
   //Triggers on FALLING (change from HIGH to LOW)
   attachInterrupt(0, counter, FALLING);
   // Initialize
   pulses = 0;
   rpm = 0;
   timeold = 0;

 }

 void loop()
 {
   if (millis() - timeold >= 1000){  /*Uptade every one second, this will be equal to reading frecuency (Hz).*/
 //lcd.setCursor(0,0);
  //Don't process interrupts during calculations
   detachInterrupt(0);
   //Note that this would be 60*1000/(millis() - timeold)*pulses if the interrupt
   //happened once per revolution
   rpm = (60 * 1000 )/pulsesperturn/ (millis() - timeold)* pulses;
   timeold = millis();
   pulses = 0;
   lcd.clear();
   //Write it out to serial port
   Serial.print("\n");
   lcd.setCursor(0,0);
   Serial.print("RPM = ");
   Serial.println(rpm,DEC);
   lcd.print("RPM = ");
   lcd.println(rpm,DEC);
   //Restart the interrupt processing
   attachInterrupt(0, counter, FALLING);
   lcd.setCursor(0,1);
   if(rpm==0)
   {
    lcd.print("Neutral");
    }
   else if(rpm<360 && rpm>0)
   {
    Serial.print("1st gear");
    lcd.print("1st gear");
    delay(1000);
    }
   else if(rpm>360)
   {
    Serial.print("Shift to 2nd gear");
    lcd.print(" gear up  2nd gear");
    delay(1000);
   }
   else if(rpm>547)
   {
    Serial.print("shift to  3rd gear");
    lcd.print("3rd gear");
    delay(1000);
   }
   else if(rpm>742)
   {
    Serial.print("shift to 4th gear");
    lcd.print("4th gear");
    delay(1000);
   }
   else if(rpm>936)
   {
    Serial.print("shift to 5th gear");
    lcd.print("5th gear");
    delay(1000);
   }
   else if(rpm>936&&rpm<1068)
   {
    Serial.print("You're in 5th gear");
    lcd.print("last gear");
    delay(1000);
   }
   }
  }
