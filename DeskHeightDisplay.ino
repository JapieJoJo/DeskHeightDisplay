extern void SetupOLED();
extern void DisplayBlack();
extern void DisplayString( const char * const pszString );


extern void SetupDistance();
extern unsigned long GetDistance();
extern void DisplayDistance( unsigned long lDistance, bool fOnChangeOnly );

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
  unsigned long ulStartTime = millis();

  DisplayDistance( GetDistance(), true );

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

void DisplayDistance( unsigned long ulDistance, bool fOnChangeOnly )
{
  static unsigned long ulPrevDistance = 0;
  static unsigned long ulLastUpdate   = 0;
  static bool          fDisplayActive = true;

  unsigned long        ulTolerance    = 0;      // [cm]
  bool                 fChange;
  char                 szDistance[32] = "";

  // change larger than tolerance?
  fChange = abs(ulPrevDistance - ulDistance) > ulTolerance;

  // store previous value
  ulPrevDistance  = ulDistance;
  
  // early exit when we only trigger the display on change - but there is no change
  // but remain a but longer when we display the last change   
  if( !(fDisplayActive || fChange || !fOnChangeOnly) )
   return;   

  unsigned long ulNow = millis();
  
  // update the value or dim the screen delayed to protect the OLED display
  if( fChange )
  {
    // valid measurement range is 500cm
    if( ulDistance > 500 )
      sprintf( szDistance, " --- cm" );
    else
      sprintf( szDistance, "%3d cm", ulDistance );
    
    DisplayString( szDistance );
    
    ulLastUpdate   = ulNow;
    fDisplayActive = true;
  }
  else if( ulNow - ulLastUpdate > 5000 )
  {
    // no change anymore - clear display - prevent burn in
    fDisplayActive = false;
    DisplayBlack();
  }
}