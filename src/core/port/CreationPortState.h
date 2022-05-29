#ifndef _CREATION_PORT_STATE
#define _CREATION_PORT_STATE

#include <pcap/pcap.h>
#include <iomanip>
#include <sstream>
#include "DownPortState.h"
#include "IPort.h"
#include "IPortState.h"
#include "PortManager.h"
#include "UpPortState.h"

namespace faker_tsn {

class CreationPortState : public IPortState, public std::enable_shared_from_this<CreationPortState> {
  private:
    /* data */
  public:
    CreationPortState(/* args */);
    ~CreationPortState();

    /* template method */
    void doAction(std::shared_ptr<IPort>) override;

    /* create raw socket */
    int createSocket();

    /* open pcap */
    int openPCAP(const char*, std::shared_ptr<pcap_t>);
};

}  // namespace faker_tsn

#endif  // _CREATION_PORT_STATE