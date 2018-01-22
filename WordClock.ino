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
}

void loop()
{
  
	int exampel = GetIntegerTime();
	Serial.println(exampel);
  delay(1000);

  
	
	/*
	Wire.beginTransmission(0x20);
	Wire.write(0x20); //GPIOA
	Wire.write(0b10101010 ); 
	Wire.endTransmission();
  Serial.print("Exicuted");
  */

}

//------------------------------------------------------------------
//Function to read RTC and return an integer value for each hour
//and minute combination possible. Negative values indicate
//some sort of failure
//------------------------------------------------------------------
int GetIntegerTime()
{
  tmElements_t tm;
  int EncodedTime;

  //Reding and setting hour
  if (RTC.read(tm)) 
  {
    if(tm.Hour == 0 || tm.Hour == 12)
    { EncodedTime = 10; }
    
    else if(tm.Hour == 1 || tm.Hour == 13)
    { EncodedTime = 20; }
    
    else if(tm.Hour == 2 || tm.Hour == 14)
    { EncodedTime = 30; }

    else if(tm.Hour == 3 || tm.Hour == 15)
    { EncodedTime = 40; }

    else if(tm.Hour == 4 || tm.Hour == 16)
    { EncodedTime = 50; }

    else if(tm.Hour == 5 || tm.Hour == 17)
    { EncodedTime = 60; }

    else if(tm.Hour == 6 || tm.Hour == 18)
    { EncodedTime = 70; }

    else if(tm.Hour == 7 || tm.Hour == 19)
    { EncodedTime = 80; }

    else if(tm.Hour == 8 || tm.Hour == 20)
    { EncodedTime = 90; }

    else if(tm.Hour == 9 || tm.Hour == 21)
    { EncodedTime = 100; }
    
    else if(tm.Hour == 10 || tm.Hour == 22)
    { EncodedTime = 110; }

    else if(tm.Hour == 11 || tm.Hour == 23)
    { EncodedTime = 120; }
    
    else
    { EncodedTime = -300; }
  }

   //Reading and setting minutes
   if (RTC.read(tm)) 
   {
    //O'clock
    if(tm.Minute > 0 && tm.Minute <= 5)
    { EncodedTime = EncodedTime; }

    //Five past the hour
    else if(tm.Minute > 5  && tm.Minute <= 10)
    { EncodedTime = EncodedTime + 1; }

    //Ten past the hour
    else if(tm.Minute > 10 && tm.Minute <= 15)
    { EncodedTime = EncodedTime + 2; }

    //Quarter past the hour
    else if(tm.Minute > 15 && tm.Minute <= 20)
    { EncodedTime = EncodedTime + 3; }

    //Twenty past the hour
    else if(tm.Minute > 20 && tm.Minute <= 30)
    { EncodedTime = EncodedTime + 4; }

    //Half past the hour
    else if(tm.Minute > 30 && tm.Minute <= 40)
    { EncodedTime = EncodedTime + 5; }

    //Twenty to the next hour
    else if(tm.Minute > 40 && tm.Minute <= 45)
    { EncodedTime = EncodedTime + 6; }

    //Quarter to the next hour
    else if(tm.Minute > 45 && tm.Minute <= 50)
    { EncodedTime = EncodedTime + 7; }

    //Ten to the next hour
    else if(tm.Minute > 50 && tm.Minute <= 55)
    { EncodedTime = EncodedTime + 8; }

    //Five to the next hour
    else if(tm.Minute > 55)
    { EncodedTime = EncodedTime + 9; }
    
    else
    { EncodedTime = -400; }
  }
    
  return EncodedTime;
}



