#ifndef _OS_
#define _OS_

/* ############### INCLUDES ############### */
#include <functions.h>

/* ############### EXTERNALS ############### */

/* OS Interface functions */
extern void OS_task_Init();
extern void OS_task_fast();
extern void OS_task_1ms();
extern void OS_task_5ms();
extern void OS_task_100ms();
extern void OS_task_200ms();

#endif /* _OS_ */