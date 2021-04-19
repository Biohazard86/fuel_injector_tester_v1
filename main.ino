unsigned int pin_inj_1 = 2; // pin 2 to the injector n1
unsigned int led_1 = 1;      // LED for the injector pulse

void setup() {
 pinMode(pin_inj_1, OUTPUT);    // pin 2 as an output
 digitalWrite(pin_inj_1, LOW);

 pinMode(led_1, OUTPUT);
 digitalWrite(led_1, LOW);

 

}

void do_pulse(int ms_pulse, int pin){

  unsigned int micros1, micros2;
  

  pinMode(pin, HIGH);
  micros1 = micros();
  do{
    
    }while(micros() < (micros1 + ) )
  pinMode(pin, LOW);
  
  }



void loop() {
  // put your main code here, to run repeatedly:

}
