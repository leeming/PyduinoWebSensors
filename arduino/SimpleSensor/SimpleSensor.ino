/**
 * Arduino part of the sensor logger. Currently just has
 * LDR and TMP sensors
 *
 * @author Leeming
 */

#include <avr/power.h>
#include <avr/sleep.h>

//Both of these are analog 
int ldrPin =0;
int tmpPin =1;

int baud =9600;

void setup()
{
  Serial.begin(baud);
}


int incomingByte = 0; 
void loop()
{
  if (Serial.available()) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.print("t:");
    Serial.print(getTemp(tmpPin));
    Serial.print(":l:");
    Serial.println(getLDR(ldrPin));
  }
  else
  {
    //Serial.println("zzzzz");
    //sleepNow();
  }
}

float getTemp(int pin)
{
   //getting the voltage reading from the temperature sensor
 int reading = analogRead(pin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 
 // now return the temperature
 return (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
}

/**
 * Simple wrapper for getting light value
 */
float getLDR(int pin)
{
  return analogRead(pin);
}

void sleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     *  the power reduction management <avr/power.h>  is described in 
     *  http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
     */  
     
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 
  
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  
  sleep_mode();            // here the device is actually put to sleep!!
 
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep:
                            // disable sleep...

  power_all_enable();
   
}
