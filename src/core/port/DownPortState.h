#ifndef _DOWN_PORT_STATE
#define _DOWN_PORT_STATE

#include "IPortState.h"

namespace faker_tsn {

class DownPortState : public IPortState, public std::enable_shared_from_this<DownPortState> {
  private:
    /* data */
  public:
    DownPortState(/* args */);
    ~DownPortState();

    void doAction(std::shared_ptr<IPort>) override;
};

}  // namespace faker_tsn

#endif  // _DOWN_PORT_STATE