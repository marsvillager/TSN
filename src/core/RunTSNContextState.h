#if !defined(_RUN_TSN_CONTEXT_H)
#define _RUN_TSN_CONTEXT_H

#include "ITSNContextState.h"

namespace faker_tsn {

class RunTSNContextState : public ITSNContextState, public std::enable_shared_from_this<RunTSNContextState> {
  private:
    /* data */
  public:
    RunTSNContextState(/* args */);
    ~RunTSNContextState();

    virtual void doAction(TSNContext&) override;
};

}  // namespace faker_tsn

#endif  // _RUN_TSN_CONTEXT_H
