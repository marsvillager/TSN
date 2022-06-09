#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory>

using namespace faker_tsn;
using namespace std;

static void Sender() {
    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    const char* deviceName = cs.get<const char*>("deviceName");

    // crate port 1
    shared_ptr<IPort> port1 = make_shared<DataPort>(deviceName);
    shared_ptr<IPortState> creationState1 = make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // crate port 2
    shared_ptr<IPort> port2 = make_shared<DataPort>(deviceName);
    shared_ptr<IPortState> creationState2 = make_shared<CreationPortState>();
    creationState1->doAction(port2);

    // add port into port manager
    shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    portManager->appendPort(port1);
    portManager->appendPort(port2);

    /* load config file */
    string filename = cs.get<string>("routesDir");
    MacTable::loadRouteXML(filename);

    // enable reactor
    Reactor::getInstance().handle_events();
}

TEST(TEST_H1_SENDER, SENDER) {
    Sender();
}