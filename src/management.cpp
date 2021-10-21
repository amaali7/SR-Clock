#include "management.h"

void vTaskManager(void *par){
  String _data, key, value, temp;
  MH_t state;
  for (;;)
  {
    
    if (uxQueueMessagesWaiting(xKeypadQueue))
    {
      xQueueReceive(xKeypadQueue,&_data,(TickType_t) 5);
      if (!_data.isEmpty())
      {
        SplitString(&_data,&temp,"*");
        SplitString(&_data,&key,"*");
        SplitString(&_data,&value,"#");
        if (key == "A")
        {
          // Initalize system
          state.status = true;
          state.velocity = value.toInt();
          xQueueSend(Motor1_Q,&state,(TickType_t) 0);
          xQueueSend(Motor2_Q,&state,(TickType_t) 0);
          xQueueSend(Motor3_Q,&state,(TickType_t) 0);
          xQueueSend(Motor4_Q,&state,(TickType_t) 0);
          xQueueSend(Motor5_Q,&state,(TickType_t) 0);
        }
        else if (key == "B")
        {
          // Stop System
          state.status = false;
          state.velocity = 0;
          xQueueSend(Motor1_Q,&state,(TickType_t) 0);
          xQueueSend(Motor2_Q,&state,(TickType_t) 0);
          xQueueSend(Motor3_Q,&state,(TickType_t) 0);
          xQueueSend(Motor4_Q,&state,(TickType_t) 0);
          xQueueSend(Motor5_Q,&state,(TickType_t) 0);
        }
        else if (key == "C")
        {
          // Reset System
          state.status = false;
          state.velocity = 0;
          xQueueSend(Motor1_Q,&state,(TickType_t) 0);
          xQueueSend(Motor2_Q,&state,(TickType_t) 0);
          xQueueSend(Motor3_Q,&state,(TickType_t) 0);
          xQueueSend(Motor4_Q,&state,(TickType_t) 0);
          xQueueSend(Motor5_Q,&state,(TickType_t) 0);
          
        }
        else
        {
          
        }
      }
    }
    vTaskDelay(20);
  }
  vTaskDelete(NULL);
}

void vTaskMotor(void *par){

  // Start :: Motor Config and Variables
  Stepper *motor = (Stepper *) par;
  MH_t state;
  // End :: Motor Config and Variables
  int lastValue = 1;
  for (;;)
  {
    xQueueReceive(motor->queue,&state,(TickType_t) 5);
    if (state.status)
    {
      if (motor->resatable)
      {
        motor->Init(4000,1);
        motor->GoTarget();
      }
      else
      {
        if (state.velocity != lastValue)
        {
          motor->SetDelay(state.velocity);  
          lastValue  = state.velocity;
        }
        else if (state.velocity == 0 && motor->resatable)
        {
          motor->RestMotor(0);
        }
        
        motor->GoStep();
      }
    }
    vTaskDelay(20);
  }
  vTaskDelete(NULL);
}
void vTaskKeypad(void *par){
    
    String keyStor;
    for (;;)
    {
        
        char key = kpd.getKey();
        if (key != '#')
        {
            keyStor+=key;
        }
        else
        {
            keyStor+=key;
            // Queue part
            xQueueSend(xKeypadQueue,&keyStor,(TickType_t) 0 );
            keyStor = "";
        }
        
        
    }
    vTaskDelete(NULL);
}
void vTaskLCD(void *par){
  LiquidCrystal_I2C *lcd16 = (LiquidCrystal_I2C *) par;
  String msg;
  for (;;)
  {
    if (uxQueueMessagesWaiting(xKeypadQueue))
    {
      xQueueReceive(xLCDQueue,&msg,(TickType_t) 5);
      if (!msg.isEmpty())
      {

      }
    }
    vTaskDelay(20);
  }
  vTaskDelete(NULL);
}