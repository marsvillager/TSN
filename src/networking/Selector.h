#ifndef _SELECTOR_H
#define _SELECTOR_H

#include <limits.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <vector>

#include "../utils/log/Log.h"
#include "EventType.h"

namespace faker_tsn {

class Selector {
  public:
    static const uint32_t MAX_EVENTS = 10;
    static const uint32_t MAX_WAIT_TIME = INT32_MAX;

  private:
    int m_epollfd; /* epoll handle */
    std::shared_ptr<std::array<struct epoll_event, Selector::MAX_EVENTS>>
        m_events{}; /* array to store returned events */

  public:
    Selector(/* args */);
    ~Selector();

    /* add handle */
    int addHandle(HANDLE handle, int events);

    /* delete handle */
    int deleteHandle(HANDLE handle);

    /* update handle */
    int updateHandle(HANDLE handle, int events);

    /*
     * wait events
     * this method will block until events arriave
     * and return the number of events
     */
    int wait();

    /* get events */
    std::shared_ptr<std::array<struct epoll_event, Selector::MAX_EVENTS>> getEvents();
};

}  // namespace faker_tsn

#endif