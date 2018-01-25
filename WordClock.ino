#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

  unsigned int GPOA0 = 0b00000000;
  unsigned int GPOB0 = 0b00000000;
  unsigned int GPOA1 = 0b00000000;
  unsigned int GPOB1 = 0b00000000;
  
void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("Word Clock Debugging");
  Serial.println("-------------------");
	
	Wire.begin(); // wake up I2C bus
  // set I/O pins to outputs
	Wire.beginTransmission(0x20);
	Wire.write(0x00); // IODIRA register
	Wire.write(0x00); // set all of port A to outputs
	Wire.endTransmission();
 
	Wire.beginTransmission(0x20);
	Wire.write(0x01); // IODIRB register
	Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();

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
	int hour = EncodedHour();
  int minute = EncodedMinute();


  //Increment to next hour if minutes are past half an hour
  if(minute > 5)
  {
    //roll over if at highest value
    if(hour == 120)
    {
      hour = 10;
    }
    
    else
    {
      hour = hour + 10;
    }
    
  }

  //Swtich-Case for setting bits corresponding to each hour
  switch (hour)
  {
    case 10: 
      GPOA1 = 0b00000000;
      GPOB1 = 0b00000100;
      break;

    case 20:
      GPOA1 = 0b10000000;
      GPOB1 = 0b00000000;
      break;

    case 30:
      GPOA1 = 0b01000000;
      GPOB1 = 0b00000000;
      break;

    case 40:
      GPOA1 = 0b00100000;
      GPOB1 = 0b00000000;
      break;

    case 50:
      GPOA1 = 0b00010000;
      GPOB1 = 0b00000000;
      break;

    case 60:
      GPOA1 = 0b00001000;
      GPOB1 = 0b00000000;
      break;

    case 70:
      GPOA1 = 0b00000100;
      GPOB1 = 0b00000000;
      break;

    case 80:
      GPOA1 = 0b00000000;
      GPOB1 = 0b10000000;
      break;

    case 90:
      GPOA1 = 0b00000000;
      GPOB1 = 0b01000000;
      break;

    case 100:
      GPOA1 = 0b00000000;
      GPOB1 = 0b00100000;
      break;

    case 110:
      GPOA1 = 0b00000000;
      GPOB1 = 0b00010000;
      break;

    case 120:
      GPOA1 = 0b00000000;
      GPOB1 = 0b00001000;
      break;
  }
  
//Serial.println(hour);
//Serial.println(GPOA1);
//Serial.println(GPOB1);
//delay(5000);

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

   Serial.println(minute);

  Wire.beginTransmission(0x20);
  Wire.write(0x12); // GPIOA
  Wire.write(GPOA0); // port A
  Wire.endTransmission();
  
  Wire.beginTransmission(0x20);
  Wire.write(0x13); // GPIOB
  Wire.write(GPOB0); // port B
  Wire.endTransmission();

  Wire.beginTransmission(0x27);
  Wire.write(0x12); // GPIOA
  Wire.write(GPOA1); // port A
  Wire.endTransmission();
  
  Wire.beginTransmission(0x27);
  Wire.write(0x13); // GPIOB
  Wire.write(GPOB1); // port B
  Wire.endTransmission();
  Serial.println("---");
  delay(1000);
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
  Serial.print("HOUR - ");
  Serial.print(tm.Hour);
    
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
    else if(tm.Minute > 5  && tm.Minute <= 10)
    { EncodedMinutes = EncodedMinutes + 1; }

    //Ten past the hour
    else if(tm.Minute > 10 && tm.Minute <= 15)
    { EncodedMinutes = EncodedMinute + 2; }

    //Quarter past the hour
    else if(tm.Minute > 15 && tm.Minute <= 20)
    { EncodedMinutes = EncodedMinutes + 3; }

    //Twenty past the hour
    else if(tm.Minute > 20 && tm.Minute <= 30)
    { EncodedMinutes = EncodedMinutes + 4; }

    //Half past the hour
    else if(tm.Minute > 30 && tm.Minute <= 40)
    { EncodedMinutes = EncodedMinutes + 5; }

    //Twenty to the next hour
    else if(tm.Minute > 40 && tm.Minute <= 45)
    { EncodedMinutes = EncodedMinutes + 6; }

    //Quarter to the next hour
    else if(tm.Minute > 45 && tm.Minute <= 50)
    { EncodedMinutes = EncodedMinutes + 7; }

    //Ten to the next hour
    else if(tm.Minute > 50 && tm.Minute <= 55)
    { EncodedMinutes = EncodedMinutes + 8; }

    //Five to the next hour
    else if(tm.Minute > 55)
    { EncodedMinutes = EncodedMinutes + 9; }
    
    else
    { EncodedMinutes = -400; }
  }
  Serial.println();
  Serial.print("MINUTE - ");
  Serial.println(tm.Minute);
  return EncodedMinutes;
}

