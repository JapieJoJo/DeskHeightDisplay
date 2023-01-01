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
void DisplayClear();
void DisplayDraw();
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

void DisplayClear()
{
    display.clearDisplay();
}

void DisplayDraw()
{
    display.display();
}

void DisplayString( const char * const pszString )
{
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 0);
    display.println( pszString );
    //DisplayDraw();
}



#define N_TRIANGLES 5

//
// Direction +1 = UP, -1 = DOWN
// 
void ScrollTriangles( int8_t iDirection )
{
  static int8_t iPrevDirection = 0;
  static int8_t nTriHeight     =  12;
  static int8_t nTriWidth      =  16;
  static int8_t nTriOffset     =   4;
  static int8_t nDeltaY        =  -1;
  
  static int8_t posCxL  =  nTriWidth/2;
  static int8_t posCxR  =  display.width() - 1 - nTriWidth/2;
  
  static int8_t posTop[N_TRIANGLES] = {0};
         int8_t posBot[N_TRIANGLES] = {0};

  if( 0 == iPrevDirection || iPrevDirection != iDirection )
  {
    for( uint8_t iTriangle = 0; iTriangle < N_TRIANGLES; ++iTriangle )
    {
      if( iDirection > 0 )
        posTop[iTriangle] = display.height() - 1 - iTriangle * iDirection * (nTriHeight + nTriOffset);// up - align triangle top with bottom display
      else
        posTop[iTriangle] = -1 * iTriangle * iDirection * (nTriHeight + nTriOffset);// down - align inverted triangle top with top display
    }
  }

  iPrevDirection = iDirection;

  for( uint8_t iTriangle = 0; iTriangle < N_TRIANGLES; ++iTriangle )
  {
    posBot[iTriangle] = posTop[iTriangle] + (iDirection * nTriHeight);
    
    //left side
    display.fillTriangle( posCxL,                posTop[iTriangle] + (iDirection * nDeltaY), 
                          posCxL - nTriWidth/2,  posBot[iTriangle] + (iDirection * nDeltaY), 
                          posCxL + nTriWidth/2 , posBot[iTriangle] + (iDirection * nDeltaY), SSD1306_WHITE );

    // right side
    display.fillTriangle( posCxR,                posTop[iTriangle] + (iDirection * nDeltaY), 
                          posCxR - nTriWidth/2,  posBot[iTriangle] + (iDirection * nDeltaY), 
                          posCxR + nTriWidth/2 , posBot[iTriangle] + (iDirection * nDeltaY), SSD1306_WHITE );                          

    if( iDirection > 0 && posBot[iTriangle] <= 0 )
        posTop[iTriangle] = display.height() - 1 + nTriOffset;
    else if( iDirection < 0 && posBot[iTriangle] >= display.height() - 1 )
        posTop[iTriangle] = -nTriOffset;    
    else
        posTop[iTriangle] += iDirection * nDeltaY; 
  }

  // DisplayDraw();
}

