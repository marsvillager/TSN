#ifndef _SEND_TSN_FRAME_EVENT_HANDLER_H
#define _SEND_TSN_FRAME_EVENT_HANDLER_H

#include <linux/if_packet.h>
#include <memory>

#include "../core/queue/QueueContext.h"
#include "../utils/ConvertUtils.h"
#include "../utils/log/Log.h"
#include "IEventHandler.h"
#include "Reactor.h"
#include "frame/TSNFrameBody.h"
#include "frame/VlanTCI.h"
#include "frame/if_ether.h"

namespace faker_tsn {

class SendTSNFrameEventHandler : public IEventHandler {
  private:
    HANDLE m_handle;
    struct sockaddr_ll m_sockAddrII;
    std::shared_ptr<QueueContext> m_queueContext;

  public:
    SendTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII, std::shared_ptr<QueueContext> queueContext);

    virtual ~SendTSNFrameEventHandler() override;

    /* hook method that is called by Reactor */
    virtual void handle_event(EVENT_TYPE eventType) override;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() override;
};

}  // namespace faker_tsn

#endif  // _SEND_TSN_FRAME_EVENT_HANDLER_H