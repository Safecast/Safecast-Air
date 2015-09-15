#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUF_SIZE    50
#define OLED_DC     5
#define OLED_CS     3
#define OLED_RESET  4

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


void setup()   {                
    Serial.begin(9600);

    display.begin(SSD1306_SWITCHCAPVCC);

    display.clearDisplay();   
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
}

void loop() 
{
    static int cnt = 0;
    char buf[BUF_SIZE];

    display.clearDisplay();
    display.setCursor(0,0);

    display.println("LCD Test");
    display.println();
    
    for (int i=0; i<6; i++)
    {
        snprintf(buf,BUF_SIZE,"SENS %d:  %d", i, (i+1)*cnt);
        display.println(buf);
    }

    display.display();
    cnt++;
    delay(50);
}

