#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



//
// OLED DISPLAY
//
#define OLED_SCREEN_WIDTH      128 // OLED display width, in pixels
#define OLED_SCREEN_HEIGHT      64 // OLED display height, in pixels
#define OLED_RESET              -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SCREEN_ADDRESS   0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display( OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_RESET );

void SetupOLED();
void DisplayBlack();
void DisplayString( const char * const pszString );


void SetupOLED()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if( !display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_ADDRESS))  
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }  

 // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();// Clear the buffer
}


void DisplayBlack()
{
    display.clearDisplay();
    display.display();
}


void DisplayString( const char * const pszString )
{
    display.clearDisplay();
    display.setTextSize(3); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println( pszString );
    display.display();
}
