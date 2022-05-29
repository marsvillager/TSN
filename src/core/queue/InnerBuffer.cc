#include "InnerBuffer.h"

namespace faker_tsn {

InnerBuffer::InnerBuffer(unsigned short portIndex, uint8_t pcp, uint32_t capacity)
    : m_portIndex(portIndex), m_pcp(pcp), m_capacity(capacity), m_residualCapacity(capacity), m_usedCapacity(0) {
    this->m_deviceName = ConfigSetting::getInstance().get<std::string>("deviceName");

    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity: " + std::to_string(this->m_capacity));
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(used): " + std::to_string(this->m_usedCapacity));
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(residual): " + std::to_string(this->m_residualCapacity));
}

InnerBuffer::~InnerBuffer() {
}

void InnerBuffer::drop(IFrameBody* frameBody) {
    std::cout << std::endl
              << "Drop frame\n\n";
    // INFO("Drop frame\n");
    // delete frameBody;
}

void InnerBuffer::enqueue(IFrameBody* frameBody) {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer enqueue");
    /* drop a frame if queue is full */
    if (frameBody->getBytes() > this->m_residualCapacity) {
        INFO("Frame size ( " + std::to_string(frameBody->getBytes()) + "bytes) out of residual capacity (" + std::to_string(this->m_residualCapacity) + ")");
        drop(frameBody);
        return;
    }
    this->m_residualCapacity -= frameBody->getBytes();
    this->m_usedCapacity += frameBody->getBytes();
    this->m_buffer.push_back(frameBody);

    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(used): " + std::to_string(this->m_usedCapacity));
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(residual): " + std::to_string(this->m_residualCapacity));
}

IFrameBody* InnerBuffer::dequeue() {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer dequeue");
    if (this->m_buffer.size() == 0)
        return nullptr;
    IFrameBody* frameBody = this->m_buffer.front();
    this->m_buffer.pop_front();
    this->m_residualCapacity += frameBody->getBytes();
    this->m_usedCapacity -= frameBody->getBytes();

    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(used): " + std::to_string(this->m_usedCapacity));
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer.capacity(residual): " + std::to_string(this->m_residualCapacity));

    return frameBody;
}

IFrameBody* InnerBuffer::dequeue(uint32_t index) {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + ".innerBuffer dequeue");
    if (this->m_buffer.size() == 0)
        return nullptr;
    IFrameBody* frameBody = this->getBy(index);
    this->removeBy(index);
    this->m_residualCapacity += frameBody->getBytes();
    this->m_usedCapacity -= frameBody->getBytes();
    return frameBody;
}

IFrameBody* InnerBuffer::getBy(uint32_t index) {
    if (this->m_buffer.size() == 0)
        return nullptr;
    IFrameBody* frameBody = this->m_buffer.at(index);
    return frameBody;
}

void InnerBuffer::removeBy(uint32_t index) {
    if (this->m_buffer.size() == 0)
        return;
    this->m_buffer.erase(this->m_buffer.begin() + index);
}

}  // namespace faker_tsn
