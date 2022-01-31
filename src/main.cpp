/* ############### INCLUDES ############### */
#include <functions.h>
#include <os.h>

/* ############### GLOBAL VARIABLES ############### */
uint32 timeMain;

/* Function name: void setup()
** Description  : Arduino specific funtion; Called once on startup
** Params       : -
** Return       : -
*/
void setup() 
{
    OS_task_Init();
}

/* Function name: void loop()
** Description  : Arduino specific funtion; Called cyclically
** Params       : -
** Return       : -
*/
void loop()
{
    /* Call different tasks based on their period */
    OS_task_fast();

    timeMain = millis();
    if (timeMain % 200 == 0)
    {
        OS_task_200ms();
    }
    else if (timeMain % 100 == 0)
    {
        OS_task_100ms();
    }
    else if (timeMain % 5 == 0)
    {
        OS_task_5ms();
    }
    else if (timeMain % 1 == 0)
    {
        OS_task_1ms();
    }
    else
    {
        /* Nothing */
    }
    delay(1);
}