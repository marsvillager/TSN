#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <string>

using namespace faker_tsn;
using namespace std;

static void TestInitialization() {
    // 读取配置文件并全部输出
    ConfigSetting& cs = ConfigSetting::getInstance();
    cout << cs << endl;

    // 取 deviceName 这个属性，并输出其值
    const char* deviceName = cs.get<const char*>("deviceName");
    INFO(deviceName);

    // 建立时钟事件
    string className = cs.get<string>("class");
    IClock* clock = Reflector::getNewInstance<IClock>(className);
    cout << endl;

    // 输出原端口值和修改后的端口值，对于 ConfigSetting cs 这个对象而言，端口改变
    int port = cs.get<int>("port");
    INFO("原端口: " + to_string(port));
    cs.set("port", 8080);
    port = cs.get<int>("port");
    INFO("修改后的端口: " + to_string(port) + "\n");

    // 1、增加了 object 的键值对
    string objectName = "DefaultObject1";
    objectName = cs.get("object", objectName);
    INFO(objectName);

    // 修改 object 值
    objectName = cs.get<string>("object", "DefaultObject2");
    INFO(objectName);
    cout << endl;

    // 输出修改后的 ConfigSetting cs
    cout << cs << endl;

    // 2、用 cs.set 增加键值对
    string add = "add";
    cs.set("object", add);
    objectName = cs.get<string>("object");
    INFO(objectName);

    // 输出修改后的 ConfigSetting cs
    cout << cs << endl;
}

TEST(TEST_CONFIG_SETTING, TEST_INITIALIZATION) {
    TestInitialization();
}