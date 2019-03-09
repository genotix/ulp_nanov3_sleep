#define RXLED            17            // The RX LED has a defined Arduino pin
#define TXLED            30            // The TX LED has a defined Arduino pin

#define max_cycle         3

byte cycle            = 0;
byte led_onboard[3]   = {LED_BUILTIN, RXLED, TXLED};    //use arduino on-board led for indicating sleep or wakeup status
byte led_array[6]     = {4,5,6, 10,11,12};    //use arduino on-board led for indicating sleep or wakeup status
//byte led_array[9]   = {4,5,6, 7,8,9, 10,11,12};    //use arduino on-board led for indicating sleep or wakeup status

//byte led_array[2]     = {4, 10};    //use arduino on-board led for indicating sleep or wakeup status
bool led_status[3]      = {false, false, false};

byte led_count_per_pair = 3;
byte led_array_size     = sizeof(led_array);
byte led_pairs          = (led_array_size / led_count_per_pair);

void port(bool value, byte ledNumber = 0) {

  byte ledPin = led_array[ledNumber];

/*
  if ( debugmode ) {
    if ( value ) {
      Serial.print(F("En"));         
    } else {
      Serial.print(F("Dis"));    
    }
    Serial.print(F("abling pin:"));
    Serial.println(ledPin);
  }
  Serial.flush();
*/

  if (value == HIGH) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    pinMode(ledPin, INPUT);
  }
  
}

void set_all(bool value) {

/*
  if ( debugmode ) {
    Serial.print(F("Array length:"));
    Serial.println(led_array_size);
    Serial.flush();
  }
*/
  
  for ( byte i = 0; i < led_array_size; i++) {
    port(value, i);
  }
  
}

void runthrough(bool value, int switchtime) {
  if ( cycle >= max_cycle ) cycle = 0;

  // Turn the proper LED's on
  for ( byte j = 0 ; j < led_pairs; j++ )
  {
    port(value, (j*led_count_per_pair) + cycle);    
  }

  // Turn the other LED's off
  for ( byte j = 0 ; j < led_pairs; j++ )
  {
    for ( int i = 0; i < led_count_per_pair; i++ ) {
      if ( cycle == i ) 
      { 
        // do nothing ; this led should be on
      } else {
        // turn this led off
        port(LOW, (j*led_count_per_pair) + i);        
      }
    }
  }


  for ( byte j = 0 ; j < led_pairs; j++ )
  {
    port(value, (j*led_count_per_pair) + cycle);    
  }

  cycle++;
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
