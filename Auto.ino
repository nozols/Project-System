#include "Util\Logger.h"
#include "Util\SDcard.h"

//SDcard card(4, "log_output.txt");

bool canStart = true;

unsigned long msgTime = 0;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");


  if(!canStart)
  {
    Logger::warning("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }
  msgTime = millis();
  Logger::info("Test");
  Serial.println(millis() - msgTime);
  msgTime = millis();
  Logger::info(1234);
  Serial.println(millis() - msgTime);
  msgTime = millis();
  Logger::info(123);
  Serial.println(millis() - msgTime);
  msgTime = millis();
  Logger::info(12345);
  Serial.println(millis() - msgTime);
  msgTime = millis();
  Logger::info(0.2F);
  Serial.println(millis() - msgTime);
  msgTime = millis();

}

void loop()
{

}
