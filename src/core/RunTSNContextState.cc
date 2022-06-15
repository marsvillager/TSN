#include "RunTSNContextState.h"

#include "TSNContext.h"

namespace faker_tsn {

RunTSNContextState::RunTSNContextState(/* args */) {
}

RunTSNContextState::~RunTSNContextState() {
}

void RunTSNContextState::doAction(TSNContext& context) {
    auto prt = shared_from_this();
    context.setState(prt);
    INFO("RUN STATE");
    
    // crate port 1
    std::shared_ptr<IPort> port1 = std::make_shared<DataPort>(context.getDeviceName());
    shared_ptr<IPortState> creationState1 = make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // crate port 2
    std::shared_ptr<IPort> port2 = std::make_shared<DataPort>(context.getDeviceName());
    shared_ptr<IPortState> creationState2 = make_shared<CreationPortState>();
    creationState1->doAction(port2);

    // add port into port manager
    shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    portManager->appendPort(port1);
    portManager->appendPort(port2);

    // enable reactor
    Reactor::getInstance().handle_events();
}

}  // namespace faker_tsn
