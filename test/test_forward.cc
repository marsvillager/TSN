#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory>

using namespace faker_tsn;
using namespace std;

static void Sender() {
    // crate port 1
    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    const char* deviceName1 = cs.get<const char*>("deviceName");
    shared_ptr<IPort> port1 = make_shared<DataPort>(deviceName1);
    shared_ptr<IPortState> creationState1 = make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // add port into port manager
    shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    portManager->appendPort(port1);

    /* load config file */
    string filename = cs.get<string>("routesDir");
    MacTable::loadRouteXML(filename);

    // enable reactor
    Reactor::getInstance().handle_events();
}

TEST(TEST_H1_SENDER, SENDER) {
    Sender();
}