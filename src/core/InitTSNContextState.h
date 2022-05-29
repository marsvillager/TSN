#if !defined(_IINIT_TSN_CONTEXT_STATE)
#define _IINIT_TSN_CONTEXT_STATE

#include "ITSNContextState.h"

namespace faker_tsn {

class InitTSNContextState : public ITSNContextState, public std::enable_shared_from_this<InitTSNContextState> {
  private:
    /* data */
  public:
    InitTSNContextState(/* args */);
    ~InitTSNContextState();

    virtual void doAction(TSNContext&) override;
};

}  // namespace faker_tsn

#endif  // _IINIT_TSN_CONTEXT_STATE
