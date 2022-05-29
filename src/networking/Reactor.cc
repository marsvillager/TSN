#include "Reactor.h"

namespace faker_tsn {

Reactor::Reactor() {
}

Reactor::~Reactor() {
}

void Reactor::toEpollEvent(EVENT_TYPE eventType, int& events) {
    if (eventType == EVENT_TYPE::READ) {
        events |= EPOLLIN;
    } else if (eventType == EVENT_TYPE::WRITE) {
        events |= EPOLLOUT;
    }
}

EVENT_TYPE Reactor::toEventType(int event) {
    if (event == EPOLLIN) {
        return EVENT_TYPE::READ;
    } else if (event == EPOLLOUT) {
        return EVENT_TYPE::WRITE;
    } else {
        return EVENT_TYPE::ERROR;
    }
}

int Reactor::register_handler(std::shared_ptr<IEventHandler> eventHandler, EVENT_TYPE eventType) {
    auto handle = eventHandler->getHandle();
    int fd = handle;
    if (this->m_handlers.find(fd) == this->m_handlers.end()) { /* FD not included */
        /* add */
        std::unordered_map<EVENT_TYPE, std::shared_ptr<IEventHandler>> temp;
        temp.emplace(std::move(eventType), std::move(eventHandler));
        this->m_handlers.emplace(std::move(fd), std::move(temp));
        /* set Epoll events */
        int events = 0x000;  
        this->toEpollEvent(eventType, events);
        /* add handle to Selector */
        this->m_demultiplexer.addHandle(handle, events);
    } else { /* FD included */
        /* add */
        auto temp = this->m_handlers[fd];
        temp.emplace(std::move(eventType), std::move(eventHandler));
        /* set Epoll events */
        int events = 0;  
        for (auto it = temp.begin(); it != temp.end(); it++) {
            this->toEpollEvent(it->first, events);
        }
        /* update handle to Selector */
        this->m_demultiplexer.updateHandle(handle, events);
    }
    // this->m_handlers.emplace(std::move(eventType), std::move(eventHandler));
    return 0;
}

int Reactor::handle_events(std::chrono::duration<int, std::milli> timeout) {
    while (1) {
        int eventCount = this->m_demultiplexer.wait();
        auto events = this->m_demultiplexer.getEvents();
        /* handle events */
        for (int i = 0; i < eventCount; i++) {
            int fd = events->at(i).data.fd;
            auto mapTypeHandler = this->m_handlers[fd];

            if (events->at(i).events & EPOLLIN) {
                INFO("READ EVENT on socket: " + std::to_string(fd));
                EVENT_TYPE type = toEventType(EPOLLIN);
                auto handler = mapTypeHandler[type];
                handler->handle_event(type);
            } else if (events->at(i).events & EPOLLOUT) {
                INFO("WRITE EVENT on socket: " + std::to_string(fd));
                EVENT_TYPE type = toEventType(EPOLLOUT);
                auto handler = mapTypeHandler[type];
                handler->handle_event(type);
            }
        }
    }
    return 0;
}

}  // namespace faker_tsn
