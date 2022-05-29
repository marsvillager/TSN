#ifndef _I_FRAME_BODY_H
#define _I_FRAME_BODY_H

#include <linux/if_packet.h>

#include "RelayEntity.h"

namespace faker_tsn {

class IFrameBody {
  private:
    RELAY_ENTITY m_type;

  public:
    virtual ~IFrameBody() = default;

    virtual unsigned int getBytes() = 0;

    void setType(RELAY_ENTITY type) {
        this->m_type = type;
    }

    RELAY_ENTITY getType() {
        return this->m_type;
    }
};

}  // namespace faker_tsn

#endif  // _I_FRAME_BODY_H