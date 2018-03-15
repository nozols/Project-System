#include "Arduino.h"
#include "Logger.h"

/**
 * @var Stores loglevel that is currently used
 */
uint8_t Logger::_loglevel = LOG_DEBUG;
/**
 * @var enable/disable data
 */
bool Logger::_dataEnabled = true;

/**
 * Get the prefix string for the loglevel
 * @param loglevel {uint8_t} - loglevel
 * @param buffer (*char) - the buffer to store the word in (AT LEAST 32!)
 */
void Logger::getPrefixString(uint8_t loglevel, char *buffer)
{
  buffer[0] = '[';
  char wordBuffer[10] = {0};
  Logger::getLevelWord(loglevel, wordBuffer);

  strcat(buffer, wordBuffer);
  strcat(buffer, " ");
  Logger::getMillisString(wordBuffer);
  strcat(buffer, wordBuffer);
  strcat(buffer, "]");
}

/**
 * Get the word which correspondents with the level
 * @param loglevel {uint8_t} - loglevel
 * @param buffer (*char) - the buffer to store the word in
 */
void Logger::getLevelWord(uint8_t loglevel, char *buffer)
{
  switch(loglevel)
  {
    case LOG_DEBUG:
      strcpy(buffer, "DEBUG");
      break;
    case LOG_WARNING:
      strcpy(buffer, "WARNING");
      break;
    case LOG_ERROR:
      strcpy(buffer, "ERROR");
      break;
    case LOG_INFO:
      strcpy(buffer, "INFO");
      break;
    case LOG_DATA:
      strcpy(buffer, "DATA");
      break;
    default:
      strcpy(buffer, "LOGERROR!");
  }
}

/**
 * Get the current millis as a string
 * @param buffer {*char} - buffer to store string in
 */
void Logger::getMillisString(char *buffer)
{
  #if LOGGER_USER_FRIENDLY

  uint32_t mils = millis();
  uint32_t divider = 3600000;
  uint32_t hours = mils / divider;
  mils %= divider;
  divider = 60000;
  uint16_t minutes = mils / divider;
  mils %= divider;
  divider = 1000;
  uint16_t seconds = mils / divider;
  mils %= divider;

  sprintf(buffer, "%ld", hours);
  strcat(buffer, ":");
  sprintf(buffer + strlen(buffer), "%d", minutes);
  strcat(buffer, ":");
  sprintf(buffer + strlen(buffer), "%d", seconds);
  strcat(buffer, ":");
  sprintf(buffer + strlen(buffer), "%ld", mils);

  #else

  sprintf(buffer, "%ld", millis());

  #endif
}
