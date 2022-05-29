#ifndef _MAC_TABLE_H
#define _MAC_TABLE_H

#include <netinet/if_ether.h>
#include <tinyxml2/tinyxml2.h>
#include <exception>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../networking/interface/MacAddress.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/log/Log.h"

using namespace tinyxml2;

namespace faker_tsn {

struct ForwardPair {
    std::vector<unsigned short> ports;  // outbound port or ports
    long long timeout;                  // timeout, -1 means it will never timeout

    ForwardPair(std::vector<unsigned short>& ports, long long timeout = -1)
        : ports(ports), timeout(timeout) {}

    std::string toString() {
        std::stringstream ss;
        ss << "PORT: ";
        for (auto it = ports.begin(); it != ports.end(); it++) {
            ss << *it << " ";
        }
        ss << "TIMEOUT: " << timeout;
        return ss.str();
    }
};

/* funcotr as Hash parameter*/
struct HashFunc {
    int operator()(const unsigned char* mac) const {
        unsigned long long temp;
        memcpy(&temp, mac, ETH_ALEN);
        auto func = std::hash<unsigned long long>();
        return func(temp);
    }
};  // namespace faker_tsn

/* functor as Pred parameter */
struct MacAddressCmp {
    bool operator()(const unsigned char* mac1, const unsigned char* mac2) const {
        for (int i = 0; i < ETH_ALEN; i++) {
            if (mac1[i] != mac2[i])
                return false;
        }
        return true;
    }
};

/* lambda expression as Hash parameter */
auto macCmpFunc = [](const unsigned char* mac1, const unsigned char* mac2) {
    for (int i = 0; i < ETH_ALEN; i++)
        if (mac1[i] != mac2[i])
            return false;
    return true;
};

/* lambda expression as Pred parameter */
auto macHashFunc = [](const unsigned char* mac) {
    int seed = 131;  // 31  131 1313 13131131313 etc//
    int hash = 0;
    while (*mac) {
        hash = (hash * seed) + (*mac);
        mac++;
    }
    return hash & (0x7FFFFFFF);
};

class LackOfTagException : public std::exception {
  private:
    std::string m_tagName;

  public:
    LackOfTagException(std::string tagName)
        : m_tagName(tagName) {}

    const char* what() const noexcept {
        std::stringstream ss;
        ss << "Lack of tag \"";
        ss << this->m_tagName;
        ss << "\"";
        return ss.str().c_str();
    }
};

struct MacTable {
    static std::unordered_map<mac_token, ForwardPair> items;  // <mac addr, forward pair> map

    static void loadRouteXML(std::string filename);

    static std::string toString();

    /* convert hex string mac address to raw mac address */
    static void parseMacAddress(const std::string& macString, unsigned char* mac);

    /* conver port string to port vector */
    static std::vector<unsigned short> parsePortIndex(const std::string& portString);

    static void addItem(const unsigned char* mac, std::vector<unsigned short>& ports, long long timeout = -1) {
        mac_token temp;
        memcpy(&temp, mac, ETH_ALEN);
        ForwardPair forwardPair(ports, timeout);
        MacTable::items.emplace(std::move(temp), std::move(forwardPair));
    }

    static void removeItem(const unsigned char* mac) {
        mac_token temp;
        memcpy(&temp, mac, ETH_ALEN);
        auto it = MacTable::items.find(temp);
        if (it != MacTable::items.end()) {
            MacTable::items.erase(it);
        }
    }

    static void updateItem(const unsigned char* mac, std::vector<unsigned short>& ports, long long timeout = -1) {
        mac_token temp;
        memcpy(&temp, mac, ETH_ALEN);
        auto it = MacTable::items.find(temp);
        if (it != MacTable::items.end()) {
            it->second.ports = ports;
            it->second.timeout = timeout;
        }
    }

    static std::vector<unsigned short> findOutboundPorts(const unsigned char* mac) {
        mac_token temp;
        memcpy(&temp, mac, ETH_ALEN);
        auto it = MacTable::items.find(temp);
        if (it != MacTable::items.end()) {
            return it->second.ports;
        } else {
            return std::vector<unsigned short>();
        }
    }
};

}  // namespace faker_tsn

#endif  // _MAC_TABLE_H;