/*
msgTime = millis();
Logger::info("Test");
//Serial.println("Test");
Serial.println(millis() - msgTime);
msgTime = millis();
//Serial.println(1234);
Logger::info(1234);
Serial.println(millis() - msgTime);
msgTime = millis();
//Serial.println(123);
Logger::info(123);
Serial.println(millis() - msgTime);
msgTime = millis();
//Serial.println(12345);
Logger::info(12345);
Serial.println(millis() - msgTime);
msgTime = millis();
//Serial.println(0.2F);
Logger::info(0.2F);
Serial.println(millis() - msgTime);
msgTime = millis();

uint16_t countLimit = 200;
unsigned long total = 0;
unsigned long startTime = millis();
for(uint16_t i = 0; i < countLimit; i++)
{
  msgTime = millis();
  Serial.print("[INFO ");
  Serial.print(millis());
  Serial.print("] ");
  Serial.println(i);
  //Logger::info("DIT IS EEN STRING");
  //Logger::info(i);
  unsigned long current = millis() - msgTime;
  total += current;
  Serial.println(current);
}
unsigned long endTime = millis();
float average = (float)total / (float)countLimit;

Serial.print("start: ");
Serial.println(startTime);
Serial.print("end: ");
Serial.println(endTime);
Serial.print("average: ");
Serial.println(average);
Serial.print("total: ");
Serial.println(total);
Serial.print("test took: ");
Serial.println(endTime - startTime);
*/
