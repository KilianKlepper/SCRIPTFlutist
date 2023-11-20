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
    display.clearDisplay();

    // Draw a single pixel in white
    display.drawBitmap(1,1,ble_nc,24,24,1);
    Serial.println("OK");

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
}

void displayBLE_Status(int ble_state) {
    display.clearDisplay();
    int x_pos_icon = 1;
    int y_pos_icon = 1;
    switch (ble_state)
    {
    case 1: // Disconnected
        display.fillRect(x_pos_icon, y_pos_icon, x_pos_icon+24, y_pos_icon+24, BLACK);
        display.drawBitmap(x_pos_icon,y_pos_icon,ble_nc,24,24,1);
        break;
    case 2: // Searching
        display.fillRect(x_pos_icon, y_pos_icon, x_pos_icon+24, y_pos_icon+24, BLACK);
        display.drawBitmap(x_pos_icon,y_pos_icon,ble_search,24,24,1);
        break;
    case 3: // Connected
        display.fillRect(x_pos_icon, y_pos_icon, x_pos_icon+24, y_pos_icon+24, BLACK);
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
    display.setCursor(1,26);             // Start at top-left corner
    if(!OnOff_State) {
        display.setTextColor(SSD1306_WHITE); // Draw white textdisplay.setTextColor(SSD1306_WHITE); // Draw white text
        display.println(F(":)"));
    } else {
        display.setTextColor(SSD1306_BLACK); // Draw 'inverse' text
        display.println(F(":)"));
    }
    display.display();
}

void displayCapSense_Status(bool CapSense_Up, bool CapSense_Down, bool CapSense_Effect) {
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setCursor(90,1);  
    if(CapSense_Up) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
    display.print(" UP ");

    display.setCursor(90,12);           
    if(CapSense_Down) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
    display.print("DOWN");

    display.setCursor(90,24);         
    if(CapSense_Effect) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    } 
    display.print("EFCT");
    
    display.display();
}

void displayRGBValue(int dec_red, int dec_green, int dec_blue) {
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setCursor(40,1);   
    display.setTextColor(SSD1306_WHITE);
    display.print("R:"); display.print(dec_red); 
    display.setCursor(40,12);  
    display.print("G:"); display.print(dec_green); 
    display.setCursor(40,24);  
    display.print("B:"); display.print(dec_blue); 
    display.display();
}
