#ifndef _INFO_LOGGER_H
#define _INFO_LOGGER_H

#include "Logger.h"
#include "WarnLogger.h"

namespace faker_tsn {

class InfoLogger : public Logger {
  private:
    /* data */
  public:
    InfoLogger(/* args */);
    ~InfoLogger();

    virtual void write(const char*, const char*, int) override;
};

}  // namespace faker_tsn

#endif  // _INFO_LOGGER_H