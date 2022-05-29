#ifndef _I_EVENT_HANDLER_H
#define _I_EVENT_HANDLER_H

#include <assert.h>
#include <memory>
#include "EventType.h"

namespace faker_tsn {

class IEventHandler {
  public:
    virtual ~IEventHandler() = default;

    /* hook method that is called by Reactor */
    virtual void handle_event(EVENT_TYPE eventType) = 0;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() = 0;
};

}  // namespace faker_tsn

#endif