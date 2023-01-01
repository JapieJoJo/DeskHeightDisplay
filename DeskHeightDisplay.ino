extern void SetupOLED();
extern void DisplayBlack();
extern void DisplayClear();
extern void DisplayDraw();
extern void DisplayString( const char * const pszString );


extern void SetupDistance();
extern unsigned long GetDistance();
void DisplayDistance( unsigned long ulDistance );


extern void ScrollTriangles( int8_t iDirection );

void CycleTime( unsigned long ulStartTime );

void setup() 
{
  Serial.begin(9600);  // Serial Communication is starting with 9600 of baudrate speed

  SetupDistance();
  SetupOLED();
}

#define CYCLE_TIME_MS (50)

void loop() 
{
  const unsigned long  TOLERANCE      =     0; // [cm]
  const unsigned long  SCROLLDELAY    =   500; // [ms]
  const unsigned long  TEXTDELAY      = 10000; // [ms]

  static unsigned long lastChange     = millis();
  static int8_t        iDirection     = 0;
  static unsigned long ulPrevDistance = 0;
  static bool          fShowScroll    = false;
  static bool          fShowText      = true;

  unsigned long        ulStartTime    = millis();
  unsigned long        ulDistance     = GetDistance();


  DisplayClear();

  long lChange = ulPrevDistance - ulDistance;

//Serial.println( lChange );
 
  if( abs(lChange) > TOLERANCE )
  {
    fShowScroll = true;
    fShowText   = true;
    lastChange  = millis();

    ulPrevDistance = ulDistance;
  }
  
  if( millis() - lastChange > SCROLLDELAY )
    fShowScroll = false;
  
  if( millis() - lastChange > TEXTDELAY )
    fShowText = false;

  if( lChange > 0 )
    iDirection = +1;
  else if( lChange < 0 )
    iDirection = -1;
  else if( !fShowScroll )
    iDirection = 0;

  if( fShowScroll )
    ScrollTriangles( iDirection );

  if( fShowText )
    DisplayDistance( ulDistance );

  DisplayDraw();

  CycleTime( ulStartTime );
}


void CycleTime( unsigned long ulStartTime )
{
  unsigned long ulBusyTime = millis() - ulStartTime;

  if( ulBusyTime < CYCLE_TIME_MS )
  {
    delay( CYCLE_TIME_MS - ulBusyTime );
  }
}

void DisplayDistance( unsigned long ulDistance )
{
    char szDistance[32] = "";
    
    // valid measurement range is 500cm
    if( ulDistance > 500 )
      sprintf( szDistance, " --- cm" );
    else
      sprintf( szDistance, "%3lu cm", ulDistance );
    
    DisplayString( szDistance );
}