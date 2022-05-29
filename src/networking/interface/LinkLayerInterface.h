#ifndef _LINK_LAYER_INTERFACE_H
#define _LINK_LAYER_INTERFACE_H

#include <linux/if_packet.h>
#include <memory.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <sstream>
#include <vector>

#include "../../utils/log/Log.h"
#include "../../utils/ConvertUtils.h"
#include "../frame/if_ether.h"
#include "IAddress.h"
#include "IInterface.h"
#include "MacAddress.h"

namespace faker_tsn {

class LinkLayerInterface : public IInterface {
  private:
    const char* m_name;                        // interface name
    int m_index;                               // interface index
    std::shared_ptr<MacAddress> m_macAddress;  // mac address

  public:
    LinkLayerInterface()
        : m_name(""), m_index(-1) {
    }

    LinkLayerInterface(const char* name)
        : m_name(name) {
        this->m_index = LinkLayerInterface::getIndex(name);
        this->m_macAddress.reset(LinkLayerInterface::getMacAddress(name));
    }

    LinkLayerInterface(const char* name, std::shared_ptr<MacAddress> macAddress)
        : m_name(name), m_macAddress(macAddress) {
        this->setIndex(LinkLayerInterface::getIndex(name));
    }

    /* bind socket to sockaddr_ll */
    virtual void bind(int sockfd, std::shared_ptr<IAddress> address);

    virtual std::string toString() override {
        std::stringstream ss;
        ss << "name = " << this->m_name << std::endl;
        ss << "index = " << std::to_string(this->m_index) << std::endl;
        ss << "mac = " << this->m_macAddress->toString();
        return ss.str();
    }

    inline void setIndex(int index) {
        this->m_index = index;
    }

    inline void setMacAddress(const std::shared_ptr<MacAddress>& macAddress) {
        this->m_macAddress = macAddress;
    }

    inline int getIndex() {
        return this->m_index;
    }

    inline std::shared_ptr<MacAddress> getMacAddress() {
        return this->m_macAddress;
    }

    /* find interface */
    static LinkLayerInterface* findInterface(const char* name);

    /* find all interface */
    static std::vector<LinkLayerInterface> findAllInterfaces();

    /* get mac address by interface name */
    static MacAddress* getMacAddress(const char* name);

    /* get interface index by interface name */
    static int getIndex(const char* name);
};

class InterfaceNotFound : public std::exception {
    const char* what() const noexcept {
        return "interface not found";
    }
};

class MacAddressNotFound : public std::exception {
    const char* what() const noexcept {
        return "mac address not found";
    }
};

class IndexNotFound : public std::exception {
    const char* what() const noexcept {
        return "interface index not found";
    }
};

class BindSocketToInterfaceFailed : public std::exception {
    const char* what() const noexcept {
        return "Failed to bind socket to interface";
    }
};

}  // namespace faker_tsn

#endif  // _LINK_LAYER_INTERFACE_H