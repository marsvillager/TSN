#include "Selector.h"

namespace faker_tsn {

/* Consturctor */
Selector::Selector() {
    this->m_epollfd = epoll_create1(0); /* create epoll handle */
    if (this->m_epollfd == -1) {
        ERROR("failed to create epoll file descriptor");
        // handle error
    }
    this->m_events = std::make_shared<std::array<struct epoll_event, Selector::MAX_EVENTS>>();
}

/* Disturctor */
Selector::~Selector() {
    close(this->m_epollfd);
}

int Selector::addHandle(HANDLE handle, int events) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    int fd = handle;
    event.data.fd = fd;
    event.events = events; /* get event types used for epoll_ctl() */

    /* add fd */
    if (epoll_ctl(this->m_epollfd, EPOLL_CTL_ADD, fd, &event) < 0) {
        ERROR("failed to add file descriptor to epoll");
        // handle error
    }

    return 0;
}

int Selector::updateHandle(HANDLE handle, int events) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    int fd = handle;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(this->m_epollfd, EPOLL_CTL_MOD, fd, &event) < 0) {
        ERROR("failed to update file descriptor to epoll");
    }
    return 0;
}

int Selector::deleteHandle(HANDLE handle) {
    int fd = handle;
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    if (epoll_ctl(this->m_epollfd, EPOLL_CTL_DEL, fd, &event) < 0) {
        ERROR("failed to update file descriptor to epoll");
    }
    return 0;
}

int Selector::wait() {
    int eventCount = epoll_wait(this->m_epollfd, this->m_events->data(), Selector::MAX_EVENTS, Selector::MAX_WAIT_TIME);

    return eventCount;
}

std::shared_ptr<std::array<struct epoll_event, Selector::MAX_EVENTS>> Selector::getEvents() {
    return this->m_events;
}

}  // namespace faker_tsn
