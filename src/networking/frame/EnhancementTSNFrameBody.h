#ifndef _ENHANCEMENT_TSN_FRAME_BODY_H
#define _ENHANCEMENT_TSN_FRAME_BODY_H

#include "TSNFrameBody.h"

namespace faker_tsn {

class EnhancementTSNFrameBody : public TSNFrameBody {
  private:
    unsigned long m_flowId; /* flow id */
    unsigned int m_phase;   /* phase */

  public:
    EnhancementTSNFrameBody() {}

    virtual ~EnhancementTSNFrameBody() {}

    inline unsigned long getFlowId() {
        return this->m_flowId;
    }

    inline unsigned int getPhase() {
        return this->m_phase;
    }
};

}  // namespace faker_tsn

#endif  // _ENHANCEMENT_TSN_FRAME_BODY_H