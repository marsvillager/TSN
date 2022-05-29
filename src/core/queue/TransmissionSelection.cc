#include "TransmissionSelection.h"

#include "QueueContext.h"

namespace faker_tsn {

IFrameBody* TransmissionSelection::dequeue(QueueContext& queueContext) {
    INFO(queueContext.m_deviceName + ".port" + std::to_string(queueContext.m_portIndex) + " TransmissionSelection on call");
    /* invoke queue from 7 to 0 */
    for (auto it = queueContext.m_queues.rbegin(); it != queueContext.m_queues.rend(); it++) {
        IFrameBody* frame = (*it)->dequeue();
        if (frame != nullptr)
            return frame;
    }
    return nullptr;
}

}  // namespace faker_tsn
