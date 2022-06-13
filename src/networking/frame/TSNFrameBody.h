#ifndef _TSN_FRAME_BODY_H
#define _TSN_FRAME_BODY_H

#include <memory.h>

#include "../../utils/ConvertUtils.h"
#include "../../utils/log/Log.h"
#include "if_ether.h"
#include "IFrameBody.h"

namespace faker_tsn {

class TSNFrameBody : public IFrameBody {
  private:
    unsigned short m_pcp;                 /* priority code point */
    unsigned short m_vid;                 /* vlan id */
    unsigned short m_seq;                 /* sequence nubmer */
    unsigned int m_bytes;                 /* no. of bytes */
    unsigned char m_data[ETH_DATA_LEN];   /* data */
    unsigned char m_dst[ETH_ALEN];        /* dst mac */
    unsigned char m_src[ETH_ALEN];        /* src mac */
    unsigned char m_proto[ETH_TLEN];      /* proto */
    unsigned char m_vlan_proto[ETH_TLEN]; /* vlan proto */
    unsigned char m_rtag[ETH_ALEN]; /* vlan proto */

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

    void setFrame(unsigned char* frame) {
        memcpy(this->m_dst, frame, 6);
        memcpy(this->m_src, frame + 6, 6);
        memcpy(this->m_proto, frame + 12, 2);
        memcpy(this->m_vlan_proto, frame + 16, 2);
        memcpy(this->m_rtag, frame + 18, 6);
        memcpy(this->m_data, frame + 24, strlen((char*)frame + 24));

        this->m_bytes = strlen((char*)frame + 24);
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

    unsigned char* getDstMAC() {
        return this->m_dst;
    }

    unsigned char* getSrcMAC() {
        return this->m_src;
    }

    unsigned char* getProto() {
        return this->m_proto;
    }

    unsigned char* getVlanProto() {
        return this->m_vlan_proto;
    }

    unsigned char* getRtag() {
        return this->m_rtag;
    }

    unsigned char* getData() {
        return this->m_data;
    }
};

}  // namespace faker_tsn

#endif