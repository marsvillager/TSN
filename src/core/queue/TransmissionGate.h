#ifndef _TRANSMISSON_GATE_H
#define _TRANSMISSON_GATE_H

#include <cstdint>
#include <memory>

#include "../../networking/frame/IFrameBody.h"
#include "GateControlList.h"
#include "TransmissionSelectionAlgorithm.h"

namespace faker_tsn {

class TransmissionGate {
  private:
    uint8_t m_pcp;
    bool m_isOpen;
    std::shared_ptr<TransmissionSelectionAlgorithm> m_algorithm;

  public:
    TransmissionGate(std::shared_ptr<TransmissionSelectionAlgorithm> algorithm)
        : m_isOpen(false) {
        this->m_algorithm = algorithm;
        this->m_pcp = algorithm->getPCP();
    }

    ~TransmissionGate() = default;

    inline void setOpen(bool state) {
        this->m_isOpen = state;
    }

    /* dequeue head frame */
    IFrameBody* dequeue();

    /* Observer pattern */

    /* register into GateControlList */
    void registerGCL();

    /* callable funtion */
    void onUpdate();
};

}  // namespace faker_tsn

#endif  // _TRANSMISSON_GATE_H