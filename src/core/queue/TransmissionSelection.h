#ifndef _TRANSMISSION_SELECTION_H
#define _TRANSMISSION_SELECTION_H

#include <memory>
#include <vector>

#include "../../networking/frame/IFrameBody.h"
#include "IQueue.h"

namespace faker_tsn {

class QueueContext;

/* friend class of QueueContext */
class TransmissionSelection {
  public:
    ~TransmissionSelection() = default;

    IFrameBody* dequeue(QueueContext& queueContext);
};

}  // namespace faker_tsn

#endif  // _TRANSMISSION_SELECTION_H