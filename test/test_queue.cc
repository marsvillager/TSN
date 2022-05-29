#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace faker_tsn;
using namespace std;

static void TestGateControlList() {
    GateControlList* gcl = dynamic_cast<GateControlList*>(REFLECTOR::CreateByTypeName(std::string("faker_tsn::GateControlList"), 2U));  // scope declaration is required
}

TEST(TEST_QUEUE, TEST_GATE_CONTROL_LIST) {
    TestGateControlList();
}