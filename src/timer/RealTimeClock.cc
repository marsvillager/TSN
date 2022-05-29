#include "RealTimeClock.h"

#include "../utils/log/Log.h"

namespace faker_tsn {

REFLECT(RealTimeClock);

RealTimeClock::RealTimeClock() {
    INFO("Construct RealTimeClock");
}

RealTimeClock::~RealTimeClock() {
}

Time::TimePoint RealTimeClock::now() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    Time::TimePoint tp(ts.tv_sec, ts.tv_nsec);
    return tp;
}

}  // namespace faker_tsn
