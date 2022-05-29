#ifndef _REAL_TIME_CLOCK_H
#define _REAL_TIME_CLOCK_H

#include "IClock.h"

namespace faker_tsn {

class RealTimeClock : public IClock {
  private:
    /* data */
  public:
    RealTimeClock();
    ~RealTimeClock();

    virtual Time::TimePoint now() override;
};

}  // namespace faker_tsn

#endif  // _REAL_TIME_CLOCK_H