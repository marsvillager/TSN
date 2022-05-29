#ifndef _MAC_ADDRESS_H
#define _MAC_ADDRESS_H

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/socket.h>
#include <memory.h>
#include <netinet/if_ether.h>
#include <stdio.h>

#include <exception>

#include "IAddress.h"

namespace faker_tsn {

typedef unsigned long long mac_token;

class MacAddress : public IAddress {
  private:
    unsigned char m_family;    // address family
    int m_ifindex;             // interface index
    unsigned char m_protocol;  // protocol
    unsigned char m_mac[6];    // raw mac address

  public:
    MacAddress()
        : m_family(0), m_ifindex(-1), m_protocol(0) {
        memset(this->m_mac, 0x00, 6);
    }

    MacAddress(unsigned char* mac, unsigned char family = 0, unsigned char ifindex = -1, unsigned char protocol = 0)
        : m_family(family), m_ifindex(ifindex), m_protocol(protocol) {
        memcpy(this->m_mac, mac, 6);
    }

    /* get raw struct sockaddr */
    virtual void getRawSockAddr(sockaddr** addr) {
        struct sockaddr_ll* addr_ll = (struct sockaddr_ll*)malloc(sizeof(struct sockaddr_ll));
        memset(addr_ll, 0x00, sizeof(addr_ll));
        addr_ll->sll_family = this->m_family;             // set family
        addr_ll->sll_ifindex = this->m_ifindex;           // set index
        addr_ll->sll_halen = ETH_ALEN;                    // set address length
        addr_ll->sll_protocol = htons(this->m_protocol);  // set protocp
        memcpy(addr_ll->sll_addr, this->m_mac, ETH_ALEN);
        *addr = (struct sockaddr*)addr_ll;
    }

    inline void setFamily(unsigned char famlify) {
        this->m_family = famlify;
    }

    inline void setIndex(int index) {
        this->m_ifindex = index;
    }

    inline void setProtocol(unsigned char protocol) {
        this->m_protocol = protocol;
    }

    inline void setMacAddress(unsigned char* mac) {
        memcpy(this->m_mac, mac, 6);
    }

    inline unsigned char getFamily() {
        return this->m_family;
    }

    inline int getInterfaceIndex() {
        return this->m_ifindex;
    }

    inline unsigned char getProtocol() {
        return this->m_protocol;
    }

    inline void getRaw(unsigned char* mac) {
        memcpy(mac, this->m_mac, 6);
    }

    std::string toString() {
        return MacAddress::toRaw(this->m_mac);
    }

    static std::string toRaw(unsigned char* mac) {
        char macStr[32];
        sprintf(macStr, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return std::string(macStr);
    }
};

}  // namespace faker_tsn

#endif  // _MAC_ADDRESS_H