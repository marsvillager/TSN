#ifndef _UP_PORT_STATE
#define _UP_PORT_STATE

#include "IPortState.h"

namespace faker_tsn {

class UpPortState : public IPortState, public std::enable_shared_from_this<UpPortState> {
  private:
    /* data */
  public:
    UpPortState(/* args */);
    ~UpPortState();

    void doAction(std::shared_ptr<IPort>) override;
};

}  // namespace faker_tsn

#endif  // _UP_PORT_STATE