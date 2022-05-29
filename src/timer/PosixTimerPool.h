#ifndef _POSIX_TIMER_POOL_H
#define _POSIX_TIMER_POOL_H

#include <time.h>

#include <queue>

namespace faker_tsn {

class PosixTimerPool {
  private:
    unsigned int m_size;
    std::deque<timer_t> m_timers;

  public:
    PosixTimerPool(/* args */);

    ~PosixTimerPool();

    /* get timer from pool */
    timer_t getPosixTimer();

    /* crate timer */
    timer_t createPosixTimer();

    /* restore timer */
    void restorePosixTimer();

    /* release timer */
    void releaseTimer();
};

}  // namespace faker_tsn

#endif  // _POSIX_TIMER_POOL_H