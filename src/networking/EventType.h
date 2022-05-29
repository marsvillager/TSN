#ifndef _EVNET_TYPE_H
#define _EVNET_TYPE_H

#include "../core/port/IPort.h"

namespace faker_tsn {

// typedef IPort HANDLE;
typedef int HANDLE;

enum EVENT_TYPE {
    READ,
    WRITE,
    ERROR
};

}  // namespace faker_tsn

#endif  // _EVNET_TYPE_H