

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

unsigned int pin_inj_1 = 2; // pin 2 to the injector n1
unsigned int led_1 = 1;      // LED for the injector pulse
const int analogPin = A0; 

// i2c LCD
LiquidCrystal_I2C lcd(0x3F,16,2);  //

// ------------------------------------------------------------------

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  
  pinMode(pin_inj_1, OUTPUT);    // pin 2 as an output
  digitalWrite(pin_inj_1, LOW);

  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);

  //Inicializate the LCD
  lcd.init();
  // Turn on the backligth screen
  lcd.backlight();
  // Print in the Screen:
  lcd.setCursor(4, 0);
  lcd.print("Biohazard86");
  lcd.setCursor(2, 1);
  lcd.print("Injector tester");
  delay(3000);  //Sleep 3 secs
  clear();

}

// ------------------------------------------------------------------

void do_pulse(int ms_pulse, int pin){

  unsigned int micros1, micros_pulse;

  micros_pulse = ms_pulse*1000; // When the end of the pulse has to happen in microseconds

  // Put on the injector
  pinMode(pin, HIGH);
  // Read the micros of the MCU
  micros1 = micros();
  
  do{
    // dont do anything until the pulse duration passed
  }while( micros() < (micros1+micros_pulse ) );
  //Then turn off the injector
  pinMode(pin, LOW);
  
  }


// ------------------------------------------------------------------
void loop() {
  int selector_value, hz_value=0, duty_cyle=0, rpm_engine;


  

}
