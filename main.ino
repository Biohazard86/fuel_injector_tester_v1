
// LIBRARY
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//DEFINITIONS
#define MAX_RPM 7000
#define MAX_DUTY 85

unsigned int pin_output = 5; // pin 5 to the injector n1
unsigned int led_1 = 6;      // LED for the injector pulse
const int analogPin = A0;    // For the potentiometer
const int next_button = 2;    // For the next button
const int ok_button = 3;      // For the ok button

//VARS
 int selector_menu=0;
 boolean start_test = false;
 int selector_value, hz_value, duty_cycle, rpm_engine, value, test_secs;

 //CUSTOM CHARACTERS
 byte selected_option[] = {
  B11000,
  B11100,
  B11110,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
 } ;

// i2c LCD
LiquidCrystal_I2C lcd(0x3F,16,2);  //For my 12x2 LCD i2c screen

// ------------------------------------------------------------------

void setup() {
  // VARS
  
 


  //CODE
  Serial.begin(9600); // open the serial port at 9600 bps:
  
  pinMode(pin_output, OUTPUT);    // pin 2 as an output
  digitalWrite(pin_output, LOW);

  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);

  pinMode(next_button, INPUT);
  pinMode(ok_button, INPUT);

  //attachInterrupt(digitalPinToInterrupt(next_button), add_next_button, RISING);
  //attachInterrupt(digitalPinToInterrupt(ok_button), add_ok_button, RISING);

  //Inicializate the LCD
  lcd.init();
  // Turn on the backligth screen
  lcd.backlight();
  // Print in the Screen:
  lcd.setCursor(2, 0);
  lcd.print("Biohazard86");
  lcd.setCursor(0, 1);
  lcd.print("Injector tester");
  delay(2500);  //Sleep 3 secs
  lcd.createChar(0,selected_option);
  lcd.clear();
}

// ------------------------------------------------------------------
// This function changes to false if the parameter is true
boolean change_to_false(boolean var){
    if(var == true){
      return false;
      }
  }


// ------------------------------------------------------------------

// ------------------------------------------------------------------


// managed by the interruption,
void add_next_button(){
    selector_menu++;
    // if =6 then 0 to can manage the menu
    if(selector_menu == 6){
      selector_menu = 0;
      }
  }


// ------------------------------------------------------------------
// To controll the ok button
void add_ok_button(){
  // only changes the boolean
    if(start_test == true){
      start_test = false;
    }
    if(start_test == false){
      start_test = true;  
    }
  }


// ------------------------------------------------------------------
// this function generates a pulse. Recive the miliseconds of the duration and the pin trow th pulse has to be send

void do_pulse(long int us_wait){
  
  long int micros_wait;
  long int micros_init, micros_end;
  long int i;

  

  //micros_wait = ms * 1000;   //miliseconds to microseconds
  micros_init = micros();  // Read the microseconds from the MCU
  micros_end = micros_init + us_wait; //Calculate when the end is
    Serial.print("\n US\n");
    Serial.print(us_wait);
    Serial.print("\nMicros END\n");
    Serial.print(micros_end);
    Serial.print("\nMicros INIT\n");
    Serial.print(micros_init);

    pinMode(pin_output, HIGH);
    Serial.print("\nGO HIGH\n");
    do{
      i = micros();
    }while( i < micros_end );
    pinMode(pin_output, LOW);
    Serial.print("\nEXIT HIGH\n");
    
    
  
  }


// ------------------------------------------------------------------
// This function converts the hz selected to RPM to can know the "real" RPMs of the engine.
int hz_to_rpm_conversion(){
  int rpm;

  // HZ = pulses per sec
  // RPM = Revs per min
  // Hz * 60 = RPM
  rpm=hz_value*60;
  return rpm;
  }

  
// ------------------------------------------------------------------
// This shows the main screen of the program
// All data are showed
int screen1(int start_test, int hz_value, int rpm_engine, int duty_cycle){
  lcd.clear();
  // 1st line
  lcd.setCursor(0, 0);
  lcd.print("HZ");
  lcd.setCursor(3, 0);
  lcd.print(hz_value);

  lcd.setCursor(8, 0);
  lcd.print("RPM");
  lcd.setCursor(12, 0);
  lcd.print(rpm_engine);
  
  // 2nd line
  lcd.setCursor(0, 1);
  lcd.print("DUTY CYCLE");
  lcd.setCursor(12, 1);
  lcd.print(duty_cycle);
  lcd.setCursor(15, 1);
  lcd.print("%");
  }

// ------------------------------------------------------------------
// This function shows the HZ selected. You can choose another value
int show_hz(){
  lcd.clear();
  // 1st line
  lcd.setCursor(0, 0);
  lcd.print("HZ");
  lcd.setCursor(3, 0);
  lcd.print(hz_value);

  lcd.setCursor(8, 0);
  lcd.print("RPM");
  lcd.setCursor(12, 0);
  lcd.print(rpm_engine);
  
  // 2nd line
  lcd.setCursor(0, 1);
  lcd.print("DUTY CYCLE");
  lcd.setCursor(12, 1);
  lcd.print(duty_cycle);
  lcd.setCursor(15, 1);
  lcd.print("%");
  }

// ------------------------------------------------------------------
// This function shows the duty cycle you select. You can change it
int show_cycle(int duty_cycle){
  lcd.clear();
  // 1st line
  lcd.setCursor(2, 0);
  lcd.print("DUTY CYCLE");
  //2nd line
  lcd.setCursor(2, 1);
  lcd.print(duty_cycle);
  lcd.setCursor(6, 0);
  lcd.print("%");
  }
// ------------------------------------------------------------------
// This function wait ms ms that you pass as an argumen
void wait_ms(long int us_wait){
  
  long int micros_wait;
  long int micros_init, micros_end;
  long int i;

  

  //micros_wait = ms * 1000;   //miliseconds to microseconds
  micros_init = micros();  // Read the microseconds from the MCU
  micros_end = micros_init + us_wait; //Calculate when the end is
  Serial.print("\n US\n");
  Serial.print(us_wait);
  Serial.print("\nMicros END\n");
  Serial.print(micros_end);
  Serial.print("\nMicros INIT\n");
  Serial.print(micros_init);

    Serial.print("\nGO WAIT\n");
    do{
      i = micros();
    }while( i < micros_end );
    //delay(ms);
    //delayMicroseconds(micros_wait);
    Serial.print("\nEXIT WAIT\n");
    
  
  }

// ------------------------------------------------------------------
// Function to test x secs the injectors
// calculate the high and low times per siganl and sends to the functions
void test(int secs, int hz, int rpm, int dc){
  unsigned long time_start_test, time_end;
  double ms;
  int ms_high, ms_low;
  long int send_us_low,send_us_high;
  
  ms_high = (pow(hz, -1)*dc) * 10;  //miliseconds with the duty cycle correction (High signal)
  ms_low= ((pow(hz, -1)*1000) - ms_high);      //miliseconds of the low signal of the cycle
  send_us_high = long(ms_high) * 1000;
  send_us_low = long(ms_low) * 1000;
  
  //Serial.print("\nHZ\n");
  //Serial.print(hz);
  //Serial.print("\nDC\n");
  //Serial.print(dc);

  //Serial.print("\nPOW\n");
  //Serial.print((pow(hz, -1)*1000));
  //Serial.print("\nSEND US \n");
  //Serial.print(send_us_low);
  //Serial.print("\nMS\n");
  //Serial.print(ms_high);
  //Serial.print("\nMS_LOW\n");
  //Serial.print(ms_low);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RUNING TEST");
  lcd.setCursor(0, 1);
  lcd.print(secs);
  lcd.setCursor(3, 1);
  lcd.print("SECS");
  
  time_start_test = micros();    // read the microseconds from the MCU
  time_end = time_start_test + (secs * 1000000);    // calc all the time of the test

  //Serial.print("TIME START\n");
  //Serial.print(time_start_test);
  //Serial.print("TIME END\n");
  //Serial.print(time_end); 
  //lcd.clear();
  
  
  do{
    // if the time is 0 that is that is an infinite test
    //if(secs == 0){
    //  time_end = (micros() +1000);
    //}
      //Do a pulse of ms milisends in the pin 1
      do_pulse(send_us_high);
        // and wait ms_low ms to the next pulse
      wait_ms(send_us_low);
      
      
    }
    while( (micros() < time_end ));
    lcd.clear();
  }


// ------------------------------------------------------------------
// This function is to run an infinite test
int run_test(int start_test, int hz_value, int rpm_engine, int duty_cycle){
  lcd.clear();
  // if the test is running we show the info of the main menu
  if(start_test == 1){
    //screen1(start_test, hz_value, rpm_engine, duty_cycle);
    delay(1000);
    test(0, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN INFINITE TEST");
    }
  }

// ------------------------------------------------------------------
// this is to run a 10 secs test
int run_test_10(int start_test, int hz_value, int rpm_engine, int duty_cycle){
  lcd.clear();
  // if the test is running we show the info of the main menu
  if(start_test == 1){
    //screen1(start_test, hz_value, rpm_engine, duty_cycle);
    delay(1000);
    test(10, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN 10s TEST");
    }
  }

// ------------------------------------------------------------------
// THis runs a X secs test
int run_test_secs(int start_test, int hz_value, int rpm_engine, int duty_cycle, int secs){
  lcd.clear();
  // if the test is running we show the info of the main menu
  if(start_test == 1){
    //screen1(start_test, hz_value, rpm_engine, duty_cycle);
    delay(1000);
    test(secs, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN ");
    lcd.print(secs);
    lcd.print("s TEST");
    }
  }

// ------------------------------------------------------------------
void show_menu(){
  lcd.setCursor(1, 0);
  lcd.print("HZ ");
  lcd.setCursor(4, 0);
  lcd.print(hz_value);
  
  lcd.setCursor(10, 0);
  lcd.print("DC");
  lcd.setCursor(13, 0);
  lcd.print(duty_cycle);
  lcd.setCursor(15, 0);
  lcd.print("%");
  //-------------------
  lcd.setCursor(1, 1);
  lcd.print("T ");
  lcd.setCursor(3, 1);
  lcd.print(test_secs);
  lcd.setCursor(5, 1);
  lcd.print("s");
  
  lcd.setCursor(11, 1);
  lcd.print("OK");
  }

// ------------------------------------------------------------------

void loop() {
  int i;
  
  rpm_engine = hz_to_rpm_conversion();
  show_menu();

  if(digitalRead(next_button) == HIGH){
    selector_menu++;
    delay(250);
    }

    if(selector_menu == 4){
      selector_menu = 0;
      }
//----------------------------------------
   if(selector_menu == 0){
      value = analogRead(analogPin); 
      hz_value = map(value, 100, 1023, 0, 71);  // (MAX_RPM/60)
      if(hz_value < 10){
        lcd.setCursor(5, 0);
        lcd.print(" ");
        }
      if(hz_value < 0){
        hz_value =0;
        }
      
      }

//----------------------------------------
    if(selector_menu == 1){
      value = analogRead(analogPin); 
      duty_cycle = map(value, 100, 1023, 0, 80);
      if(duty_cycle < 10){
        lcd.setCursor(14, 0);
        lcd.print(" ");
        }
      if(duty_cycle < 0){
        duty_cycle =0;
        }
      }

//----------------------------------------      
    if(selector_menu == 2){
      // Test duration
      
      value = analogRead(analogPin); 
      test_secs = map(value, 100, 1020, 0, 61);
      if(test_secs < 10){
        lcd.setCursor(4, 1);
        lcd.print(" ");
        }
      if(test_secs < 0){
        test_secs =0;
        }
      }
      
//----------------------------------------      
    if(selector_menu == 3){
      //lcd.createChar(0,selected_option);
      
      
      if(digitalRead(ok_button) == HIGH){
        selector_menu++;
        delay(250);
        //RUN THE TEST
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("STARTING TEST");
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("3");
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("2");
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("1");
        delay(1000);
        // ----------------------------------------
        test(test_secs, hz_value, rpm_engine, duty_cycle);
        // ----------------------------------------
        selector_menu = 0;
        lcd.clear();
      }
      }      
//----------------------------------------
    if(selector_menu == 0){
      lcd.setCursor(0, 0);
      lcd.write(0);
      lcd.setCursor(9, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(10, 1);
      lcd.print(" ");
      }

      if(selector_menu == 1){
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(9, 0);
      lcd.write(0);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(10, 1);
      lcd.print(" ");
      }

      if(selector_menu == 2){
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(9, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(10, 1);
      lcd.print(" ");
      }

      if(selector_menu == 3){
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(9, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(10, 1);
      lcd.write(0);
      }
      
//----------------------------------------
        
  }
