# SR-Clock
Under Havy dev ^_^
## PinOut Map:
    LCD
        22, 21, 18
    Keypad 
        Rows 15, 2, 4, 5
        Cols 18, 23, 3:RX0 ,1:TX0 
    Motors {Direction, Step}
        {13, 12}, {14,27}, {26,25}, {33,32}, {35,34}


## Code for check LCD I2C address 
    /*********
      Rui Santos
      Complete project details at https://randomnerdtutorials.com  
    *********/

    #include <Wire.h>
     
    void setup() {
      Wire.begin();
      Serial.begin(115200);
      Serial.println("\nI2C Scanner");
    }
     
    void loop() {
      byte error, address;
      int nDevices;
      Serial.println("Scanning...");
      nDevices = 0;
      for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
          Serial.print("I2C device found at address 0x");
          if (address<16) {
            Serial.print("0");
          }
          Serial.println(address,HEX);
          nDevices++;
        }
        else if (error==4) {
          Serial.print("Unknow error at address 0x");
          if (address<16) {
            Serial.print("0");
          }
          Serial.println(address,HEX);
        }    
      }
      if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
      }
      else {
        Serial.println("done\n");
      }
      delay(5000);          
    }


## Massage 
    /*********
      Rui Santos
      Complete project details at https://randomnerdtutorials.com  
    *********/

    #include <LiquidCrystal_I2C.h>

    // set the LCD number of columns and rows
    int lcdColumns = 16;
    int lcdRows = 2;

    // set LCD address, number of columns and rows
    // if you don't know your display address, run an I2C scanner sketch
    LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

    String messageStatic = "Static message";
    String messageToScroll = "This is a scrolling message with more than 16 characters";

    // Function to scroll text
    // The function acepts the following arguments:
    // row: row number where the text will be displayed
    // message: message to scroll
    // delayTime: delay between each character shifting
    // lcdColumns: number of columns of your LCD
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

    void setup(){
      // initialize LCD
      lcd.init();
      // turn on LCD backlight                      
      lcd.backlight();
    }

    void loop(){
      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      // print static message
      lcd.print(messageStatic);
      // print scrolling message
      scrollText(1, messageToScroll, 250, lcdColumns);
    }

