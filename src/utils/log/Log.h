#ifndef _LOG_H
#define _LOG_H

#include "DebugLogger.h"

#define INFO(x) Log::info(x, __FILE__, __LINE__)
#define DEBUG(x) Log::debug(x, __FILE__, __LINE__)
#define WARN(x) Log::warn(x, __FILE__, __LINE__)
#define ERROR(x) Log::error(x, __FILE__, __LINE__)

namespace faker_tsn {

class Log {
  private:
    Log(/* args */);

  public:
    static const uint8_t DEBUG_LEVEL;
    static const uint8_t INFO_LEVEL;
    static const uint8_t WARN_LEVEL;
    static const uint8_t ERROR_LEVEL;
    static uint8_t level;

    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

    static Logger& getLogger();

    static void setLoggerLevel(const uint8_t&);

    static void setLoggerStreamFormat();  // TODO

    static void info(const char*, const char*, int);

    static void info(const std::string&, const char*, int);

    static void debug(const char*, const char*, int);

    static void debug(const std::string&, const char*, int);

    static void warn(const char*, const char*, int);

    static void warn(const std::string&, const char*, int);

    static void error(const char*, const char*, int);

    static void error(const std::string&, const char*, int);
};

}  // namespace faker_tsn

#endif  // _LOG_H