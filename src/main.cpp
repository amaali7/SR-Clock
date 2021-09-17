#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Keypad.h>
#include "Stepper.h"

// Start  :: xTask Config and Variables
const String passcode = "1234";

TaskHandle_t xMotorHandle1_t = NULL;
TaskHandle_t xMotorHandle2_t = NULL;
TaskHandle_t xMotorHandle3_t = NULL;
TaskHandle_t xMotorHandle4_t = NULL;
TaskHandle_t xMotorHandle5_t = NULL;
TaskHandle_t xKeypadHandle = NULL;
TaskHandle_t xLCDHandle = NULL;

QueueHandle_t xKeypadQueue = NULL;
QueueHandle_t xLCDQueue = NULL;

void SplitString(String *data, String *token, String separator);
void vTaskManager(void *par);
void vTaskMotor(void *par);
void vTaskKeypad(void *par);
void vTaskLCD(void *par);

//End :: xTask Config and Variables

//Start :: Motor  Config
// Stepper(int idTag, int dir_pin, int step_pin, int steps_per_revolution, int delaymic, bool resatablety ){


typedef struct {
  bool status = false;
  int velocity;
}MH_t;

const float c11 = 299792.458000000;
int valueT = 0;

QueueHandle_t Motor1_Q = NULL;
QueueHandle_t Motor2_Q = NULL;
QueueHandle_t Motor3_Q = NULL;
QueueHandle_t Motor4_Q = NULL;
QueueHandle_t Motor5_Q = NULL;
//End :: Motor  Config

//Start :: Keypad Config
typedef struct{
  int motor;
  String msg;
}KeypadOject;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9', 'C'},
{'*','0','#', '.'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {8, 7, 6, 19}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;

//End :: Keypad Config

double ReCalc(int velocity, int IdTag){
  
  double RPM = 0.00000000000000;
  switch (IdTag)
  {
  case 1:
    RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))))*12.86956521;
    break;
  case 2:
    RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))/60))*12.17391304;
    break;
  case 3:
    RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))/720))*12.15217391;
    break;
  case 4:
    RPM = 40;
    break;
  case 5:
    RPM = 50*(sqrt(1-(sq(velocity)/sq(c11))))/3.81;
    break;
  default:
    break;
  }
  return RPM;
}

void setup() {
  // put your setup code here, to run once:
  
  // QueueHandle_t xKeypadQueue = xQueueCreate(5,sizeof(String));
  // QueueHandle_t xLCDQueue = xQueueCreate(5,sizeof(String));
  Motor1_Q = xQueueCreate(5,sizeof(MH_t));
  Motor2_Q = xQueueCreate(5,sizeof(MH_t));
  Motor3_Q = xQueueCreate(5,sizeof(MH_t));
  Motor4_Q = xQueueCreate(5,sizeof(MH_t));
  Motor5_Q = xQueueCreate(5,sizeof(MH_t));

  MotorConfig Motor1 = {.id = 1,.dirPin= 2,.stepPin= 3,
                          .steps_per_revolution = 200, .delaymic = 2000, .resatablety = false
                       };

  Stepper Motor01(Motor1);
  // Keypad keypad;
  
  BaseType_t xReturnedMotor1;
  BaseType_t xReturnedMotor2;
  BaseType_t xReturnedMotor3;
  BaseType_t xReturnedMotor4;
  BaseType_t xReturnedMotor5;
  BaseType_t xReturnedKeypad;
  BaseType_t xReturnedLCD;

  QueueHandle_t Motor1_t = xQueueCreate(5,sizeof(KeypadOject));
  QueueHandle_t Motor2_t = xQueueCreate(5,sizeof(KeypadOject));
  QueueHandle_t Motor3_t = xQueueCreate(5,sizeof(KeypadOject));
  QueueHandle_t Motor4_t = xQueueCreate(5,sizeof(KeypadOject));
  QueueHandle_t Motor5_t = xQueueCreate(5,sizeof(KeypadOject));

  xReturnedMotor1 = xTaskCreatePinnedToCore(vTaskMotor,"Motor1",10000,&Motor01,1,&xMotorHandle1_t,1);
  xReturnedKeypad = xTaskCreatePinnedToCore(vTaskKeypad,"Keypad",10000,(void *) 1,1,&xKeypadHandle,0);
  xReturnedLCD = xTaskCreatePinnedToCore(vTaskLCD,"LCD",10000,(void *) 1,2,&xLCDHandle,0);
  
  if (xReturnedMotor1 == pdPASS)
  {
    vTaskDelete(xMotorHandle1_t);
  }  
  if (xReturnedKeypad == pdPASS)
  {
    vTaskDelete(xKeypadHandle);
  }
  if (xReturnedLCD == pdPASS)
  {
    vTaskDelete(xLCDHandle);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(portMAX_DELAY);
}

void SplitString(String *data, String *token, String separator){
  *token = data->substring(0,data->indexOf(separator));
  data->remove(0,data->indexOf(separator)+separator.length());
}

void vTaskManager(void *par){
  String _data, key, value, temp;
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
          
        }
        else if (key == "B")
        {
          // Stop System
        }
        else if (key == "C")
        {
          // Reset System
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
        motor->delayMic = ReCalc(state.velocity,motor->IdTag);
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

}