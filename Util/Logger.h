#ifndef Logger_h
#define Logger_h

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3
#define LOG_DATA 4
#define LOG_OFF 5
#define LOGGER_BUFFER_SIZE 32

#ifndef LOGGER_SD_ENABLED
#define LOGGER_SD_ENABLED false      // DECREASES PERFORMANCE
#endif
#ifndef LOGGER_USER_FRIENDLY
#define LOGGER_USER_FRIENDLY false   // INCREASES LOOPTIME BY 10 MICROSECONDS
#endif

#include "Arduino.h"

#if LOGGER_SD_ENABLED
#include "SDcard.h"
#endif

#include "Logger.h"


/**
 * Logger class
 * Log data
 */
class Logger{
  public:
    template<typename T> static void log(uint8_t loglevel, T value);
    template<typename T> static void debug(T value);
    template<typename T> static void info(T value);
    template<typename T> static void warning(T value);
    template<typename T> static void error(T value);
    template<typename T> static void data(T value);
    template<typename T, typename TV> static void data(T name, TV value);
    template<typename T> static void logarray(uint8_t loglevel, T& value);
    static void getPrefixString(uint8_t loglevel, char *buffer);
    static void getLevelWord(uint8_t loglevel, char *buffer);
    static void getMillisString(char *buffer);
    static bool init();

    #if LOGGER_SD_ENABLED
    static SDcard _sdcard;
    static uint8_t _loglevelSD;
    #endif

    static uint8_t _loglevel;
    static bool _dataEnabled;
    static bool _userFriendly;
};

/*
 * Define template methods here. Can't be done in Logger.cpp
 */
template<typename T> void Logger::log(uint8_t loglevel, T value){
  char prefixBuffer[32] = {0};
  Logger::getPrefixString(loglevel, prefixBuffer);

  if(loglevel >= Logger::_loglevel)
  {
    Serial.print(prefixBuffer);

    Serial.println(value);
  }

  #if LOGGER_SD_ENABLED

  if(loglevel >= Logger::_loglevelSD)
  {
    Logger::_sdcard.log(prefixBuffer, value);
  }

  #endif
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

template<typename T, typename TV> void Logger::data(T name, TV value){
  if(Logger::_dataEnabled){
    char prefixBuffer[LOGGER_BUFFER_SIZE] = {0};
    Logger::getPrefixString(LOG_DATA, prefixBuffer);
    Serial.print(prefixBuffer);
    Serial.print(name);
    Serial.print(" ");
    Serial.println(value);
  }
}

template<typename T> void Logger::logarray(uint8_t loglevel, T& value){
  if(loglevel >= Logger::_loglevel)
  {
    char prefixBuffer[LOGGER_BUFFER_SIZE] = {0};
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
