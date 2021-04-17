/* ############### INCLUDES ############### */
#include<os.h>


/* OS TASK INIT: Se executa o singura data, la alimentarea cu energie electrica */
void OS_task_Init()
{
  EEPROM.begin(512);
  Serial.begin(115200);
    Function_Init();
    // IRrecv irrecv(PIN_IR_RECEIVER);
    

   // irrecv.enableIRIn(); // Start the receiver
    /* Se declara directia piniilor de iesire */
    pinMode(PIN_RELAY1, OUTPUT);
    pinMode(PIN_RELAY2, OUTPUT);
    pinMode(PIN_RELAY3, OUTPUT);
    //pinMode(PIN_RELAY4, OUTPUT);


    /* Sting Releul implicit (functioneaza pe logica inversa) */
    digitalWrite(PIN_RELAY1, HIGH);
    digitalWrite(PIN_RELAY2, HIGH);
    digitalWrite(PIN_RELAY3, HIGH);
    //digitalWrite(PIN_RELAY4, HIGH);
    
    /* Se declara directia piniilor de intrare */
    pinMode(PIN_BUTTON1, INPUT_PULLUP);
    pinMode(PIN_BUTTON2, INPUT_PULLUP);
    pinMode(PIN_BUTTON3, INPUT_PULLUP);

   // pinMode(PIN_IR_RECEIVER, INPUT);
    // if (0 == MEMORIE_SCRISA) 
    // {
    //     /* Golesc memoria EEPROM */
    //       for (int i = 0 ; i < EEPROM.length() ; i++) 
    //       {
    //          EEPROM.write(i, 0);
    //       }
        
    //     /* Scriu in memoria EEPROM profilurile prestabilite */
    //     EEPROM.put(ADRESA_STRUCTURA_CODURI, btnCodesIR);
    // }

    /* Citesc din memoria EEPROM profilurile stocate */
    EEPROM.get(ADRESA_STRUCTURA_CODURI, btnCodesIR_buffer);
    

}
/* OS TASK fast: Se executa ciclic la fiecare 0.25 microsecunde */
void OS_task_fast()
{
    
}

/* OS TASK 1ms: Se executa ciclic la fiecare 1 milisecunda */
void OS_task_1ms()
{
     Btn_Cyclic();
     Relay_Cyclic();
     MainAppStateMachine();
}

/* OS TASK 5ms: Se executa ciclic la fiecare 5 milisecunde */
void OS_task_5ms()
{
  // schimbare_stare();
}

/* OS TASK 100ms: Se executa ciclic la fiecare 100 milisecunde */
void OS_task_100ms()
{
    
}

/* OS TASK 200ms: Se executa ciclic la fiecare 200 milisecunde */
void OS_task_200ms()
{
    
}