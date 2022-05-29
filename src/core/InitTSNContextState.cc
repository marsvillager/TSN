#include "InitTSNContextState.h"

#include "TSNContext.h"

namespace faker_tsn {

InitTSNContextState::InitTSNContextState(/* args */) {
}

InitTSNContextState::~InitTSNContextState() {
}

void InitTSNContextState::doAction(TSNContext& context) {
    auto prt = shared_from_this();
    context.setState(prt);
    INFO("START STATE");

    // TODO initilize TSN Context
}

}  // namespace faker_tsn
