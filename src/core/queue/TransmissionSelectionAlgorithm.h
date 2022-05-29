#ifndef _TRANSMISSION_SELECTION_ALGORITHM_H
#define _TRANSMISSION_SELECTION_ALGORITHM_H

#include <cstdint>
#include <memory>

#include "../../networking/frame/IFrameBody.h"
#include "InnerBuffer.h"

namespace faker_tsn {

class TransmissionSelectionAlgorithm {
  private:
    uint8_t m_pcp;
    std::shared_ptr<InnerBuffer> m_innerBuffer;

  public:
    TransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer);
    ~TransmissionSelectionAlgorithm();

    inline uint8_t getPCP() {
        return this->m_pcp;
    }

    IFrameBody* dequeue();
};

}  // namespace faker_tsn

#endif  // _TRANSMISSION_SELECTION_ALGORITHM_H