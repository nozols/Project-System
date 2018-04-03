#include "Util\Logger.h"
#include "Util\SDcard.h"

//SDcard card(4, "log_output.txt");

bool canStart = true;

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

  Logger::info("Test");
  Logger::info(1234);
  Logger::info(123);
  Logger::info(12345);
  Logger::info(0.2F);

}

void loop()
{

}
