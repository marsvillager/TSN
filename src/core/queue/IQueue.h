#ifndef _I_QUEUE_H
#define _I_QUEUE_H

#include "../../networking/frame/IFrameBody.h"

namespace faker_tsn {

class IQueue {
  public:
    virtual ~IQueue() = default;

    virtual std::string toString() {
        return "IQueue";
    }

    /* enqueue frame body */
    virtual void enqueue(IFrameBody* frameBody) = 0;

    /* get frame body from queue */
    virtual IFrameBody* dequeue() = 0;

    /* check whether queue is emptry or not */
    virtual bool isEmpty() = 0;

    /* get capacity of queue */
    virtual unsigned int getCapacity() = 0;

    /* get residual capacity of queue */
    virtual unsigned int getResidualCapacity() = 0;
};

}  // namespace faker_tsn

#endif  //  _I_QUEUE_H