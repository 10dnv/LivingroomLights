/* ############### INCLUDES ############### */
#include <platform_types.h>
#include <functions.h>

/* Array of buttons */
struct_button BtnArray[] = 
{
    {
        .button_state     = Button_NotPressed,
        .button_pin       = PIN_BUTTON1,
        .button_CountUp   = 0,
        .button_CountDown = 0
    },
    {
        .button_state     = Button_NotPressed,
        .button_pin       = PIN_BUTTON2,
        .button_CountUp   = 0,
        .button_CountDown = 0
    },
    {
        .button_state     = Button_NotPressed,
        .button_pin       = PIN_BUTTON3,
        .button_CountUp   = 0,
        .button_CountDown = 0
    }
};

struct_relay RelayArray[] = 
{
    {
        PIN_RELAY1,
        STD_RELAY_OFF
    },
    {
        PIN_RELAY2,
        STD_RELAY_OFF
    },
    {
        PIN_RELAY3,
        STD_RELAY_OFF
     }
#ifdef USE_4TH_RELAY
    ,{
        PIN_RELAY4,
        STD_RELAY_OFF
    }
#endif /* #ifdef USE_4TH_RELAY */
};

struct_btnCodes btnCodesIR =
{
    0xE0E036C9,
    0xE0E028D7,
    0xE0E0A857,
    0xE0E06897
};

struct_btnCodes btnCodesIR_buffer =
{
    0,
    0,
    0,
    0
};