#include <Arduino.h>
#include "management.h"

void setup() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
  QueueHandle_t xKeypadQueue = xQueueCreate(5,sizeof(String));
  QueueHandle_t xLCDQueue = xQueueCreate(5,sizeof(String));
  
  Motor1_Q = xQueueCreate(5,sizeof(MH_t));
  Motor2_Q = xQueueCreate(5,sizeof(MH_t));
  Motor3_Q = xQueueCreate(5,sizeof(MH_t));
  Motor4_Q = xQueueCreate(5,sizeof(MH_t));
  Motor5_Q = xQueueCreate(5,sizeof(MH_t));

  MotorConfig Motor1 = {.id = 1,.dirPin= 13,.stepPin= 12,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = false, .queue = &Motor1_Q
                       };
  MotorConfig Motor2 = {.id = 2,.dirPin= 14,.stepPin= 27,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = false, .queue = &Motor2_Q
                       };
  MotorConfig Motor3 = {.id = 3,.dirPin= 26,.stepPin= 25,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = false, .queue = &Motor3_Q
                       };
  MotorConfig Motor4 = {.id = 4,.dirPin= 33,.stepPin= 32,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = false, .queue = &Motor4_Q
                       };
  MotorConfig Motor5 = {.id = 5,.dirPin= 35,.stepPin= 34,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = true, .queue = &Motor5_Q
                       };

  Stepper Motor01(Motor1);
  Stepper Motor02(Motor2);
  Stepper Motor03(Motor3);
  Stepper Motor04(Motor4);
  Stepper Motor05(Motor5);
  

  xTaskCreatePinnedToCore(vTaskMotor,"Motor1",10000,&Motor01,1,&Motor1_t,1);
  xTaskCreatePinnedToCore(vTaskMotor,"Motor2",10000,&Motor02,1,&Motor2_t,1);
  xTaskCreatePinnedToCore(vTaskMotor,"Motor3",10000,&Motor03,1,&Motor3_t,1);
  xTaskCreatePinnedToCore(vTaskMotor,"Motor4",10000,&Motor04,1,&Motor4_t,1);
  xTaskCreatePinnedToCore(vTaskMotor,"Motor5",10000,&Motor05,1,&Motor5_t,1);
  xTaskCreatePinnedToCore(vTaskKeypad,"Keypad",10000,(void *) 1,1,&xKeypadHandle,0);
  xTaskCreatePinnedToCore(vTaskLCD,"LCD",10000,(void *) 1,2,&xLCDHandle,0);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(portMAX_DELAY);
}


