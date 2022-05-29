#ifndef _TIME_CONTEXT_H
#define _TIME_CONTEXT_H

#include "ITimer.h"
#include "PQTimer.h"

namespace faker_tsn {

class TimeContext {
  private:
    ITimer* m_timer;

    TimeContext() {
        // TODO read config file
        this->m_timer = Reflector::getNewInstance<ITimer>("PQTimer");
    }

  public:
    ~TimeContext() = default;

    static TimeContext& getInstance() {
        static TimeContext timeContext;
        return timeContext;
    }

    inline ITimer* getTimer() {
        return this->m_timer;
    }
};

}  // namespace faker_tsn

#endif  // _TIME_CONTEXT_H