#include "TSNContext.h"

namespace faker_tsn {

TSNContext::TSNContext(/* args */) {
    /* create PortManager */
    this->m_portManager = std::make_shared<PortManager>();
    // TODO create Timer
}

TSNContext::~TSNContext() {
}

void TSNContext::setState(const std::shared_ptr<ITSNContextState>& state) {
    this->m_state = state;
    /* new port manager */
    this->m_portManager = std::make_shared<PortManager>();
}

std::shared_ptr<ITSNContextState>& TSNContext::getState() {
    return this->m_state;
}

}  // namespace faker_tsn
