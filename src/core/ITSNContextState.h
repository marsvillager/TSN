#ifndef _I_TSN_CONTEXT_STATE_H
#define _I_TSN_CONTEXT_STATE_H

#include <iostream>
#include <memory>

#include "../utils/log/Log.h"

namespace faker_tsn {

class TSNContext;

class ITSNContextState {
  public:
    virtual ~ITSNContextState() = default;

    /* do something when state changed */
    virtual void doAction(TSNContext&) = 0;
};

}  // namespace faker_tsn

#endif