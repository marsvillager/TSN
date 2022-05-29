#include "DataPort.h"

#include <sys/socket.h>

namespace faker_tsn {

uint8_t DataPort::s_portNum = 0;

DataPort::DataPort(const char* deviceName)
    : m_deviceID(DataPort::s_portNum++), m_deviceName(deviceName) {
    INFO("Construct Port[" + std::to_string(s_portNum) + "|" + std::string(this->m_deviceName) + "] created");
    this->m_sockfd = socket(PF_PACKET, SOCK_RAW, 0);
    printf("m_sockfd: %d\n", this->m_sockfd);
}

DataPort::~DataPort() {
    /* close socket */
    shutdown(this->m_sockfd, SHUT_RDWR);
    close(this->m_sockfd);

    INFO("Destruct " + std::to_string(s_portNum) + "|" + std::string(this->m_deviceName) + "] closed");
}

void DataPort::setState(const std::shared_ptr<IPortState>&) {
    this->state = state;
}

std::shared_ptr<IPortState>& DataPort::getState() {
    return this->state;
}

uint8_t DataPort::getDeviceID() const {
    return this->m_deviceID;
}

void DataPort::setSockfd(int sockfd) {
    this->m_sockfd = sockfd;
}

int DataPort::getSockfd() {
    return this->m_sockfd;
}

void DataPort::setMacAddress(const char mac[]) {
    memcpy(this->m_hwaddr, mac, sizeof(this->m_hwaddr));
}

void DataPort::setDeviceName(const char* deviceName) {
    this->m_deviceName = deviceName;
}

const char* DataPort::getDeviceName() const {
    return this->m_deviceName;
}

void DataPort::setEventTypes(int events) {
    this->m_eventTypes = events;
}

int DataPort::getEventTypes() {
    return this->m_eventTypes;
}

void DataPort::createSocket() {
    /* create inbound socket */
    int inSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (inSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    this->setInSockfd(inSockfd);
    INFO("Create inbound socket: " + std::to_string(this->getInSockfd()));

    /* create outbound socket */
    int outSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (outSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    this->setOutSockfd(outSockfd);
    INFO("Create outbound socket: " + std::to_string(this->getOutSockfd()));

    /* create LinkLayerInterface */
    try {
        this->m_interface.reset(LinkLayerInterface::findInterface(this->m_deviceName));
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }
    INFO("Create LinkLayerInterface:\n" + this->m_interface->toString());

    /* set MacAddress */
    std::shared_ptr<MacAddress> macAddress = this->m_interface->getMacAddress();
    macAddress->setFamily(PF_PACKET);    // set family
    macAddress->setProtocol(ETH_P_ALL);  // set protocol
    INFO("Create MacAddress");

    /* bind inbound socket to link layer interface */
    try {
        this->m_interface->bind(this->m_inSockfd, macAddress);
        INFO("Bind port with interface");
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }

    printf("device id: %d\n", this->m_deviceID);

    /* create QueueContext */
    this->m_queueContext.reset(new QueueContext(this->m_deviceID));
    INFO("Create QueueConext\n");
}

void DataPort::registerEventHandler() {
    /* fill in struct sockaddr_ll */
    struct sockaddr_ll* addr_ll;
    this->m_interface->getMacAddress()->getRawSockAddr((struct sockaddr**)&addr_ll);

    /* register for inbound socket */
    std::shared_ptr<IEventHandler> recvTSNFrameEventHandler = std::make_shared<RecvTSNFrameEventHandler>(this->m_inSockfd, *addr_ll);
    Reactor::getInstance().register_handler(recvTSNFrameEventHandler, EVENT_TYPE::READ);
    INFO("Register RecvTSNFrameEventHandler in Reactor");

    /* register for outbound socket */
    std::shared_ptr<IEventHandler> sendTSNFrameEventHandler = std::make_shared<SendTSNFrameEventHandler>(this->m_outSockfd, *addr_ll, this->m_queueContext);
    Reactor::getInstance().register_handler(sendTSNFrameEventHandler, EVENT_TYPE::WRITE);
    INFO("Register SendTSNFrameEventHandler in Reactor");
}

void DataPort::sendTest() {
    INFO("TEST SEND");
    INFO(this->m_deviceName);
    union ethframe {
        struct
        {
            struct ethhdr header;
            unsigned char data[ETH_DATA_LEN];
        } field;
        unsigned char buffer[ETH_FRAME_LEN];
    };
    /* get interface index */
    struct ifreq buffer;
    int ifindex;
    printf("m_sockfd: %d\n", this->m_sockfd);
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, this->m_deviceName, IFNAMSIZ);
    if (ioctl(this->m_sockfd, SIOCGIFINDEX, &buffer) < 0) {
        printf("Error: could not get interface index\n");
        // TODO handle error
    }
    ifindex = buffer.ifr_ifindex;
    printf("index: %d\n", ifindex);

    /* fill in frame */
    union ethframe frame;
    unsigned char dest[ETH_ALEN] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x90};
    unsigned short proto = ETH_P_TSN;
    const char* data = "hello world\n";
    unsigned short data_len = strlen(data);
    memcpy(frame.field.header.h_dest, dest, ETH_ALEN);
    memcpy(frame.field.header.h_source, this->m_hwaddr, ETH_ALEN);
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
    std::cout << frame.buffer << std::endl;

    /* send data */
    if (sendto(this->m_sockfd, frame.buffer, frame_len, 0, (struct sockaddr*)&saddrll, sizeof(saddrll)) > 0)
        INFO("Send success!");
    else
        INFO("Error, could not send");
}

void DataPort::input(void* data, size_t len, RELAY_ENTITY type) {
    if (type == IEEE_802_1Q_TSN_FRAME) {
        INFO("Input TSN frame");
        TSNFrameBody* frame = reinterpret_cast<TSNFrameBody*>(data);
        frame->setType(type);
        this->m_queueContext->enqueue(frame);
    } else if (type == IEEE_802_1Q_TSN_FRAME_E) {
        INFO("Input enhanced-TSN frame");
        EnhancementTSNFrameBody* frame = reinterpret_cast<EnhancementTSNFrameBody*>(data);
        frame->setType(type);
        this->m_queueContext->enqueue(reinterpret_cast<EnhancementTSNFrameBody*>(data));
    }
}

}  // namespace faker_tsn
