#ifndef _RECV_TSN_FRAME_EVENT_HANDLER_H
#define _RECV_TSN_FRAME_EVENT_HANDLER_H

#include <linux/if_packet.h>

#include "../core/forward/ForwardFunction.h"
#include "../core/forward/MacTable.h"
#include "../core/queue/QueueContext.h"
#include "../networking/Reactor.h"
#include "../networking/frame/RelayEntity.h"
#include "../utils/ConvertUtils.h"
#include "../utils/config/ConfigSetting.h"
#include "../utils/log/Log.h"
#include "IEventHandler.h"
#include "frame/TSNFrameBody.h"
#include "frame/VlanTCI.h"
#include "frame/if_ether.h"

namespace faker_tsn {

class RecvTSNFrameEventHandler : public IEventHandler {
  private:
    HANDLE m_handle;
    struct sockaddr_ll m_sockAddrII;
    std::shared_ptr<QueueContext> m_queueContext;
    bool m_isEnhanced;

  public:
    RecvTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII);

    virtual ~RecvTSNFrameEventHandler() override;

    /* hook method that is called by Reactor */
    virtual void handle_event(EVENT_TYPE eventType) override;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() override;
};

}  // namespace faker_tsn

#endif  // _RECV_TSN_FRAME_EVENT_HANDLER_H