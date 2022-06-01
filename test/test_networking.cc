#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <string>

#include "../src/networking/frame/VlanTCI.h"
#include "../src/networking/frame/if_ether.h"
#include "../src/utils/ConvertUtils.h"

using namespace faker_tsn;
using namespace std;

void port_status(unsigned int flags) {
    if (flags & IFF_UP) { /* Interface is up. */
        printf("is up\n");
    }
    if (flags & IFF_BROADCAST) { /* Broadcast address valid. */
        printf("is broadcast\n");
    }
    if (flags & IFF_LOOPBACK) { /* Is a loopback net. */
        printf("is loop back\n");
    }
    if (flags & IFF_POINTOPOINT) { /* Interface is point-to-point link. */
        printf("is point to point\n");
    }
    if (flags & IFF_RUNNING) {
        printf("is running\n");
    }
    if (flags & IFF_PROMISC) {
        printf("is promisc\n");
    }
}

static void TestGetMulticastMacAddress() {
    printf("测试 1: TestGetMulticastMacAddress \n");
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    // Interface request
    struct ifreq buffer;
    int ifindex;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, "ens33", IFNAMSIZ);  // ifr_name

    // 1、socket io control get interface hardware address
    if (ioctl(sockfd, SIOCGIFHWADDR, &buffer) < 0) { /* Get hardware address */
        printf("Error: could not get MAC address\n");
    }

    unsigned char macaddr[ETH_ALEN] = {0};
    memcpy(macaddr, buffer.ifr_hwaddr.sa_data, ETH_ALEN);
    printf("MAC address");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", macaddr[i]);

    // 2、socket io control get interface address
    if (ioctl(sockfd, SIOCGIFADDR, &buffer) < 0) { /* get PA addres */
        printf("Error: could not get IP address\n");
    }
    cerr << "\nIP address: " << inet_ntoa(((struct sockaddr_in*)&(buffer.ifr_ifru.ifru_addr))->sin_addr) << '\n';

    struct ifconf ifc;
    struct ifreq buf[32];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    // 3、socket io control get interface config
    int ret = ioctl(sockfd, SIOCGIFCONF, &ifc);
    if (ret < 0) {
        printf("get if config info failed\n");
    }

    int num = ifc.ifc_len / sizeof(struct ifreq);
    printf("interface num is %d\n\n", num);
    while (num-- > 0) {
        printf("net device: %s\n", buf[num].ifr_name);

        ret = ioctl(sockfd, SIOCGIFFLAGS, (char*)&buf[num]);
        if (ret)
            continue;

        INFO("interface index is: " + to_string(buf[num].ifr_ifindex));
        // printf("interface index is: %d\n", buf[num].ifr_ifru.ifru_ivalue);

        printf("flag is: %d\n", buf[num].ifr_flags);
        port_status(buf[num].ifr_flags);

        ret = ioctl(sockfd, SIOCGIFADDR, (char*)&buf[num]);
        if (ret)
            continue;
        printf("IP address is: %s\n", inet_ntoa(((struct sockaddr_in*)(&buf[num].ifr_addr))->sin_addr));

        ret = ioctl(sockfd, SIOCGIFHWADDR, (char*)&buf[num]);
        if (ret)
            continue;
        printf("MAC address is: %02x:%02x:%02x:%02x:%02x:%02x\n\n",
               (unsigned char)buf[num].ifr_hwaddr.sa_data[0],
               (unsigned char)buf[num].ifr_hwaddr.sa_data[1],
               (unsigned char)buf[num].ifr_hwaddr.sa_data[2],
               (unsigned char)buf[num].ifr_hwaddr.sa_data[3],
               (unsigned char)buf[num].ifr_hwaddr.sa_data[4],
               (unsigned char)buf[num].ifr_hwaddr.sa_data[5]);
    }

    // 4、socket io control get interface index
    sockfd = socket(PF_PACKET, SOCK_RAW, 0);
    if (ioctl(sockfd, SIOCGIFINDEX, &buffer) < 0) {  // used ioctl to find interface index
        ERROR("Could not get interface index");
        throw IndexNotFound();
    }

    INFO("interface index is: " + to_string(buffer.ifr_ifindex));
    printf("net device: %s\n", buffer.ifr_ifrn.ifrn_name);

    cerr << "IP address: " << inet_ntoa(((struct sockaddr_in*)&(buffer.ifr_ifru.ifru_addr))->sin_addr) << '\n';

    memcpy(macaddr, buffer.ifr_hwaddr.sa_data, ETH_ALEN);
    printf("MAC address");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", macaddr[i]);
}

static void TestGetMacAddress() {
    try {
        MacAddress* macAddress = LinkLayerInterface::getMacAddress("ens33");

        unsigned char rawMac[ETH_ALEN];
        macAddress->getRaw(rawMac);
        printf("raw MAC address is: ");
        for (int i = 0; i < ETH_ALEN; i++) {
            printf("%d ", rawMac[i]);
        }
    } catch (const exception& e) {
        cerr << e.what() << '\n';
    }
}

static void TestGetIndex() {
    try {
        int index = LinkLayerInterface::getIndex("ens33");
    } catch (const exception& e) {
        cerr << e.what() << '\n';
    }
}

static void TestFindInterface() {
    try {
        LinkLayerInterface* interface = LinkLayerInterface::findInterface("ens33");
        INFO(interface->toString());
    } catch (const exception& e) {
        cerr << e.what() << '\n';
    }
}

static void TestIfEther() {
    const char* deviceName = "ens33";

    /* get interface index */
    int ifindex = LinkLayerInterface::getIndex(deviceName);
    printf("\n");

    /* get interface mac address */
    unsigned char src[8];
    LinkLayerInterface::getMacAddress(deviceName)->getRaw(src);
    printf("\n");

    // test Ethernet header
    struct ethhdr eth_hdr;
    INFO("Ethernet header length = " + to_string(sizeof(eth_hdr)));
    memset(&eth_hdr, 0x00, sizeof(eth_hdr));
    memcpy(&eth_hdr.h_source, src, ETH_ALEN);  // set src mac
    eth_hdr.h_proto = htons(ETH_P_8021Q);      // set IEEE 802.1Q protocol
    printf("src MAC address");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", eth_hdr.h_source[i]);
    printf("\ndst MAC address");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", eth_hdr.h_dest[i]);
    INFO("\nprotocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2) + "\n");

    struct vlan_tci tci;
    INFO("TCI length = " + to_string(sizeof(tci)));
    memset(&tci, 0x00, sizeof(tci));
    tci.pcp = 1;
    tci.vid = 1;
    INFO("pcp = " + to_string(tci.pcp));  // 001
    INFO("dei = " + to_string(tci.dei));  // 0
    INFO("vid = " + to_string(tci.vid));  // 000000000001
    // N    0x2001 : 0010 0000 , 0000 0001
    // H    0x0120 : 0000 0001 , 0010 0000
    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&tci), 2) + "\n");

    __be16 _tci = htons(0xE001);
    INFO("n_TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&_tci), 2));
    __be16 __tci = 0xE001;
    INFO("h_TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&__tci), 2) + "\n");

    VlanTCI vlanTCI = VlanTCI::parse(_tci);
    INFO("VlanTCI.pcp = " + to_string(vlanTCI.pcp));
    INFO("VlanTCI.dei = " + to_string(vlanTCI.dei));
    INFO("VlanTCI.vid = " + to_string(vlanTCI.vid));

    // test VLAN tag
    struct vlan_hdr vlan_tag;
    INFO("VLAN tag length = " + to_string(sizeof(vlan_tag)));
    memset(&vlan_tag, 0x00, sizeof(vlan_tag));
    memcpy(&vlan_tag.h_vlan_TCI, &_tci, sizeof(_tci));        // set TCI
    vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_8021Q);  // set IEEE 1722 protocol
    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2) + "\n");

    // test R-tag
    struct rtag_hdr rtag;
    INFO("R-tag length = " + to_string(sizeof(rtag)));
    memset(&rtag, 0x00, sizeof(rtag));
    rtag.h_rtag_seq_num = htons(1);  // set
    rtag.h_rtag_encapsulated_proto = htons(ETH_P_ALL);
    INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
    INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2) + "\n");

    // construct TSN frame
    union tsn_frame frame;
    char* data = "hello world\n";
    INFO("raw data = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(data), 13));
    memset(&frame, 0x00, sizeof(tsn_frame));
    INFO("TSN frame length = " + to_string(sizeof(frame)));
    memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));     // eth_hdr
    memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));  // vlan tag
    memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));             // r_tag
    memcpy(frame.filed.data, data, strlen(data));                       // data
    // eth_hdr
    printf("src MAC address");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", frame.filed.header.eth_hdr.h_source[i]);
    printf("\ndst MAC address：");
    for (int i = 0; i < ETH_ALEN; i++)
        printf("%02x ", frame.filed.header.eth_hdr.h_dest[i]);
    INFO("\nprotocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.eth_hdr.h_proto), 2));
    // vlan_tag
    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.vlan_tag.h_vlan_TCI), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.vlan_tag.h_vlan_encapsulated_proto), 2));
    // rtag
    INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.r_tag.h_rtag_rsved), 2));
    INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.r_tag.h_rtag_seq_num), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&frame.filed.header.r_tag.h_rtag_encapsulated_proto), 2));
    // // data
    INFO("data = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(frame.filed.data), 13));
}

static void TestMacTable() {
    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    string filename = cs.get<string>("routesDir");
    MacTable::loadRouteXML(filename);
    INFO("\n" + MacTable::toString());
}

TEST(TEST_NETWORKING, TEST_NETWORKING_INTERFACE) {
    // TestGetMulticastMacAddress();  // 通过套接字获取所有网卡设备的 MAC 地址
    // TestGetMacAddress();           // 通过抓包库 libpcap 获取指定网卡设备的 MAC 地址（LinkLayerInterface 类中定义的方法）
    // TestGetIndex();                // 通过套接字获取指定网卡设备的接口索引（LinkLayerInterface 类中定义的方法）
    // TestFindInterface();           // 通过套接字和抓包库 libpcap 实现 LinkLayerInterface 类对象的定义（name、index、mac）
    // TestIfEther();                 // 上面四种的综合
}

TEST(TEST_NETWORKING, TEST_MAC_TABLE) {
    TestMacTable();
}