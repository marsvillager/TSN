#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory>

using namespace faker_tsn;
using namespace std;

void Test_State_Pattern() {
    // 定义
    TSNContext& context = TSNContext::getInstance();
    shared_ptr<ITSNContextState> initState = make_shared<InitTSNContextState>();
    shared_ptr<ITSNContextState> runState = make_shared<RunTSNContextState>();

    initState->doAction(context);
    runState->doAction(context);
}

TEST(TEST_STATE_PATTERN, TEST_STATE_PATTERN_READ) {
    Test_State_Pattern();
}