#include <faker_tsn.h>
#include <gtest/gtest.h>

using namespace faker_tsn;

void Test_Logger() {
    Log::setLoggerLevel(Log::INFO_LEVEL);
    DEBUG("debug world");
    INFO("info world");
}

TEST(TEST_LOGGER, TEST_LOGGER_READ) {
    Test_Logger();
}