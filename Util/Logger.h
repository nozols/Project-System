#ifndef Logger_h
#define Logger_h

#define LOG_DEBUG 0
#define LOG_WARNING 1
#define LOG_ERROR 2
#define LOG_INFO 3
#define LOG_DATA 4

#include "Arduino.h"

#define LOGGER_USER_FRIENDLY false   // may dramatically decrease performance!

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
    static uint8_t _loglevel;
    static bool _dataEnabled;
    static bool _userFriendly;
    static void getPrefixString(uint8_t loglevel, char *buffer);
    static void getLevelWord(uint8_t loglevel, char *buffer);
    static void getMillisString(char *buffer);
};

/*
 * Define template methods here. Can't be done in Logger.cpp
 */
template<typename T> void Logger::log(uint8_t loglevel, T value){
  if(loglevel >= Logger::_loglevel)
  {
    char prefixBuffer[32] = {0};
    Logger::getPrefixString(loglevel, prefixBuffer);
    Serial.print(prefixBuffer);
    Serial.print(" ");
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


#endif
