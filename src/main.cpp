#include <Arduino.h>
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key         = 0;
int adc_key_in      = 0;
#define btnRIGHT    0
#define btnUP       1
#define btnDOWN     2
#define btnLEFT     3
#define btnSELECT   4
#define btnNONE     5

// define some values used for the menu system
int rt_count        = 0;    // for testing
int up_count        = 0;    // for testing
int dn_count        = 0;    // for testing
int lf_count        = 0;    // for testing
int sl_count        = 0;    // for testing
int old_button      = 0;    // part of the debounce  
int temp_button     = 0;    // part of the debounce
int pressed_button  = 0;    // part of the debounce
int last_button     = 5;    // used to get event from a button press
int current_button  = 5;    // used to single button press



// Variables for GUI
      int currentMenuState    = 0;
      // 1234567890123456
      // 0)To Next Water        Default Menu
      //   HH:MM:SS
      // 1)Set Water Freq       Set Watering Frequency
      //  4 8 12 24 36 48
      // 2)Set Water Leng       Set Watering Length
      //  5 15 30 60 120
      // 3)Water Now            Water Now
      //  5 15 30 Hold
      // 4)Enabled? ON/OFF      Whether to disable/enable watering
      // ON    OFF

// Variables for watering system
unsigned long   waterFreq       = 28800000;    // 1000millisec*60sec*60min*8 = 8 hours
unsigned long   waterNextTime   = 28800000;    // the next time watering will occur compared to millis()

// read the buttons
int readButtonsLCD() {
    int i;

    adc_key_in = 0;
    for (i=0; i<4; i++)
    {
        adc_key_in += analogRead(0); // read the value from the sensor 4 times
    }
    adc_key_in = adc_key_in / 4;     // take the average of the sensor reading

    // my buttons when read are centered at these values: 30, 150, 360, 535, 760 (from BEST CHOICE ELECTRONICS)
    if (adc_key_in > 1000)  return btnNONE; // We make this the 1st option for speed reasons, since it's most common
    if (adc_key_in < 30)    return btnRIGHT;
    if (adc_key_in < 150)   return btnUP;
    if (adc_key_in < 360)   return btnDOWN;
    if (adc_key_in < 535)   return btnLEFT;
    if (adc_key_in < 760)   return btnSELECT;

    return btnNONE;   // when all others fail, return this
}

// provides a debounced output of buttons from LCD
int readButtonsLCD_DB() {
    
    lcd_key = readButtonsLCD();   // read the buttons

    if (lcd_key != old_button)
    {
        delay(50);
        temp_button = readButtonsLCD();
        if (lcd_key == temp_button)
        {
            old_button = lcd_key;
            switch (lcd_key)                // depending on which button was pushed, we perform an action
            {
                case btnRIGHT:
                {
                    return btnRIGHT;
                }

                case btnLEFT:
                {
                    return btnLEFT;
                }

                case btnUP:
                {
                    return btnUP;
                }

                case btnDOWN:
                {
                    return btnDOWN;
                }

                case btnSELECT:
                {
                    return btnSELECT;
                }

                case btnNONE:
                {
                    return btnNONE;
                }
            }
        
        }
        
    }

    

}

void menuDefault() {
    lcd.setCursor(0,0);
    lcd.print("0)To Next Water");
    
    unsigned long   tempTime;
    unsigned long   tempTimeHour;
    unsigned long   tempTimeMinute;
    unsigned long   tempTimeSecond;
    
    tempTime = ( waterNextTime - millis() ) / 1000;

    tempTimeSecond  = tempTime % 60;
    tempTime        = tempTime / 60;
    tempTimeMinute  = tempTime % 60;
    tempTime        = tempTime / 60;
    tempTimeHour    = tempTime;

    lcd.setCursor(0,1);
    
    if (tempTimeHour < 10) {
        lcd.print("0");
    } 
    lcd.print(tempTimeHour);
    lcd.print(":");

    if (tempTimeMinute < 10) {
        lcd.print("0");
    }
    lcd.print(tempTimeMinute);
    lcd.print(":");

    if (tempTimeSecond < 10) {
        lcd.print("0");
    }
    lcd.print(tempTimeSecond);


}

void menuSetWaterFreq() {
    lcd.setCursor(0,0);
    lcd.print("1)Set Water Freq");
    lcd.print("PLACEHOLDER");

}

void menuSetWaterLeng(){

}

void menuWaterNow() {

}

void menuEnable() {

}

// display the menu
void display_menu() {
    switch (currentMenuState)
    {
        case 0:
        {
            menuDefault();
            break;
        }

        case 1:
        {
            menuSetWaterFreq();
            break;
        }

        case 2:
        {
            menuSetWaterLeng();
            break;
        }

        case 3:
        {
            menuWaterNow();
            break;
        }

        case 4:
        {
            menuEnable();
            break;
        }

        default:
        {
            menuDefault();
            break;
        }
    }

}



void setup() {
    // put your setup code here, to run once:
    lcd.begin(16, 2);               // start the library
    lcd.setCursor(0,0);
//   lcd.print("RT LF UP DN SL ");  // print a simple message
}


void loop() {
    // put your main code here, to run repeatedly:
    
    /* DEBUGGING
    
    lcd.setCursor(0,0);             // DEBUGGING
    lcd.print(readButtonsLCD());      // DEBUGGING
    lcd.setCursor(3,0);
    lcd.print(readButtonsLCD_DB());
    lcd.setCursor(6,0);
    lcd.print(analogRead(0));

    */
   

    lcd.setCursor(15,1);            // move cursor to second line "1" and 9 spaces over
    lcd.print((millis()/1000)%10);  // display seconds since power up

    lcd.setCursor(0,1);             // move to the beginning of the second line

    display_menu();
    
    /*
    current_button = readButtonsLCD_DB();
    if (current_button != last_button)
    {
        last_button = current_button;
        
        switch (current_button)                // depending on which button was pushed, we perform an action
        {
            case btnRIGHT:
            {
                lcd.setCursor(0,1);
                rt_count++;
                lcd.print(rt_count);
                break;
            }

            case btnLEFT:
            {
                lcd.setCursor(3,1);
                lf_count++;
                lcd.print(lf_count);
                break;
            }

            case btnUP:
            {
                lcd.setCursor(6,1);
                up_count++;
                lcd.print(up_count);
                break;
            }

            case btnDOWN:
            {
                lcd.setCursor(9,1);
                dn_count++;
                lcd.print(dn_count);
                break;
            }

            case btnSELECT:
            {
                lcd.setCursor(12,1);
                sl_count++;
                lcd.print(sl_count);
                break;
            }

            case btnNONE:
            {
                
                break;
            }
        }
    }
    */



}