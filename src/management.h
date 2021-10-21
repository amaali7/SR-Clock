#ifndef ManageMent_H
#define ManageMent_H
#include "configV.h"
#include "Stepper.h"

void vTaskManager(void *par);
void vTaskMotor(void *par);
void vTaskKeypad(void *par);
void vTaskLCD(void *par);


#endif