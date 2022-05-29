#ifndef _TSN_FRAME_BODY_H
#define _TSN_FRAME_BODY_H

#include <memory.h>

#include "../../utils/log/Log.h"
#include "if_ether.h"
#include "IFrameBody.h"

namespace faker_tsn {

class TSNFrameBody : public IFrameBody {
  private:
    unsigned short m_pcp;               /* priority code point */
    unsigned short m_vid;               /* vlan id */
    unsigned short m_seq;               /* sequencr nubmer */
    unsigned int m_bytes;               /* no. of bytes */
    unsigned char m_data[ETH_DATA_LEN]; /* data */

  public:
    TSNFrameBody() = default;

    virtual ~TSNFrameBody() { INFO("Destruct TSNFrameBody"); };

    void setPCP(unsigned short pcp) {
        this->m_pcp = pcp;
    }

    void setVID(unsigned short vid) {
        this->m_vid = vid;
    }

    void setSeq(unsigned short seq) {
        this->m_seq = seq;
    }

    void setData(unsigned char* data, unsigned int len) {
        memcpy(this->m_data, data, len);
        this->m_bytes = len;
    }

    unsigned short getPCP() {
        return this->m_pcp;
    }

    unsigned short getVID() {
        return this->m_vid;
    }

    unsigned short getSeq() {
        return this->m_seq;
    }

    virtual unsigned int getBytes() override {
        return this->m_bytes;
    }
};

}  // namespace faker_tsn

#endif