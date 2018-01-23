#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

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
}

void loop()
{
	int hour = EncodedHour();
  int minute = EncodedMinute();

  int GPOA0 [8] = {0,0,0,0,0,0,1,1};
  int GPOB0 [8] = {0,0,0,0,0,0,0,0};
  int GPOA1 [8] = {0,0,0,0,0,0,0,0};

  //Swtich-Case for setting bits corresponding to each hour
  switch (hour)
  {
    case 10:
      GPOA1[1] = 1;
      break;

    case 20:
      GPOB0[4] = 1;
      break;

    case 30:
      GPOB0[3] = 1;
      break;

    case 40:
      GPOB0[2] = 1;
      break;

    case 50:
      GPOB0[1] = 1;
      break;

    case 60:
      GPOB0[0] = 1;
      break;

    case 70:
      GPOA1[7] = 1;
      break;

    case 80:
      GPOA1[6] = 1;
      break;;

    case 90:
      GPOA1[5] = 1;
      break;;

    case 100:
      GPOA1[4] = 1;
      break;

    case 110:
      GPOA1[3] = 1;
      break;

    case 120:
      GPOA1[2] = 1;
      break;
  }

  //Swtich-Case for setting bits corresponding to each minute
  switch (minute)
  {
    case 0:
      GPOA1[3] = 1; //Oclock
      break;

    case 1:
      GPOA1[2] = 1; //Five
      GPOA1[4] = 1; //Past
      break;

    case 2:
      GPOA0[1] = 1; //Ten
      GPOA1[4] = 1; //Past
      break;

    case 3:
      GPOA1[0] = 1; //Quarter
      GPOA1[4] = 1; //Past
      break;

    case 4:
      GPOB0[7] = 1; //Twenty
      GPOA1[4] = 1; //Past
      break;

    case 5:
      GPOB0[6] = 1; //Half
      GPOA1[4] = 1; //Past
      break;

    case 6:
      GPOB0[7] = 1; //Twenty
      GPOB0[5] = 1; //Minutes
      GPOA0[5] = 1; //To
      break;

    case 7:
      GPOA0[0] = 1; //Quarter
      GPOA0[5] = 1; //To
      break;

    case 8:
      GPOA0[1] = 1; //Ten
      GPOB0[5] = 1; //Minutes
      GPOA0[5] = 1; //To
      break;

    case 9:
      GPOA0[2] = 1; //Five
      GPOB0[5] = 1; //Minutes
      GPOA0[5] = 1; //To
      break;
  }
  
 for(int i = 7; i > 0; i--)
{
  Serial.print(GPOA0[i]);
}
Serial.print("|");

 for(int i = 7; i > 0; i--)
{
  Serial.print(GPOB0[i]);
}
Serial.print("|");
 for(int i = 7; i > 0; i--)
{
  Serial.print(GPOA1[i]);
}
 Serial.println();
delay(5000);


  Wire.beginTransmission(0x20);
  Wire.write(0x12); // GPIOA
  Wire.write(0b11111111); // port A
  Wire.endTransmission();
  
  Wire.beginTransmission(0x20);
  Wire.write(0x13); // GPIOB
  Wire.write(0b11111111); // port B
  Wire.endTransmission();


}

//------------------------------------------------------------------
//Function to read RTC and return an integer value for each hour
//Negative values indicate some sort of failure
//------------------------------------------------------------------
int EncodedHour()
{
  tmElements_t tm;
  int EncodedHours;

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
    if(tm.Minute > 0 && tm.Minute <= 5)
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
    
  return EncodedMinutes;
}

