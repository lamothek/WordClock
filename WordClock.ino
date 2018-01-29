#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int RelayControl = 12; //Relay control connected to digital pin 12
int ButtonRead = 13;   //Pushbutton connected to digital pin 13
int Counter = 0;

unsigned int GPOA0 = 0b00000000;
unsigned int GPOB0 = 0b00000000;
unsigned int GPOA1 = 0b00000000;
unsigned int GPOB1 = 0b00000000;

void setup()
{
  pinMode(RelayControl, OUTPUT); //Set the digital pin 12 as output
  pinMode(ButtonRead, INPUT);    // sets the digital pin 13 as input
   
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("Word Clock Debugging");
  Serial.println("-------------------");

  //Setup  first I/0 chip as addres 0x20 (all grouned)
	Wire.begin(); // wake up I2C bus
	Wire.beginTransmission(0x20);
	Wire.write(0x00); // IODIRA register
	Wire.write(0x00); // set all of port A to outputs
	Wire.endTransmission();
 
	Wire.beginTransmission(0x20);
	Wire.write(0x01); // IODIRB register
	Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();

  //Setup second I/0 chip as 0x27 (all 5V)
  Wire.beginTransmission(0x27);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();
 
  Wire.beginTransmission(0x27);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
}

void loop()
{
  PrintTime(); //Prints current time from RTC
   
  //Call functions to return encoded time values for hours and minutes
	int hour = EncodedHour();
  int minute = EncodedMinute();

Counter = OutputEnable(Counter);
 


  
  
  //Increment to next hour if minutes are past half an hour
  if(minute > 5)
  {
    //Roll over if at highest value
    if(hour == 120)
    {
      hour = 10;
    }

    //Increment hour if more than half past the current hour
    else
    {
      hour = hour + 10;
    }
  }

  //Swtich-Case for setting bits corresponding to each hour
  switch (hour)
  {
    case 10: //Hour - 0,12
      GPOA1 = 0b10000000;
      GPOB1 = 0b00000000;
      break;

    case 20:  //Hour - 1, 13
      GPOA1 = 0b10000000;
      GPOB1 = 0b00000000;
      break;

    case 30:  //Hour - 2, 14
      GPOA1 = 0b01000000;
      GPOB1 = 0b00000000;
      break;

    case 40:  //Hour - 3, 15
      GPOA1 = 0b00100000;
      GPOB1 = 0b00000000;
      break;

    case 50:  //Hour - 4, 16
      GPOA1 = 0b00010000;
      GPOB1 = 0b00000000;
      break;

    case 60:  //Hour - 5, 17
      GPOA1 = 0b00001000;
      GPOB1 = 0b00000000;
      break;

    case 70:  //Hour - 6, 18
      GPOA1 = 0b00000100;
      GPOB1 = 0b00000000;
      break;

    case 80:  //Hour - 7, 19
      GPOA1 = 0b00000000;
      GPOB1 = 0b10000000;
      break;

    case 90:  //Hour - 8, 20
      GPOA1 = 0b00000000;
      GPOB1 = 0b01000000;
      break;

    case 100: //Hour - 9, 21
      GPOA1 = 0b00000000;
      GPOB1 = 0b00100000;
      break;

    case 110: //Hour - 10, 22
      GPOA1 = 0b00000000;
      GPOB1 = 0b00010000;
      break;

    case 120: //Hour - 11, 23
      GPOA1 = 0b00000000;
      GPOB1 = 0b00001000;
      break;
  }

  //Swtich-Case for setting bits corresponding to each minute
  switch (minute)
  {
    case 0: //Oclock
      GPOA0 = 0b11001000;
      GPOB0 = 0b00000000;
      break;

    case 1: //IT IS FIVE PAST 
      GPOA0 = 0b11010000;
      GPOB0 = 0b10000000;
      break;

    case 2: //IT IS TEN PAST
      GPOA0 = 0b11010000;
      GPOB0 = 0b01000000;
      break;

    case 3: //IT IS QUARTER PAST
      GPOA0 = 0b11010000;
      GPOB0 = 0b00100000;
      break;

    case 4: //IT IS TWENTY PAST
      GPOA0 = 0b11010000;
      GPOB0 = 0b00010000;
      break;

    case 5: //IT IS HALF PAST
      GPOA0 = 0b11010000;
      GPOB0 = 0b00001000;
      break;

    case 6: //IT IS TWENTY MINUTES TO
      GPOA0 = 0b11100100;
      GPOB0 = 0b00010000;
      break;

    case 7: //IT IS QUARTER TO
      GPOA0 = 0b11100000;
      GPOB0 = 0b00100000;
      break;

    case 8: //IT IS TEN MINUTES TO
      GPOA0 = 0b11100100;
      GPOB0 = 0b01000000;
      break;

    case 9: // IT IS FIVE MINUTES TO
      GPOA0 = 0b11100100;
      GPOB0 = 0b10000000;
      break;
  }

  //Write to port A0
  Wire.beginTransmission(0x20);
  Wire.write(0x12); //GPIOA
  Wire.write(GPOA0);
  Wire.endTransmission();

  //Write to port B0
  Wire.beginTransmission(0x20);
  Wire.write(0x13); //GPIOB
  Wire.write(GPOB0);
  Wire.endTransmission();

  //Write to port A1
  Wire.beginTransmission(0x27);
  Wire.write(0x12); //GPIOA
  Wire.write(GPOA1);
  Wire.endTransmission();

  //Write to port B1
  Wire.beginTransmission(0x27);
  Wire.write(0x13); //GPIOB
  Wire.write(GPOB1);
  Wire.endTransmission();
  delay(1000);
}

int OutputEnable(int Counter)
{
  
  int val = digitalRead(ButtonRead);     // read the input pin

  //Increment counter if push button is high
  if (val == HIGH)
  {
    Counter ++;
  }

  Serial.println(Counter);

  //Turn Relay Control HIGH if counter is even
  if(Counter % 2 == 0)
  {
    digitalWrite(RelayControl, HIGH); 
  }

  else
  {
    digitalWrite(RelayControl, LOW);
  }

  return Counter;
}

//------------------------------------------------------------------
//Function to read RTC and return an integer value for each hour
//Negative values indicate some sort of failure
//------------------------------------------------------------------
int EncodedHour()
{
  tmElements_t tm;
  int EncodedHours = 0;

  //Reding and setting hour
  if (RTC.read(tm)) 
  {
    if(tm.Hour == 0 || tm.Hour == 12)
    { EncodedHours = 10; }
    
    else if(tm.Hour == 1 || tm.Hour == 13)
    { EncodedHours = 20; }
    
    else if(tm.Hour == 2 || tm.Hour == 14)
    { EncodedHours = 30; }

    else if(tm.Hour == 3 || tm.Hour == 15)
    { EncodedHours = 40; }

    else if(tm.Hour == 4 || tm.Hour == 16)
    { EncodedHours = 50; }

    else if(tm.Hour == 5 || tm.Hour == 17)
    { EncodedHours = 60; }

    else if(tm.Hour == 6 || tm.Hour == 18)
    { EncodedHours = 70; }

    else if(tm.Hour == 7 || tm.Hour == 19)
    { EncodedHours = 80; }

    else if(tm.Hour == 8 || tm.Hour == 20)
    { EncodedHours = 90; }

    else if(tm.Hour == 9 || tm.Hour == 21)
    { EncodedHours = 100; }
    
    else if(tm.Hour == 10 || tm.Hour == 22)
    { EncodedHours = 110; }

    else if(tm.Hour == 11 || tm.Hour == 23)
    { EncodedHours = 120; }
    
    else
    { EncodedHours = -300; }
  }
  
  //Printing for debugging purposes
  Serial.print("HOUR - ");
  Serial.print(tm.Hour);
  Serial.print(", ENCODED HOUR - ");
  Serial.print(EncodedHours);
  
  return EncodedHours;
}

//------------------------------------------------------------------
//Function to read RTC and return an integer value for each minute
//Negative values indicate some sort of failure
//------------------------------------------------------------------
int EncodedMinute()
{
  tmElements_t tm;
  int EncodedMinutes = 0;
  
   //Reading and setting minutes
   if (RTC.read(tm)) 
   {
    //O'clock
    if(tm.Minute >= 0 && tm.Minute <= 5)
    { EncodedMinutes = EncodedMinutes; }

    //Five past the hour
    else if(tm.Minute >= 5  && tm.Minute <= 10)
    { EncodedMinutes = EncodedMinutes + 1; }

    //Ten past the hour
    else if(tm.Minute >= 10 && tm.Minute <= 15)
    { EncodedMinutes = EncodedMinute + 2; }

    //Quarter past the hour
    else if(tm.Minute >= 15 && tm.Minute <= 20)
    { EncodedMinutes = EncodedMinutes + 3; }

    //Twenty past the hour
    else if(tm.Minute >= 20 && tm.Minute <= 30)
    { EncodedMinutes = EncodedMinutes + 4; }

    //Half past the hour
    else if(tm.Minute >= 30 && tm.Minute <= 40)
    { EncodedMinutes = EncodedMinutes + 5; }

    //Twenty to the next hour
    else if(tm.Minute >= 40 && tm.Minute <= 45)
    { EncodedMinutes = EncodedMinutes + 6; }

    //Quarter to the next hour
    else if(tm.Minute >= 45 && tm.Minute <= 50)
    { EncodedMinutes = EncodedMinutes + 7; }

    //Ten to the next hour
    else if(tm.Minute >= 50 && tm.Minute <= 55)
    { EncodedMinutes = EncodedMinutes + 8; }

    //Five to the next hour
    else if(tm.Minute > 55)
    { EncodedMinutes = EncodedMinutes + 9; }
    
    else
    { EncodedMinutes = -400; }
  }
   
  //Printing for debugging purposes
  Serial.println();
  Serial.print("MINUTE - ");
  Serial.print(tm.Minute);
  Serial.print(", ENCODED MINUTE - ");
  Serial.print(EncodedMinutes);
  Serial.println();
  Serial.println("---");
  
  return EncodedMinutes;
}

//------------------------------------------------------------------
//Prints time read from RTC. H:M:S - DD/MM/YYYY
//------------------------------------------------------------------
void PrintTime(void)
{
  tmElements_t tm;
  
   if (RTC.read(tm))
   {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  }
  
  else 
  {
    if (RTC.chipPresent())
    {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    }
    else
    {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
}

//------------------------------------------------------------------
//For formatting RTC time 
//------------------------------------------------------------------
void print2digits(int number)
{
  if (number >= 0 && number < 10)
  {
    Serial.write('0');
  }
  Serial.print(number);
}

