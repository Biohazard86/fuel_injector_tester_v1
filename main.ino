
// LIBRARY
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//DEFINITIONS
#define MAXRPM 7000
#define MAXDUTY 85

unsigned int pin_inj_1 = 1; // pin 2 to the injector n1
unsigned int led_1 = 5;      // LED for the injector pulse
const int analogPin = A0;    // For the potentiometer
const int next_button = 2;    // For the next button
const int ok_button = 3;      // For the ok button

//VARS
 int selector_menu=0;
 boolean start = false;

// i2c LCD
LiquidCrystal_I2C lcd(0x3F,16,2);  //For my 12x2 LCD i2c screen

// ------------------------------------------------------------------

void setup() {
  // VARS
  int selector_value, hz_value=0, duty_cyle=0, rpm_engine, value, test_secs=0;
 


  //CODE
  Serial.begin(9600); // open the serial port at 9600 bps:
  
  pinMode(pin_inj_1, OUTPUT);    // pin 2 as an output
  digitalWrite(pin_inj_1, LOW);

  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);

  pinMode(next_button, INPUT);
  pinMode(ok_button, INPUT);

  attachInterrupt(digitalPinToInterrupt(next_button), add_next_button(), RISING);
  attachInterrupt(digitalPinToInterrupt(ok_button), add_ok_button(), RISING);

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
    // if =6 then 0
    if(selector_menu == 6){
      selector_menu = 0;
      }
  }


// ------------------------------------------------------------------


void add_ok_button(){
  // only changes the boolean
    if(start == true){
      start = false;
    }
    if(start == false){
      start = true;  
    }
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

int hz_to_rpm(hz_value){
  int rpm;

  // HZ = pulses per sec
  // RPM = Revs per min
  // Hz * 60 = RPM
  rpm=hz_value*60;
  return rpm;
  }

  
// ------------------------------------------------------------------

int screen1(int start, int hz_value, int rpm_engine, int duty_cycle){
  clear();
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

int showhz(int hz_value){
  clear();
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

int show_cycle(int duty_cycle){
  clear();
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

void wait_ms(int ms){
  int micros_wait, micros_init, micros_end;

  micros_wait = ms*1000;
  micros_init = micros();
  micros_end = micros_init+micros_wait;
  do{
    }while( micros() < (micros_end) );
    
  
  }

// ------------------------------------------------------------------
void test(int secs, int hz, int rpm, int dc){
  unsigned long time_star, time_end;
  int ms,ms_low;

  ms = (1/hz*1000) * (dc/100);
  ms_low= (1/hz*1000) -ms;
  clear();
  lcd.setCursor(0, 0);
  lcd.print("RUNING TEST");
  lcd.setCursor(0, 1);
  lcd.print(secs);
  lcd.setCursor(3, 1);
  lcd.print("SECS");
  
  time_start = micros();
  time_end = time_star + (secs * 1000000);
  do{
    if(secs == 0){
        time_end = (micros() +100000);
      }
      //Do a pulse of ms milisends in the pin 1
      do_pulse(ms, 1);
      // and wait ms_low ms to the next pulse
      wait_ms(ms_low);
    
    }
    while((start) && (micros() < time_end))
  }


// ------------------------------------------------------------------

int run_test(int start, int hz_value, int rpm_engine, int duty_cycle){
  clear();
  // if the test is running we show the info of the main menu
  if(start == 1){
    //screen1(start, hz_value, rpm_engine, duty_cycle);
    delay(1000);
    test(0, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN INFINITE TEST");
    }
  }

// ------------------------------------------------------------------

int run_test_10(int start, int hz_value, int rpm_engine, int duty_cycle){
  clear();
  // if the test is running we show the info of the main menu
  if(start == 1){
    //screen1(start, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN 10s TEST");
    }
  }

// ------------------------------------------------------------------

int run_test_secs(int start, int hz_value, int rpm_engine, int duty_cycle, int secs){
  clear();
  // if the test is running we show the info of the main menu
  if(start == 1){
    //screen1(start, hz_value, rpm_engine, duty_cycle);
  }else{
    // Show the info of the test
    lcd.setCursor(0, 0);
    lcd.print("RUN ");
    lcd.print(secs);
    lcd.print("s TEST");
    }
  }

// ------------------------------------------------------------------
void loop() {
  

  // calculate the rpm trow the hz
  rpm_engine = hz_to_rpm(hz_value);

  // Shows menu in the screen
  switch(selector_menu){
    case 0:
      // We call the function 
      screen1(start, hz_value, rpm_engine, duty_cycle);
   
    case 1:
      //Read the input
      value = analogRead(analogPin); 
      hz_value = map(value, 0, 1023, 0, (MAX_RPM/60));
      // We call the function 
      show_hz(hz_value, rpm_engine);
    case 2:
      // Read the input 
      value = analogRead(analogPin); 
      duty_cycle = map(value, 0, 1023, 0, MAX_DUTY);
      // We call the function 
      show_cycle(duty_cycle);
    case 3:
      
      // We call the function 
      run_test(start, hz_value, rpm_engine, duty_cycle);
    case 4:
      // Call the function
      run_test_10(start, hz_value, rpm_engine, duty_cycle);
    case 5:
      //Read the input
      value = analogRead(analogPin); 
      test_secs = map(value, 0, 1023, 0, 120);
      // Call the function
      run_test_secs(start, hz_value, rpm_engine, duty_cycle, test_secs);
  }

  start=change_to_false(start);


  if( (selector_menu == 3) && (start == true) ){
    run_test(start, hz_value, rpm_engine, duty_cycle)
    }
    
  start=change_to_false(start);

  if( (selector_menu == 4) && (start == true) ){
    run_test_10(start, hz_value, rpm_engine, duty_cycle)
    }

  start=change_to_false(start);

  if( (selector_menu == 5) && (start == true) ){
    run_test(start, hz_value, rpm_engine, duty_cycle)
    }
  start=change_to_false(start);

}
