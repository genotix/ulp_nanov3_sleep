/* 
 *  Hardware tweaks plus this sketch gives 0.2 uA or 200 nA of deep sleep power draw
 */

#include "LowPower.h"

#include <avr/sleep.h> 
#include <avr/power.h> 

#define debugmode     false
#define testmode      false   // You can use the plotter in test mode

#include "tools.h"
#include "cpu.h"

void setup() {

  disable_wdt();
  disable_adc();
  
  if ( debugmode ) { 
    Serial.begin(115200); 
    Serial.println(F("Booting..."));
  } else {
    disable_uart();             // Disable internal serial logic
  }
  
  inputs_lowpower();
  
  led_onoff(500,1);
  led_onoff(100,2);
}

void loop() {

  if ( testmode ) {
    cpu_sleep_dummy();
    unset_interrupt_dummy();
  } else {
    sleepNow();
  }

  led_onoff(500,1);
  led_onoff(100,2);
}
