#ifndef _PLATFORM_TYPES_
#define _PLATFORM_TYPES_

/* ############### INCLUDES ############### */
#include <Arduino.h>

/* ############### DEFINES ############### */
#define MEMORIE_SCRISA      1 /* 0 daca vreau sa rescrie profilulurile initiale, 1 altfel */

/* Definitii conexiunea componentelor la pinii microcontrollerului      */

/* IESIRI */
#define PIN_RELAY1           D2  /* Releu                                 */
#define PIN_RELAY2           D3  /* Releu                                 */
#define PIN_RELAY3           D4  /* Releu                                 */
//#define PIN_RELAY4             /* Releu                                 */

// #define PIN_LED_RED               D8
// #define PIN_LED_GREEN             D0


/* INTRARI */
#define PIN_BUTTON1          D5
#define PIN_BUTTON2          D6
#define PIN_BUTTON3          D7
   
//#define PIN_PHOTORESISTOR   A0 /* Senzor luminozitate                   */
#define PIN_IR_RECEIVER     D1       

/* Adrese de memorie EEPROM */
#define ADRESA_STRUCTURA_CODURI     0

/* Valori debounce pentru encoder si buton */
#define BUTTON_DEBOUNCE_COUNTER_PRESS                   10
#define BUTTON_DEBOUNCE_COUNTER_LONG_PRESS             2500
#define BUTTON_DEBOUNCE_COUNTER_NOT_PRESSED              10

#define NUMBER_OF_BUTTONS  3
#define NUMBER_OF_RELAYS   3


/* ############### ENUMS ############### */
enum type_StateMachineFunctions
{
    SM_Function_Idle,
    SM_Function_Learn
};

enum type_state_Relay
{
    STD_RELAY_ON,
    STD_RELAY_OFF
};

enum type_BtnState
{
    Button_Pressed,
    Button_LongPressed,
    Button_Released,
    Button_NotPressed
};
/* ############### TIPURI DE DATE ############### */
typedef unsigned char  uint8;   /* Interval [0 - 255]        */
typedef unsigned short uint16;  /* Interval [0 - 65,535]     */
//typedef unsigned long int   uint32;  /* Interval [0 - 4294967295] */

/* ############### STRUTURI ############### */


/* Structura care defineste un buton */
typedef struct 
{
    type_BtnState button_state;     /* The state of the button                     */
    uint8         button_pin;       /* The physical pin the button is connected to */
    int           button_CountUp;   /* Debounce Counter for Pressed                */
    int           button_CountDown; /* Debounce Counter for NotPressed             */
}struct_button;

/* Structura care tine minte codurile de la telecomanda */
typedef struct 
{
    uint32 code_button1;
    uint32 code_button2;
    uint32 code_button3;
    uint32 code_button4;
}struct_btnCodes;

/* Structura care defineste un releu */
typedef struct 
{
    uint8      relay_pin;
    type_state_Relay relay_state;
}struct_relay;


/* ############### CONSTANTE ############### */

/* ############### PROTOTIPURI ############### */


#endif /* _PLATFORM_TYPES_ */