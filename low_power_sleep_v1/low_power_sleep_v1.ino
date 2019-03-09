/* 
 *  Taking 4uA in sleep
 */
 
#include <avr/sleep.h> 
#include <avr/power.h> 

#include "LowPower.h"

#define ontime        5       // (x 2 minutes)

#define debugmode     false
#define testmode      false   // You can use the plotter in test mode

#include "tools.h"
#include "cpu.h"

long  counter = 0;

void setup() {

  //disable_wdt();
  disable_adc();
  
  if ( debugmode ) { 
    Serial.begin(115200); 
    Serial.println(F("Booting..."));
    Serial.flush();
  } else {
    disable_uart();             // Disable internal serial logic
  }

  // Disabled sin ce we use some of the inputs
  inputs_lowpower();
  //cpu(LOW);
  
  led_onoff(3,1);
  led_onoff(1,2);
}

void loop() {

      //cpu(LOW);
      noInterrupts();
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      //cpu(HIGH);
      //detachInterrupt (INTERRUPT);      // stop LOW interrupt on D2
      counter ++;

      if ( counter % 4 == 0 ) {
        led_onoff(500,1);
      }
      
      led_onoff(1,2);
}
