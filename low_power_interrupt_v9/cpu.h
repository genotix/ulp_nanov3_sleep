#define INTERRUPT_PIN   2
#define INTERRUPT       digitalPinToInterrupt(INTERRUPT_PIN)

/* Divisors 
0000 - 1    16    mhz
0001 - 2     8    mhz
0010 - 4     4    mhz
0011 - 8     2    mhz
0100 - 16    1    mhz
0101 - 32     500 khz
0110 - 64     250 khz
0111 - 128    125 khz
1000 - 256     62 khz
*/
#define FastClock       B00000000
#define SlowClock       B00001000

// #define SlowClock       B00001000

void cpu(bool cpuspeed) {
  if ( cpuspeed == HIGH ) {
    CLKPR = (1<<CLKPCE);  // Tell the AtMega we want to change the system clock
    CLKPR = FastClock;    // 1/256 prescaler = 60KHz for a 16MHz crystal
  } else {
    CLKPR = (1<<CLKPCE);  // Tell the AtMega we want to change the system clock
    CLKPR = SlowClock;    // 1/256 prescaler = 60KHz for a 16MHz crystal      
  }
} 

void disable_wdt() {
  // disable WDT
  // Clear WDRF in MCUSR
  MCUSR &= ~(1 << WDRF);
  // Write logical one to WDCE and WDE
  // Keep old prescaler setting to prevent unintentional time-out
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Turn off WDT
  WDTCSR = 0x00;
}

void disable_uart() {
  UCSR0B = 0x00;
}
  
void disable_adc() {
  ADCSRA &= ~(1 << ADEN);
}

// Make sure not to jump to sleep again
void wake() {
  sleep_disable(); 
  if ( debugmode ) Serial.println(F("Interrupt unset"));
}

void sleepNow() {
  led(LOW);

  sleep_enable();                           // enables the sleep bit in the mcucr register
  noInterrupts();

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);     // SET A PULLUP RESISTOR

  if ( digitalRead(INTERRUPT_PIN) ) {
    if ( debugmode ) Serial.println(F("Will wait for falling input"));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wake, FALLING);      // Use interrupt 0 (pin 2) and run function wakeUp when pin 2 gets LOW    
  } else {
    if ( debugmode ) Serial.println(F("Will wait for rising input"));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wake, RISING);       // Use interrupt 0 (pin 2) and run function wakeUp when pin 2 gets LOW        
  }

  EIFR = bit (INTF0);                       // clear flag for interrupt 0  

  if ( debugmode ) {
    Serial.println(F("Sleeping CPU"));
    Serial.flush();                   // Make sure all serial commands are executed before entering next step
  }

//  cpu(LOW);
  interrupts ();
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt (digitalPinToInterrupt(INTERRUPT_PIN));  // stop LOW interrupt on D2  
  //cpu(HIGH);  
}

/*
 * 
 * DUMMY FUNCTIONS
 * 
 */

void set_interrupt_dummy() {
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);         // INTERRUPT
  bool PortState = digitalRead(INTERRUPT_PIN);

  if ( debugmode ) {
    Serial.print(F("Sensor"));
    Serial.print(F("\t"));
    Serial.println(PortState); 
  }
  led(PortState);

}

void cpu_sleep_dummy() {
  set_interrupt_dummy();
  // Not doing anything
}

// Release all interrupts dummy
void unset_interrupt_dummy() {
  // Not doing anything
}
