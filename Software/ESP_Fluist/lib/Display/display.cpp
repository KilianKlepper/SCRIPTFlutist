// display.cpp
#include <Arduino.h>
#include "display.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay() {
    Serial.print("Initializing Display..... ");
      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println("FAILED");
      for(;;); // Don't proceed, loop forever
    }
    // Clear the buffer
    display.setRotation(1);
    display.clearDisplay();

    // Draw a single pixel in white
    display.drawBitmap(1,10,ble_nc,24,24,1);
    Serial.println("OK");

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
}

void displayBLE_Status(int ble_state) {
    
    int x_pos_icon = 4;
    int y_pos_icon = 9;
    display.fillRect(x_pos_icon, y_pos_icon, x_pos_icon+24, y_pos_icon+24, BLACK);
    switch (ble_state)
    {
    case 1: // Disconnected
        display.drawBitmap(x_pos_icon,y_pos_icon,ble_nc,24,24,1);
        break;
    case 2: // Searching
        display.drawBitmap(x_pos_icon,y_pos_icon,ble_search,24,24,1);
        break;
    case 3: // Connected
        display.drawBitmap(x_pos_icon,y_pos_icon,ble_con,24,24,1);
        break;
    
    default:
        display.fillRect(x_pos_icon, y_pos_icon, x_pos_icon+24, y_pos_icon+24, BLACK);
        display.display();
        break;
    }
}

void displayOnOff_Status(bool OnOff_State) {
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setCursor(2,0);             // Start at top-left corner
    if(OnOff_State) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(F("ON "));
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        display.println(F("OFF "));
    }
    display.drawLine(0, 8, 31, 8, WHITE);
    display.display();
}

void displayCapSense_Status(bool CapSense_Up, bool CapSense_Down, bool CapSense_Effect) {
    int x_pos = 1;
    int y_pos = 84;
    int y_offset = 9;
    display.fillRect(0, 84, 32, 27, BLACK);
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setCursor(x_pos,y_pos);  
    if(CapSense_Up) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
    display.print("COLR");

    display.setCursor(x_pos,y_pos+y_offset);           
    if(CapSense_Down) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
    display.print("BRIG");

    display.setCursor(x_pos,y_pos+(2*y_offset));         
    if(CapSense_Effect) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    } 
    display.print("EFCT");
    display.drawLine(0, 111, 31, 111, WHITE);  
    display.display();
}

void displayeARGBValue(int effect, int dec_alpha, int dec_red, int dec_green, int dec_blue) {
    int x_pos = 1;
    int y_pos = 36;
    int y_offset = 9;
    display.drawLine(0, 33, 31, 33, WHITE);   
    display.fillRect(0, 34, 31, 45, BLACK);
    display.setTextSize(1);
    display.setCursor(x_pos,y_pos);   
    display.setTextColor(SSD1306_WHITE);
    switch (effect)
    {
    case 0: display.print("OFF"); break;
    case 1: display.print("SOLID"); break;
    case 2: display.print("FADE"); break;
    case 3: display.print("RNBW"); break;
    case 4: display.print("RBGL"); break;
    case 5: display.print("CNFT"); break;
    case 6: display.print("RUNL"); break;
    case 7: display.print("JGL"); break;
    case 8: display.print("SNLN"); break;
    case 9: display.print("STAT"); break;
    default: break;    
    }

    display.setCursor(x_pos,y_pos+y_offset);  
    display.print("A:"); display.print(dec_alpha); 
    display.setCursor(x_pos,y_pos+(y_offset*2));  
    display.print("R:"); display.print(dec_red); 
    display.setCursor(x_pos,y_pos+(y_offset*3));  
    display.print("G:"); display.print(dec_green); 
    display.setCursor(x_pos,y_pos+(y_offset*4));  
    display.print("B:"); display.print(dec_blue); 
    display.drawLine(0, 82, 31, 82, WHITE); 
    display.display();
}

void displayClear(){
    display.clearDisplay();
}