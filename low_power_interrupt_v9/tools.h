#define RXLED            17            // The RX LED has a defined Arduino pin
#define TXLED            30            // The TX LED has a defined Arduino pin

byte led_onboard[3] = {LED_BUILTIN, RXLED, TXLED};    //use arduino on-board led for indicating sleep or wakeup status
byte led_array[9] = {4,5,6, 7,8,9, 10,11,12};    //use arduino on-board led for indicating sleep or wakeup status
bool led_status[3] =  {false, false, false};


void port(bool value, byte ledNumber = 0) {

  byte ledPin = led_array[ledNumber];
  
  if (value == HIGH) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    pinMode(ledPin, INPUT);
  }
  
  led_status[ledNumber] = value;
}

void runthrough(bool value, int sleeptime) {
  for ( byte i = 0; i < 6; i++) {
    port(value, i);
    delay(sleeptime);
  }    
}

void led(bool value, byte ledNumber = 0) {

  byte ledPin = led_onboard[ledNumber];
  
  if (value == HIGH) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    pinMode(ledPin, INPUT);
  }
  
  led_status[ledNumber] = value;
}

void led_onoff(int delaytime, int nrof_blinks=1, byte ledNumber = 0){
  if ( debugmode ) Serial.println(F("Blinking..."));
  
  for (int looping = 0; looping < (nrof_blinks * 2); looping++ ) {
    //cycles the led to indicate that we are no more in sleep mode
    led(!led_status[ledNumber], ledNumber);
    delay(delaytime);
  }
}

void inputs_lowpower() {
    // Set all pin's to input for low power
    for ( uint8_t i = 0; i < 22; i++ ) {
      pinMode(i,INPUT);
    }
}
