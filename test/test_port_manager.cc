#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <pcap/pcap.h>
#include <algorithm>
#include <iterator>

using namespace faker_tsn;
using namespace std;

static void TestPortManager() {
    PortManager portManager;
    portManager.findAllDeviceName();
    auto deviceNames = portManager.getAllDeviceName();
    portManager.createPortFromDeviceNameList();
    auto port = portManager.getPort(0);
    INFO(to_string(port->getDeviceID()));
    INFO(port->getDeviceName());
    // cout << endl;
    // port->sendTest();
}

static void TestPort() {
    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    const char* deviceName = cs.get<const char*>("deviceName");
    shared_ptr<IPort> port = make_shared<DataPort>(deviceName);
    shared_ptr<IPortState> creationState = make_shared<CreationPortState>();
    creationState->doAction(port);
    port->sendTest();
}

void my_callback(u_char* useless, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    static int count = 1;
    printf("Packet %d:\n", count);
    printf("    useless: %x\n", useless);
    printf("    pkthdr: %x\n", pkthdr);
    printf("    pkthdr->ts: %d.%d\n", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec);
    printf("    pkthdr->caplen: %d\n", pkthdr->caplen);
    printf("    pkthdr->len: %d\n", pkthdr->len);
    printf("    packet: %x\n", packet);

    if (count >= 3) {
        pcap_breakloop((pcap_t*)useless);
    }
    count += 1;
}

static void TestPCAP() {
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }
    INFO("Device: " + string(dev));

    /* open the session on non-promiscouos mode */
    pcap_t* handle;
    handle = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Handle: %x\n", (u_char*)handle);

    /* find the properties for the device */
    bpf_u_int32 mask; /* Our netmask */
    bpf_u_int32 net;  /* Our IP */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* compile and apply the filter */
    struct bpf_program fp;                  /* The compiled filter */
    char filter_exp[] = "portrange 0-1023"; /* The filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* loop */
    pcap_loop(handle, 5, my_callback, (u_char*)handle);
    // pcap_dispatch(handle, 5, my_callback, (u_char*)handle);

    // struct pcap_pkthdr header; /* The header that pcap gives us */
    // const u_char* packet;      /* The actual packet */
    // packet = pcap_next(handle, &header);
    // printf("Jacked a packet with length of [%d]\n", header.len);

    fprintf(stdout, "\nDone processing packets... wheew!\n");

    pcap_close(handle);
}

static void TestSendRecv() {
    union ethframe {
        struct
        {
            struct ethhdr header;
            unsigned char data[ETH_DATA_LEN];
        } field;
        unsigned char buffer[ETH_FRAME_LEN];
    };

    /* create raw socket */
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_TSN));

    /* get interface index */
    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    const char* deviceName = cs.get<const char*>("deviceName");
    struct ifreq buffer;
    int ifindex;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, deviceName, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &buffer) < 0) {
        printf("Error: could not get interface index\n");
        // TODO handle error
    }
    ifindex = buffer.ifr_ifindex;

    /* fill in frame */
    union ethframe frame;
    unsigned char src[ETH_ALEN] = {0x00, 0x0c, 0x29, 0x6f, 0x01, 0x59};
    unsigned char dest[ETH_ALEN] = {0x00, 0x0c, 0x29, 0x6f, 0x01, 0x59};
    // unsigned char dest[ETH_ALEN] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x90};
    unsigned short proto = ETH_P_TSN;
    const char* data = "hello world\n";
    unsigned short data_len = strlen(data);
    memcpy(frame.field.header.h_dest, dest, ETH_ALEN);
    memcpy(frame.field.header.h_source, src, ETH_ALEN);
    frame.field.header.h_proto = htons(proto);
    memcpy(frame.field.data, data, data_len);

    unsigned int frame_len = data_len + ETH_HLEN;

    /* set linklayer address */
    struct sockaddr_ll saddrll;
    memset((void*)&saddrll, 0, sizeof(saddrll));
    saddrll.sll_family = PF_PACKET;
    saddrll.sll_ifindex = ifindex;
    saddrll.sll_halen = ETH_ALEN;
    memcpy((void*)(saddrll.sll_addr), (void*)dest, ETH_ALEN);

    if (write(STDIN_FILENO, frame.buffer, frame_len)) {
        INFO("Write success!");
    } else {
        INFO("Write error!");
    }

    /* send data */
    if (sendto(sockfd, frame.buffer, frame_len, 0, (struct sockaddr*)&saddrll, sizeof(saddrll)) > 0)
        INFO("Send success!");
    else
        INFO("Error, could not send");

    /* receive data */
    char recvbuffr[1024];
    socklen_t recvlen = sizeof(saddrll);
    if (recvfrom(sockfd, recvbuffr, frame_len, 0, (struct sockaddr*)&saddrll, &recvlen) > 0) {
        INFO("Recv success!");
    } else {
        INFO("Recv error!");
    }
}

TEST(TEST_PORT_MANAGER, TEST_PORT_MANAGER) {
    TestPortManager();
}

TEST(TEST_PORT_MANAGER, TEST_PORT) {
    // TestPort();
}

TEST(TEST_PORT_MANAGER, TEST_PCAP) {
    // TestPCAP();
}

TEST(TEST_PORT_MANAGER, TEST_SEND_RECV) {
    // TestSendRecv();
}