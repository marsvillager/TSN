#include "InitTSNContextState.h"

#include "TSNContext.h"

namespace faker_tsn {

InitTSNContextState::InitTSNContextState(/* args */) {
}

InitTSNContextState::~InitTSNContextState() {
}

void InitTSNContextState::doAction(TSNContext& context) {
    auto prt = shared_from_this();
    context.setState(prt);
    INFO("START STATE");

    ConfigSetting& cs = ConfigSetting::getInstance();
    /* load config file */
    context.setDeviceName(cs.get<const char*>("deviceName"));

    /* load config file */
    string filename = cs.get<string>("routesDir");
    MacTable::loadRouteXML(filename);
}

}  // namespace faker_tsn
