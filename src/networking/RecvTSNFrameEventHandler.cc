#include "RecvTSNFrameEventHandler.h"

namespace faker_tsn {

RecvTSNFrameEventHandler::RecvTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII): m_handle(handle) {
    memcpy(&this->m_sockAddrII, &sockAddrII, sizeof(sockAddrII));
    this->m_isEnhanced = ConfigSetting::getInstance().get<bool>("enhancedGCL");
}

RecvTSNFrameEventHandler::~RecvTSNFrameEventHandler() {
}

void RecvTSNFrameEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::READ);

    INFO("RecvTSNFrameEventHandler on call");

    /* receive raw data */
    unsigned char recvbuf[ETH_FRAME_LEN + 4 + 6];
    int fd = this->m_handle;
    if (recv(fd, recvbuf, ETH_FRAME_LEN + 4 + 6, 0) > 0) {
        INFO("Recv success!");
    } else {
        INFO("Recv error!");
    }

    // filter
    unsigned short proto = htons(0x8100);
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>((recvbuf + 12)), 2));
    if (memcmp(recvbuf + 12, reinterpret_cast<unsigned char*>(&proto), 2) != 0) {
        INFO("------------- Non-TSN frame --------------\n");
        return;
    } else {
        INFO("------------- TSN frame  --------------\n");
    }

    // INFO("Decode frame");

    /* parse raw data to tsn frame */
    TSNFrameBody* frame = new TSNFrameBody();
    __be16 tci;
    memcpy(&tci, recvbuf + 14, 2);
    VlanTCI vlan_tci = VlanTCI::parse(tci);
    frame->setPCP(vlan_tci.pcp);
    frame->setVID(vlan_tci.vid);
    __be16 seq;
    memcpy(&seq, recvbuf + 20, 2);
    frame->setSeq(ntohs(seq));
    frame->setData(recvbuf + 24, strlen((char*)recvbuf + 24));

    /* forward frame */
    unsigned char srcMac[ETH_ALEN];
    memcpy(srcMac, recvbuf + 6, ETH_ALEN);
    RELAY_ENTITY type = IEEE_802_1Q_TSN_FRAME;
    if (this->m_isEnhanced) type = IEEE_802_1Q_TSN_FRAME_E;
    ForwardFunction::forward(srcMac, reinterpret_cast<void*>(frame), sizeof(frame), type);
}

HANDLE RecvTSNFrameEventHandler::getHandle() {
    return this->m_handle;
}

}  // namespace faker_tsn
