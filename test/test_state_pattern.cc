#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory>

using namespace faker_tsn;

void Test_State_Pattern() {
    // 定义
    TSNContext& context = TSNContext::getInstance();
    std::shared_ptr<ITSNContextState> initState = std::make_shared<InitTSNContextState>();
    std::shared_ptr<ITSNContextState> runState = std::make_shared<RunTSNContextState>();

    initState->doAction(context);
    runState->doAction(context);
}

TEST(TEST_STATE_PATTERN, TEST_STATE_PATTERN_READ) {
    Test_State_Pattern();
}