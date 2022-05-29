#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdint.h>
#include <iostream>
#include <memory>

namespace faker_tsn {

class Logger {
  private:
    /* data */
  protected:
    uint8_t m_level;
    std::unique_ptr<Logger> m_nextLogger;

    Logger();

  public:
    static const uint8_t DEBUG;
    static const uint8_t INFO;
    static const uint8_t WARN;
    static const uint8_t ERROR;

    virtual ~Logger();

    virtual void write(const char*, const char*, int){};

    void log(uint8_t level, const char*, const char*, int);
};

}  // namespace faker_tsn

#endif  // _LOGGER_H