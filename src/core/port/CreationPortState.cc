#include "CreationPortState.h"

namespace faker_tsn {

CreationPortState::CreationPortState(/* args */) {
}

CreationPortState::~CreationPortState() {
}

/* the real construct procedure of IPort instance */
void CreationPortState::doAction(std::shared_ptr<IPort> port) {
    auto ptr = shared_from_this();
    /* set state */
    port->setState(ptr);
    INFO("Port[" + std::string(port->getDeviceName()) + "] CREATION STATE");

    /* socket */
    try {
        port->createSocket();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::shared_ptr<IPortState> downPortState = std::make_shared<DownPortState>();
        downPortState->doAction(port);
    }

    /* register event handler */
    port->registerEventHandler();

    /* change state to up */
    std::shared_ptr<IPortState> upPortState = std::make_shared<UpPortState>();
    upPortState->doAction(port);
}

int CreationPortState::openPCAP(const char*, std::shared_ptr<pcap_t>) {
    return 0;
}

}  // namespace faker_tsn
