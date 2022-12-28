//
// ECHOSOUNDER
//
#define ECHO_PIN                2  // attach pin D2 Arduino to pin Echo of HC-SR04
#define TRIGGER_PIN             3  //attach pin D3 Arduino to pin Trig of HC-SR04

#define FILTER_SIZE             20
#define FILTER_LENGTH           (FILTER_SIZE - 1)


void SetupDistance();
unsigned long GetDistance();
void DisplayDistance( unsigned long lDistance, bool fOnChangeOnly );


void SetupDistance() 
{
  pinMode(TRIGGER_PIN, OUTPUT);  // Sets the TRIGGER_PIN as an OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Sets the ECHO_PIN as an INPUT
}


unsigned long GetDistance() 
{
  unsigned long        ulDuration;  // duration of sound wave travel
  unsigned long        ulDistance;  // distance measurement

  static unsigned long aDistance[FILTER_SIZE] = { 0 };
  static size_t        iIndex = 0;

  // clears the trigger pin condition
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  // sets the trigger pin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // reads the echo pin, returns the sound wave travel time in microseconds
  ulDuration = pulseIn(ECHO_PIN, HIGH);

  // calculate distance
  ulDistance = (unsigned long )(ulDuration * 0.034 / 2);  // Speed of sound wave divided by 2 (go and back)

  aDistance[iIndex++] = ulDistance;

  if (iIndex > FILTER_LENGTH)
    iIndex = 0;  

  unsigned long iSum = 0;

  for (size_t iAvg = 0; iAvg < FILTER_SIZE; ++iAvg) {
    iSum += aDistance[iAvg];
  }

  return (unsigned long)(iSum / FILTER_SIZE + 0.5);
}
