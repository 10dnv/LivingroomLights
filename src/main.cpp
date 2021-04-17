/* ############### INCLUDES ############### */
#include <functions.h>
#include <os.h>

/* ############### VARIABILE GLOBALE ############### */
uint32 timeMain;

/* Numele functiei: void setup()
** Descriere      : Functie specifica Arduino care se apealeaza o singura data la pornire
** Parametrii     : -
** Return         : -
*/
void setup() 
{
    OS_task_Init(); /* Apelez task-ul de init */
}

/* Numele functiei: void loop()
** Descriere      : Functie specifica Arduino care se apealeaza ciclic
** Parametrii     : -
** Return         : -
*/
void loop()
{
    OS_task_fast();      /* Apelez task-ul rapid    */

    timeMain = millis();     /* Actualizez timpul       */
    if (timeMain % 200 == 0)
    {
        OS_task_200ms(); /* Apelez task-ul la 200ms */
    }
    else if (timeMain % 100 == 0)
    {
        OS_task_100ms(); /* Apelez task-ul la 100ms */
    }
    else if (timeMain % 5 == 0)
    {
        OS_task_5ms();  /* Apelez task-ul la 5ms    */
    }
    else if (timeMain % 1 == 0)
    {
        OS_task_1ms();  /* Apelez task-ul la 1ms    */
    }
    else
    {
        /* Nimic */
    }
    delay(1);
}
