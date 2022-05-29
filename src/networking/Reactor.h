#ifndef _REACTOR_H
#define _REACTOR_H

#include <chrono>
#include <memory>
#include <unordered_map>

#include "IEventHandler.h"
#include "Selector.h"

namespace faker_tsn {

class Reactor {
  private:
    Selector m_demultiplexer; /* demultiplexer */
    std::unordered_map<int, std::unordered_map<EVENT_TYPE, std::shared_ptr<IEventHandler>>>
        m_handlers; /* hashmap of FD and HANDLER */

    Reactor(/* args */); /* private Constructor */

  public:
    Reactor(const Reactor&) = delete; /* delete Copy Constructor */
    ~Reactor();

    /* get singleton */
    static Reactor& getInstance() {
        static Reactor reactor;
        return reactor;
    }

    /* register event handler for a particular event type */
    int register_handler(std::shared_ptr<IEventHandler> eventHandler, EVENT_TYPE eventType);

    /* unregister event handler  for a particular event type */
    int remove_handler(std::shared_ptr<IEventHandler> eventHandler, EVENT_TYPE eventType);

    /* entry point into the reactive event loop */
    int handle_events(std::chrono::duration<int, std::milli> timeout = std::chrono::duration<int, std::milli>(0));

    Selector& getDemultiplexer() {
        return this->m_demultiplexer;
    }

  private:
    /* change EVENT_TYPE to EpollEvent */
    void toEpollEvent(EVENT_TYPE eventType, int& evnets);

    /* chage EpollEvent to EVENT_TYPE */
    EVENT_TYPE toEventType(int event);
};

}  // namespace faker_tsn

#endif