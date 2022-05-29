#ifndef _IEEE_8021_QUEUE_H
#define _IEEE_8021_QUEUE_H

#include <memory>

#include "../../utils/log/Log.h"
#include "../../utils/reflector/DynamicCreate.h"
#include "IQueue.h"
#include "InnerBuffer.h"
#include "TransmissionSelectionAlgorithm.h"

namespace faker_tsn {

class IEEE8021Queue : public IQueue, public REFLECT_OBJECT, public DynamicCreator<IEEE8021Queue, unsigned short, uint8_t> {
  private:
    std::string m_deviceName;
    unsigned short m_portIndex;
    uint8_t m_pcp;
    std::shared_ptr<InnerBuffer>
        m_innerBuffer; /* inner buffer */
    std::shared_ptr<TransmissionSelectionAlgorithm>
        m_transmissionSelectionAlgorithm; /* transmission selection algorithm */

  public:
    IEEE8021Queue(unsigned short portIndex, uint8_t pcp);

    virtual ~IEEE8021Queue() override;

    virtual std::string toString() override {
        return "IEEE8021Queue";
    }

    /* enqueue frame body */
    virtual void enqueue(IFrameBody* frameBody) override;

    /* get frame body from queue */
    virtual IFrameBody* dequeue() override;

    /* check whether queue is emptry or not */
    virtual bool isEmpty() override;

    /* get capacity of queue */
    virtual unsigned int getCapacity() override;

    /* get residual capacity of queue */
    virtual unsigned int getResidualCapacity() override;
};

}  // namespace faker_tsn

#endif  // _IEEE_8021_QUEUE_H