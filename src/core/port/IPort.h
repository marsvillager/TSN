#ifndef _I_PORT_H
#define _I_PORT_H

#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <memory>
#include <sstream>
#include <string>

#include "../../networking/frame/RelayEntity.h"
#include "../../utils/log/Log.h"

namespace faker_tsn {

typedef unsigned short PORT_INDEX_TYPE;

class IPortState;

class IPort {
  public:
    virtual ~IPort() = default;

    /* change current state */
    virtual void setState(const std::shared_ptr<IPortState>&) = 0;

    /* return current state */
    virtual std::shared_ptr<IPortState>& getState() = 0;

    /* get device id */
    virtual uint8_t getDeviceID() const = 0;

    /* Deprecated : set socket file descripter */
    virtual void setSockfd(const int) = 0;

    /* set socket for sending */
    virtual void setInSockfd(const int) = 0;

    /* set socket for receiving */
    virtual void setOutSockfd(const int) = 0;

    /* get socket for sendingg */
    virtual int getInSockfd() = 0;

    /* get socket for receiving */
    virtual int getOutSockfd() = 0;

    /* get socket file descripter */
    virtual int getSockfd() = 0;

    /* set device name */
    virtual void setDeviceName(const char*) = 0;

    /* set mac address */
    virtual void setMacAddress(const char[]) = 0;

    /* get device name */
    virtual const char* getDeviceName() const = 0;

    /* test send data */
    virtual void sendTest() = 0;

    /* set events types */
    virtual void setEventTypes(int) = 0;

    /* get event types */
    virtual int getEventTypes() = 0;

    /* create raw socket */
    virtual void createSocket() = 0;

    /* register handler */
    virtual void registerEventHandler() = 0;

    /* input something into port */
    virtual void input(void*, size_t, RELAY_ENTITY type) = 0;

    // virtual int enableCapture() = 0;

    virtual std::string toString() = 0;
};

/* self-defined exception */
class PortCreationException : public std::exception {
  public:
    virtual const char* what() const noexcept {
        return "Create port failed";
    }
};

}  // namespace faker_tsn

#endif  // _I_PORT_H