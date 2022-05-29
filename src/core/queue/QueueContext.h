#ifndef _QUEUE_CONTEXT_H
#define _QUEUE_CONTEXT_H

#include <deque>
#include <memory>
#include <vector>

#include "../../networking/frame/EnhancementTSNFrameBody.h"
#include "../../networking/frame/IFrameBody.h"
#include "../../networking/frame/TSNFrameBody.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/log/Log.h"
#include "IEEE8021Queue.h"
#include "IQueue.h"
#include "TransmissionSelection.h"

namespace faker_tsn {

class QueueContext {
  private:
    std::deque<IFrameBody*> m_tempQueue;  // temporary queue

    std::string m_deviceName;
    unsigned short m_portIndex;
    std::vector<std::shared_ptr<IQueue>> m_queues;  // queues
    TransmissionSelection m_transmissionSelection;  // transmission selection function

  public:
    friend class TransmissionSelection;

    QueueContext(unsigned short portIndex);

    ~QueueContext();

    /* enqueue */
    void enqueue(IFrameBody* frameBody);

    /* dequeue */
    IFrameBody* dequeue();
};

}  // namespace faker_tsn

#endif  // _QUEUE_CONTEXT_H