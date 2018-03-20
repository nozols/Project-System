#include "Util\Logger.h"
#include "Util\SDcard.h"

bool canStart = true;

SDcard card(4, "logfile.txt");

void setup()
{
  Serial.begin(9600);
  Logger::info("Starting engines...");

  if(!card.init())
  {
    canStart = false;
  }
}

void loop()
{

}
