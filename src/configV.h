#ifndef ConfigV_H
#define ConfigV_H
#include <Arduino.h>
#include <Keypad.h>
#include "Stepper.h"
#include <LiquidCrystal_I2C.h>

void SplitString(String *data, String *token, String separator);
// Start  :: xTask Config and Variables

TaskHandle_t xMotorHandle1_t = NULL;
TaskHandle_t xMotorHandle2_t = NULL;
TaskHandle_t xMotorHandle3_t = NULL;
TaskHandle_t xMotorHandle4_t = NULL;
TaskHandle_t xMotorHandle5_t = NULL;
TaskHandle_t xKeypadHandle = NULL;
TaskHandle_t xLCDHandle = NULL;

QueueHandle_t xKeypadQueue = NULL;
QueueHandle_t xLCDQueue = NULL;
//End :: xTask Config and Variables


//Start :: Motor  Config


typedef struct {
  bool status = false;
  int velocity = 0;
}MH_t;

int valueT = 0;

QueueHandle_t Motor1_Q = NULL;
QueueHandle_t Motor2_Q = NULL;
QueueHandle_t Motor3_Q = NULL;
QueueHandle_t Motor4_Q = NULL;
QueueHandle_t Motor5_Q = NULL;
//End :: Motor  Config

//Start :: Keypad Config

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9', 'C'},
{'*','0','#', '.'}
};
byte rowPins[ROWS] = {15, 2, 4, 5}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {18, 23, 3, 1}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;

//End :: Keypad Config


QueueHandle_t Motor1_t = xQueueCreate(5,sizeof(MH_t));
QueueHandle_t Motor2_t = xQueueCreate(5,sizeof(MH_t));
QueueHandle_t Motor3_t = xQueueCreate(5,sizeof(MH_t));
QueueHandle_t Motor4_t = xQueueCreate(5,sizeof(MH_t));
QueueHandle_t Motor5_t = xQueueCreate(5,sizeof(MH_t));




// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
#endif