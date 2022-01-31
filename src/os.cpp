/* ############### INCLUDES ############### */
#include<os.h>

/* OS TASK INIT: This task will only be executed once on startup */
void OS_task_Init()
{
    EEPROM.begin(512);
    Serial.begin(115200);
    Function_Init();

    /* Set Output pins */
    pinMode(PIN_RELAY1, OUTPUT);
    pinMode(PIN_RELAY2, OUTPUT);
    pinMode(PIN_RELAY3, OUTPUT);
#ifdef USE_4TH_RELAY
    pinMode(PIN_RELAY4, OUTPUT);
#endif /* #ifdef USE_4TH_RELAY */

    /* Setting all relays to OFF (since the work on inverted logic) */
    digitalWrite(PIN_RELAY1, HIGH);
    digitalWrite(PIN_RELAY2, HIGH);
    digitalWrite(PIN_RELAY3, HIGH);
#ifdef USE_4TH_RELAY
    digitalWrite(PIN_RELAY4, HIGH);
#endif /* #ifdef USE_4TH_RELAY */

    /* Set input pins */
    pinMode(PIN_BUTTON1, INPUT_PULLUP);
    pinMode(PIN_BUTTON2, INPUT_PULLUP);
    pinMode(PIN_BUTTON3, INPUT_PULLUP);

#ifdef WRITTEN_MEM

    /* Empty EEPROM  */
      for (int i = 0 ; i < EEPROM.length() ; i++) 
      {
          EEPROM.write(i, 0);
      }
    
    /* Write predefined profiles into memory */
    EEPROM.put(CODES_STRUCT_ADDR, btnCodesIR);
#endif /* #ifdef WRITTEN_MEM */

    /* Read stored profiles from EEPROM */
    EEPROM.get(CODES_STRUCT_ADDR, btnCodesIR_buffer);
}
/* OS TASK fast: 0.25 uS period */
void OS_task_fast()
{
    ArduinoOTA.handle();
}

/* OS TASK 1ms */
void OS_task_1ms()
{
     Btn_Cyclic();
     Relay_Cyclic();
     MainAppStateMachine();
}

/* OS TASK 5ms */
void OS_task_5ms()
{
}

/* OS TASK 100ms */
void OS_task_100ms()
{
}

/* OS TASK 200ms */
void OS_task_200ms()
{
}