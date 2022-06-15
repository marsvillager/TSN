#include <faker_tsn.h>

using namespace faker_tsn;
using namespace std;

int main (int argc, char **argv) {
    TSNContext& context = TSNContext::getInstance();
    shared_ptr<ITSNContextState> initState = make_shared<InitTSNContextState>();
    shared_ptr<ITSNContextState> runState = make_shared<RunTSNContextState>();

    initState->doAction(context);
    runState->doAction(context);
}