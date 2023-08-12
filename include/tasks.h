#ifndef __TASKS_H
#define __TASKS_H

#include "cmsis_os.h"

// -- Definitions of tasks -- //

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ButtonTaskHandle;
const osThreadAttr_t ButtonTask_attributes = {
  .name = "BUTTON",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

osThreadId_t VCOMTaskHandle;
const osThreadAttr_t VCOMTask_attributes = {
  .name = "VCOM",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// -- Thread functions -- //

void StartDefaultTask(void *argument);
void HandleButtons(void *argument);
void SendVCOM(void *argument);

#endif