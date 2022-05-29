#include "DownPortState.h"
#include "IPort.h"

namespace faker_tsn {

DownPortState::DownPortState(/* args */) {
}

DownPortState::~DownPortState() {
}

void DownPortState::doAction(std::shared_ptr<IPort> port) {
    auto prt = shared_from_this();
    port->setState(prt);
    INFO("Port[" + std::string(port->getDeviceName()) + "] DOWN STATE");
}

}  // namespace faker_tsn
