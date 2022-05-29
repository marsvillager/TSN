#include "PortManager.h"

namespace faker_tsn {

const char* PortManager::s_portFilterList[] = {
    "lo", "any", "nflog", "nfqueue", "bluetooth0", "bluetooth-monitor", "docker0", "usbmon1", "usbmon2"};
const int PortManager::s_portFilterLen = 8;

void PortManager::appendPort(std::shared_ptr<IPort>& port) {
    this->m_ports.push_back(port);
}

void PortManager::findAllDeviceName() {
    pcap_if_t* alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_findalldevs(&alldevsp, errbuf);
    while (alldevsp) {
        INFO(alldevsp->name);
        this->m_deviceNames.push_back(alldevsp->name);
        alldevsp = alldevsp->next;
    }
    if (errbuf) {
        DEBUG(std::string(errbuf));
        // TODO some error handle
    }
    free(alldevsp);
}

void PortManager::createPortFromDeviceNameList() {
    for (auto it = this->m_deviceNames.begin(); it != this->m_deviceNames.end(); it++) {
        // filter unnessesarry devices
        bool flag = false;
        for (int i = 0; i < PortManager::s_portFilterLen; i++) {
            if (strcmp(PortManager::s_portFilterList[i], *it) == 0) {
                flag = true;
                break;
            }
        }
        if (flag)
            continue;

        // create port
        std::shared_ptr<IPort> port = std::make_shared<DataPort>(*it);
        INFO("create port name:" + std::string(port->getDeviceName()) + " id:" + std::to_string(port->getDeviceID()));
        auto creationState = std::make_shared<CreationPortState>();
        creationState->doAction(port);
        
        // add port
        this->m_ports.push_back(port);
    }
}

std::vector<const char*>& PortManager::getAllDeviceName() {
    return this->m_deviceNames;
}

std::shared_ptr<IPort> PortManager::getPort(uint8_t index) {
    return this->m_ports.at(index);
};

int PortManager::findMacAddress(std::shared_ptr<IPort> port, const char* deviceName, char hwaddr[]) {
    struct ifreq buffer;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, deviceName, IFNAMSIZ);
    /* option SIOCGIFHWADDR can get hardware (MAC) address */
    if (ioctl(port->getSockfd(), SIOCGIFHWADDR, &buffer) < 0) {
        ERROR("could not get hardware address");
        // TODO handle error
        return -1;
    }
    memcpy((void*)hwaddr, (void*)(buffer.ifr_hwaddr.sa_data), ETH_ALEN);
    return 0;
}

}  // namespace faker_tsn
