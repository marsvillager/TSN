#ifndef _PORT_MANAGER_H
#define _PORT_MANAGER_H

#include <pcap/pcap.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "../../networking/Reactor.h"
#include "../../utils/log/Log.h"
#include "CreationPortState.h"
#include "DataPort.h"

namespace faker_tsn {

class PortManager {
  private:
    std::vector<const char*> m_deviceNames;
    std::vector<std::shared_ptr<IPort>> m_ports;

  public:
    static const char* s_portFilterList[];
    static const int s_portFilterLen;

    PortManager() = default;

    ~PortManager() = default;

    /* get device name list */
    std::vector<const char*>& getAllDeviceName();

    /* get port using index */
    std::shared_ptr<IPort> getPort(uint8_t index);

    /* add port */
    void appendPort(std::shared_ptr<IPort>& port);

    /* find all device name */
    void findAllDeviceName();

    /* create port from device name list */
    void createPortFromDeviceNameList();

    /* add IEventHandler to Reactor */
    void bindEventHandler(std::shared_ptr<Reactor> reactor);

    /* find mac address */
    static int findMacAddress(std::shared_ptr<IPort>, const char*, char[]);
};

}  // namespace faker_tsn

#endif  // _PORT_MANAGER_H