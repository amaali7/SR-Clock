#include "configV.h"

void SplitString(String *data, String *token, String separator){
  *token = data->substring(0,data->indexOf(separator));
  data->remove(0,data->indexOf(separator)+separator.length());
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
