#ifndef _I_CLCOK_H
#define _I_CLCOK_H

#include <cstdint>
#include <iostream>

#include "../utils/reflector/Reflector.h"
#include "ITimer.h"

namespace faker_tsn {

class IClock : public ReflectObject {
  public:
    virtual ~IClock() = default;

    virtual Time::TimePoint now() = 0;
};

}  // namespace faker_tsn

#endif  // _I_CLCOK_H