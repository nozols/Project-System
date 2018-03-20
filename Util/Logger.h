#ifndef Logger_h
#define Logger_h

#define LOG_DEBUG 0
#define LOG_WARNING 1
#define LOG_ERROR 2
#define LOG_INFO 3
#define LOG_DATA 4

#include "Arduino.h"
#include "Logger.h"

#define LOGGER_USER_FRIENDLY false   // INCREASES LOOPTIME BY 10 MICROSECONDS

/**
 * Define logger class
 */
class Logger{
  public:
    template<typename T> static void log(uint8_t loglevel, T value);
    template<typename T> static void debug(T value);
    template<typename T> static void info(T value);
    template<typename T> static void warning(T value);
    template<typename T> static void error(T value);
    template<typename T> static void data(T value);
    template<typename T> static void logarray(uint8_t loglevel, T& value);
    static uint8_t _loglevel;
    static bool _dataEnabled;
    static bool _userFriendly;
    static void getPrefixString(uint8_t loglevel, char *buffer);
    static void getLevelWord(uint8_t loglevel, char *buffer);
    static void getMillisString(char *buffer);
};
/*
 * Create templates to check if variable is pointer
 */
template<typename T>
struct is_pointer { static const bool value = false; };

template<typename T>
struct is_pointer<T*> { static const bool value = true; };

/*
 * Define template methods here. Can't be done in Logger.cpp
 */
template<typename T> void Logger::log(uint8_t loglevel, T value){
  if(loglevel >= Logger::_loglevel)
  {
    char prefixBuffer[32] = {0};
    Logger::getPrefixString(loglevel, prefixBuffer);
    Serial.print(prefixBuffer);

    Serial.println(value);
  }
}

template<typename T> void Logger::debug(T value){
  Logger::log(LOG_DEBUG, value);
}

template<typename T> void Logger::info(T value){
  Logger::log(LOG_INFO, value);
}

template<typename T> void Logger::warning(T value){
  Logger::log(LOG_WARNING, value);
}

template<typename T> void Logger::error(T value){
  Logger::log(LOG_ERROR, value);
}

template<typename T> void Logger::data(T value){
  if(Logger::_dataEnabled)
  {
    Logger::log(LOG_DATA, value);
  }
}

template<typename T> void Logger::logarray(uint8_t loglevel, T& value){
  if(loglevel >= Logger::_loglevel)
  {
    char prefixBuffer[32] = {0};
    Logger::getPrefixString(loglevel, prefixBuffer);
    Serial.print(prefixBuffer);

    uint16_t arraysize = sizeof(T) / sizeof(value[0]);
    Serial.print("{");
    for(uint16_t i = 0; i < arraysize; i++)
    {
      Serial.print(value[i]);
      if(i < arraysize - 1)
      {
        Serial.print(" ");
      }
    }
    Serial.println("}");
  }
}


#endif
