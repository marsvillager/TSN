#include "UpPortState.h"
#include "IPort.h"

namespace faker_tsn {

UpPortState::UpPortState(/* args */) {
}

UpPortState::~UpPortState() {
}

void UpPortState::doAction(std::shared_ptr<IPort> port) {
    auto prt = shared_from_this();
    port->setState(prt);
    INFO("Port[" + std::string(port->getDeviceName()) + "] UP STATE");
}

}  // namespace faker_tsn
