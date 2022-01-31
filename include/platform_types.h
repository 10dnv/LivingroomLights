#ifndef _PLATFORM_TYPES_
#define _PLATFORM_TYPES_

/* ############### INCLUDES ############### */
#include <Arduino.h>

/* ############### DEFINES ################ */
//#define WRITTEN_MEM      /* Uncomment to write initial profiles       */
//#define USE_4TH_RELAY    /* Uncomment to use 4th relay from the board */

/* HW Configuration*/

/* OUTPUTS */
#define PIN_RELAY1           D2
#define PIN_RELAY2           D3
#define PIN_RELAY3           D4

#ifdef USE_4TH_RELAY
#define PIN_RELAY4           D8
#endif /* #ifdef USE_4TH_RELAY */


/* INPUTS */
#define PIN_BUTTON1          D5
#define PIN_BUTTON2          D6
#define PIN_BUTTON3          D7

#define PIN_IR_RECEIVER      D1

/* EEPROM  ADDRESSES */
#define CODES_STRUCT_ADDR     0

/* Button debounce values */
#define BUTTON_DEBOUNCE_COUNTER_PRESS             10
#define BUTTON_DEBOUNCE_COUNTER_LONG_PRESS      2500
#define BUTTON_DEBOUNCE_COUNTER_NOT_PRESSED       10

#define NUMBER_OF_BUTTONS  3

#ifdef USE_4TH_RELAY
#define NUMBER_OF_RELAYS   4
#else
#define NUMBER_OF_RELAYS   3
#endif /* #ifndef USE_4TH_RELAY */



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
/* ############### DATA_TYPES ############### */
typedef unsigned char  uint8;   /* Interval [0 - 255]        */
typedef unsigned short uint16;  /* Interval [0 - 65,535]     */

/* ############### STRUCTURES ############### */

/* Button Structure */
typedef struct 
{
    type_BtnState button_state;     /* The state of the button                     */
    uint8         button_pin;       /* The physical pin the button is connected to */
    int           button_CountUp;   /* Debounce Counter for Pressed                */
    int           button_CountDown; /* Debounce Counter for NotPressed             */
}struct_button;

/* Infrared codes Structure */
typedef struct 
{
    uint32 code_button1;
    uint32 code_button2;
    uint32 code_button3;
    uint32 code_button4;
}struct_btnCodes;

/* Relay Structure */
typedef struct 
{
    uint8            relay_pin;
    type_state_Relay relay_state;
}struct_relay;


/* ############### CONSTANTS ################ */

/* ############### PROTOTYPES ############### */

#endif /* _PLATFORM_TYPES_ */