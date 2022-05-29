#include "IEEE8021Queue.h"

#include "../../utils/config/ConfigSetting.h"

namespace faker_tsn {

IEEE8021Queue::IEEE8021Queue(unsigned short portIndex, uint8_t pcp)
    : m_portIndex(portIndex), m_pcp(pcp) {
    ConfigSetting& cs = ConfigSetting::getInstance();
    this->m_deviceName = cs.get<std::string>("deviceName");
    INFO("Construct " + this->m_deviceName + ".port" + std::to_string(portIndex) + ".queue" + std::to_string(pcp));
    /* initialize innner buffer */
    int capacity = cs.get<int>("switch.queue.capacity");
    this->m_innerBuffer = std::make_shared<InnerBuffer>(portIndex, pcp, capacity);
    /* initialize transimssion selection algorithm */
    std::string transmissionSelectionAlgorithmClass = cs.get<std::string>("switch.queue.transmissionSelectionAlgorithm");
    this->m_transmissionSelectionAlgorithm = std::make_shared<TransmissionSelectionAlgorithm>(this->m_innerBuffer);
}

IEEE8021Queue::~IEEE8021Queue() {}

void IEEE8021Queue::enqueue(IFrameBody* frame) {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + " enqueue");

    this->m_innerBuffer->enqueue(frame);
}

IFrameBody* IEEE8021Queue::dequeue() {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + " dequeue");

    IFrameBody* frame = this->m_innerBuffer->dequeue();
    if (frame != nullptr)
        return frame;
    return nullptr;
}

bool IEEE8021Queue::isEmpty() {
    // TODO
    return false;
}

unsigned int IEEE8021Queue::getCapacity() {
    // TODO
    return 0;
}

unsigned int IEEE8021Queue::getResidualCapacity() {
    // TODO
    return 0;
}

}  // namespace faker_tsn
