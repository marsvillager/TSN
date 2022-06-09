#include "QueueContext.h"

namespace faker_tsn {

QueueContext::QueueContext(unsigned short portIndex)
    : m_portIndex(portIndex) {
    /* initialize queues */
    ConfigSetting& cs = ConfigSetting::getInstance();
    this->m_deviceName = cs.get<std::string>("deviceName");
    int queueNum = std::atoi(cs.get<const char*>("switch.queue.number"));
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queues.number = " + std::to_string(queueNum));
    std::string queueClass = cs.get<std::string>("switch.queue.class");
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queues.class = " + queueClass);
    for (int pcp = 0; pcp < queueNum; pcp++) {
        std::shared_ptr<IQueue> queue(dynamic_cast<IQueue*>(REFLECTOR::CreateByTypeName("faker_tsn::" + queueClass, (unsigned short)this->m_portIndex, static_cast<uint8_t>(pcp))));
        this->m_queues.push_back(std::move(queue));
    }
}

QueueContext::~QueueContext() {}

/* enqueue */
void QueueContext::enqueue(IFrameBody* frame) {
    INFO("Enqueue");
    // // TODO
    // this->m_tempQueue.push_back(frame);

    if (frame->getType() == IEEE_802_1Q_TSN_FRAME || frame->getType() == IEEE_802_1Q_TSN_FRAME_E) {  // TSN frame or enhanced TSN frame
        TSNFrameBody* tsnFrame = dynamic_cast<TSNFrameBody*>(frame);
        uint8_t pcp = static_cast<uint8_t>(tsnFrame->getPCP());
        INFO("frame size: " + std::to_string(tsnFrame->getBytes()));
        this->m_queues[pcp]->enqueue(tsnFrame);
    } else if (frame->getType() == IEEE_802_1Q_FRAME) {  // IEEE 802.1Q frame
        // TODO
    }
}

/* dequeue */
IFrameBody* QueueContext::dequeue() {
    INFO("Dequeue");
    // TODO
    // if (this->m_tempQueue.size() == 0) {
    //     INFO("Empty queue");
    //     return nullptr;
    // }
    // IFrameBody* frame = this->m_tempQueue.front();
    // this->m_tempQueue.pop_front();
    // return frame;

    this->m_transmissionSelection.dequeue(*this);
}

}  // namespace faker_tsn
