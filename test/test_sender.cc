#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../src/networking/frame/if_ether.h"
#include "../src/utils/ConvertUtils.h"
#include "../src/utils/log/Log.h"

using namespace faker_tsn;
using namespace std;

static void TestSend() {
    union ethframe {
        struct
        {
            struct ethhdr header;
            unsigned char data[ETH_DATA_LEN];
        } field;
        unsigned char buffer[ETH_FRAME_LEN];
    };  // ethframe

    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    const char* deviceName = cs.get<const char*>("deviceName");

    /* create raw socket */
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_8021Q));  // create socket

    /* get interface index */
    int ifindex = LinkLayerInterface::getIndex(deviceName);

    /* get interface mac address */
    unsigned char mac[8];
    LinkLayerInterface::getMacAddress(deviceName)->getRaw(mac);

    int frameNum = 400;
    // __be16 TCI[8] = {
    __be16 TCI[4] = {
        htons(0x0001),  // 0：0000 0000 0000 0001
        // htons(0x2001),    // 1：0010 0000 0000 0001
        // htons(0x4001),    // 2：0100 0000 0000 0001
        // htons(0x6001),    // 3：0011 0000 0000 0001
        htons(0x8001),  // 4：1000 0000 0000 0001
        htons(0xA001),  // 5：1010 0000 0000 0001
        // htons(0xC001),    // 6：1100 0000 0000 0001
        htons(0xE001)  // 7：1110 0000 0000 0001
    };
    for (int i = 0; i < frameNum; i++) {
        // INFO("\n\nEncode frame");
        /* construct ethernet header */
        struct ethhdr eth_hdr;
        memset(&eth_hdr, 0x00, sizeof(eth_hdr));
        unsigned char dest[ETH_ALEN] = {0x11, 0x00, 0x5E, 0x00, 0x00, 0x01};
        // unsigned char dest[ETH_ALEN] = {0x00, 0x0C, 0x29, 0xED, 0x5A, 0xAD};
        // unsigned char dest[ETH_ALEN] = {0x64, 0x6E, 0x97, 0xC7, 0x88, 0x65};
        memcpy(&eth_hdr.h_dest, dest, ETH_ALEN);   // set dest mac
        memcpy(&eth_hdr.h_source, mac, ETH_ALEN);  // set src mac
        // don't set ETH_P_8021Q, it will ceause receiver to discard vlan tag
        eth_hdr.h_proto = htons(0x6666);  // set IEEE 802.1Q protocol
        INFO("dest mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_dest), 6));
        INFO("src mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_source), 6));
        INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2) + "\n");

        // __be16 tci = TCI[i % 8]; // 一轮优先级测试
        __be16 tci = TCI[i % 4];  // 一轮优先级测试
        struct vlan_hdr vlan_tag;
        memset(&vlan_tag, 0x00, sizeof(vlan_tag));
        memcpy(&vlan_tag.h_vlan_TCI, &tci, sizeof(tci));        // set TCI
        vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_ALL);
        // INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
        // INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2) + "\n");

        /* construct R-tag */
        struct rtag_hdr rtag;
        memset(&rtag, 0x00, sizeof(rtag));
        rtag.h_rtag_seq_num = htons(i);
        rtag.h_rtag_encapsulated_proto = htons(ETH_P_IP);
        // INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
        // INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
        // INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2) + "\n");

        /* construct TSN frame */
        union tsn_frame frame;
        const char* data = "hello world\n";
        unsigned int frame_len = strlen(data) + ETH_HLEN + 4 + 6;
        memset(&frame, 0x00, sizeof(tsn_frame));
        memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));
        memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));
        memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));
        memcpy(frame.filed.data, data, strlen(data));

        /* set linklayer address */
        struct sockaddr_ll saddrll;  // link layer socker address struct
        memset((void*)&saddrll, 0x00, sizeof(saddrll));
        saddrll.sll_family = PF_PACKET;           // set address family
        saddrll.sll_ifindex = ifindex;            // set interface index
        saddrll.sll_halen = ETH_ALEN;             // set address length
        saddrll.sll_protocol = htons(ETH_P_ALL);  // set protocol

        /* send data */
        if (sendto(sockfd, frame.buffer, frame_len, 0, (struct sockaddr*)&saddrll, sizeof(saddrll)) > 0)
            INFO("Send success!");
        else
            INFO("Error, could not send");
    }

    close(sockfd);
}

TEST(TEST_SENDER, TEST_SENDER) {
    TestSend();
}