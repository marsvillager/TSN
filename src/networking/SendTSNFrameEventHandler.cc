#include "SendTSNFrameEventHandler.h"
#include <faker_tsn.h>

namespace faker_tsn {

SendTSNFrameEventHandler::SendTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII, std::shared_ptr<QueueContext> queueContext):
     m_handle(handle),  m_queueContext(queueContext) {
    memcpy(&this->m_sockAddrII, &sockAddrII, sizeof(sockAddrII));
}

SendTSNFrameEventHandler::~SendTSNFrameEventHandler() {
}

HANDLE SendTSNFrameEventHandler::getHandle() {
    return this->m_handle;
}

void SendTSNFrameEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::WRITE);

    INFO("SendTSNFrameEventHandler on call");

    /* get frame from queue */
    TSNFrameBody* frameBody = (TSNFrameBody*)this->m_queueContext->dequeue();
    if (frameBody == nullptr) {
        // disable EPOLLOUT event
        Reactor::getInstance().getDemultiplexer().updateHandle(this->m_handle, 0x00);
        INFO("Nothing to be sent\n");
        return;
    }

    /* construct ethernet header */
    struct ethhdr eth_hdr;
    memset(&eth_hdr, 0x00, sizeof(eth_hdr));
    unsigned char dest[ETH_ALEN] = {0x01, 0x00, 0x5E, 0x00, 0x00, 0x01};
    memcpy(&eth_hdr.h_dest, frameBody->getDstMAC(), ETH_ALEN);         // set dest mac
    // memcpy(&eth_hdr.h_source, this->m_sockAddrII.sll_addr, ETH_ALEN);  // set src mac
    memcpy(&eth_hdr.h_source, frameBody->getSrcMAC(), ETH_ALEN);       // set src mac
    memcpy(&eth_hdr.h_proto, frameBody->getProto(), ETH_ALEN);         // set protocol
    INFO("dest mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_dest), 6));
    INFO("src mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_source), 6));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2));

    /* construct VLAN-tag */
    VlanTCI vlan_tci;
    vlan_tci.pcp = frameBody->getPCP();
    vlan_tci.vid = frameBody->getVID();
    __be16 tci = VlanTCI::encode(vlan_tci);
    struct vlan_hdr vlan_tag;
    memset(&vlan_tag, 0x00, sizeof(vlan_tag));
    memcpy(&vlan_tag.h_vlan_TCI, &tci, sizeof(tci));                                  // set TCI
    memcpy(&vlan_tag.h_vlan_encapsulated_proto, frameBody->getVlanProto(), ETH_ALEN); // set protocol

    /* construct R-tag */
    struct rtag_hdr rtag;
    __be16 seq = frameBody->getSeq();
    memset(&rtag, 0x00, sizeof(rtag));
    rtag.h_rtag_seq_num = htons(seq);  
    rtag.h_rtag_encapsulated_proto = htons(ETH_P_IP);

    /* construct TSN frame */
    union tsn_frame frame;
    const char* data = "hello world\n";
    unsigned int data_len = strlen(data);
    unsigned int frame_len = data_len + ETH_HLEN + 4 + 6;
    memset(&frame, 0x00, sizeof(tsn_frame));
    memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));
    memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));
    memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));
    memcpy(frame.filed.data, data, strlen(data));

    /* get interface index */
    int ifindex = LinkLayerInterface::getIndex("lo");

    this->m_sockAddrII.sll_family = PF_PACKET;           // set address family
    this->m_sockAddrII.sll_ifindex = ifindex;            // set interface index
    this->m_sockAddrII.sll_halen = ETH_ALEN;             // set address length
    this->m_sockAddrII.sll_protocol = htons(ETH_P_ALL);  // set protocol

    /* send data */
    int fd = this->m_handle;
    if (sendto(fd, frame.buffer, frame_len, 0, (struct sockaddr*)&this->m_sockAddrII, sizeof(this->m_sockAddrII)) > 0)
        INFO("Send success!\n");
    else
        INFO("Send error!\n");
}

}  // namespace faker_tsn
