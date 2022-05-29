#ifndef _WARN_LOGGER_H
#define _WARN_LOGGER_H

#include "ErrorLogger.h"
#include "Logger.h"

namespace faker_tsn {

class WarnLogger : public Logger {
  private:
    /* data */
  public:
    WarnLogger(/* args */);
    ~WarnLogger();

    virtual void write(const char*, const char*, int) override;
};

}  // namespace faker_tsn

#endif  // _WARN_LOGGER_H