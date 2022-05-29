#ifndef _I_PROT_STATE
#define _I_PROT_STATE

#include "../../utils/log/Log.h"

namespace faker_tsn {

class IPort;

/* PortState Interface */
class IPortState {
  public:
    virtual ~IPortState() = default;

    /* do something when state changed */
    virtual void doAction(std::shared_ptr<IPort>) = 0;
};

}  // namespace faker_tsn

#endif