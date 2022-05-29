#ifndef _DEBUG_LOGGER_H
#define _DEBUG_LOGGER_H

#include "InfoLogger.h"
#include "Logger.h"

namespace faker_tsn {

class DebugLogger : public Logger {
  private:
    /* data */
  public:
    DebugLogger(/* args */);
    ~DebugLogger();

    virtual void write(const char*, const char*, int) override;
};

}  // namespace faker_tsn

#endif  // _DEBUG_LOGGER_H