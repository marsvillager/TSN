#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory>

using namespace faker_tsn;

static void Sender() {
    // crate port 1
    // const char* name1 = "s2-eth0";
    const char* name1 = "ens33";
    std::shared_ptr<IPort> port1 = std::make_shared<DataPort>(name1);
    std::shared_ptr<IPortState> creationState1 = std::make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // crate port 2
    // const char* name2 = "s2-eth1";
    // const char* name2 = "ens33";
    // std::shared_ptr<IPort> port2 = std::make_shared<DataPort>(name2);
    // std::shared_ptr<IPortState> creationState2 = std::make_shared<CreationPortState>();
    // creationState2->doAction(port2);

    // crate port 3
    // const char* name3 = "s2-eth2";
    // const char* name3 = "ens33";
    // std::shared_ptr<IPort> port3 = std::make_shared<DataPort>(name3);
    // std::shared_ptr<IPortState> creationState3 = std::make_shared<CreationPortState>();
    // creationState2->doAction(port3);

    // add port into port manager
    std::shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    portManager->appendPort(port1);
    // portManager->appendPort(port2);
    // portManager->appendPort(port3);

    // load mac table
    MacTable::loadRouteXML("/home/reptile/下载/TSN/config/routes.xml");

    // enable reactor
    Reactor::getInstance().handle_events();
}

TEST(TEST_H1_SENDER, SENDER) {
    Sender();
}