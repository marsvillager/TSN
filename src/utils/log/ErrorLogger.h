#ifndef _ERROR_LOGGER_H
#define _ERROR_LOGGER_H

#include "Logger.h"

namespace faker_tsn {

class ErrorLogger : public Logger {
  private:
    /* data */
  public:
    ErrorLogger(/* args */);
    ~ErrorLogger();

    virtual void write(const char*, const char*, int) override;
};

}  // namespace faker_tsn

#endif  // _ERROR_LOGGER_H