#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <iostream>

#include "../src/utils/reflector/DynamicCreate.h"

namespace faker_tsn {

class Person : public ::ReflectObject {
  public:
    Person() {
        INFO("Person Constructor");
    }
    virtual ~Person() = default;
    virtual void show() {
        INFO("Person");
    }
};

class Bob : public Person {
  public:
    Bob() {
        INFO("Bob Constructor");
    }
    virtual void show() override {
        INFO("Bob");
    }
};

class Allen : public Person {
  public:
    Allen() {
        INFO("Allen Constructor");
    }
    virtual void show() override {
        INFO("Allen");
    }
};

REFLECT(Bob);    // register "Bob"
REFLECT(Allen);  // register "Allen"

class Cmd : public REFLECT_OBJECT, public DynamicCreator<Cmd> {
  public:
    Cmd() { std::cout << "Create Cmd " << std::endl; }
    virtual void Say() {
        std::cout << "I am Cmd" << std::endl;
    }
};

class Step : public REFLECT_OBJECT, public DynamicCreator<Step, std::string, int> {
  public:
    Step(const std::string& strType, int iSeq) { std::cout << "Create Step " << strType << " with seq " << iSeq << std::endl; }
    virtual void Say() {
        std::cout << "I am Step" << std::endl;
    }
};

class Hello : public REFLECT_OBJECT, public DynamicCreator<Hello, int> {
  public:
    Hello(int iSeq) { std::cout << "Create Hello with seq " << iSeq << std::endl; }
    virtual void Say() {
        std::cout << "I am Hello" << std::endl;
    }
};

}  // namespace faker_tsn

using namespace faker_tsn;

static void TestReflector() {
    Person* bob = ::Reflector::getNewInstance<Person>("Bob");
    Person* allen = ::Reflector::getNewInstance<Person>("Allen");
    bob->show();
    allen->show();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    ITimer* timer = ::Reflector::getNewInstance<PQTimer>("PQTimer");
}

static void TestDynamicCreate() {
    std::cout << "----------------------------------------------------------------------" << std::endl;
    // Actor* p1 = ActorFactory<std::string, int>::Instance()->Create(std::string("Cmd"), std::string("Cmd"), 1001);
    // Actor* p3 = ActorFactory<>::Instance()->Create(std::string("Cmd"));
    REFLECT_OBJECT* p1 = REFLECTOR::CreateByTypeName(std::string("faker_tsn::Cmd"));
    p1->Say();
    // std::cout << abi::__cxa_demangle(typeid(Worker).name(), nullptr, nullptr, nullptr) << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    REFLECT_OBJECT* p2 = REFLECTOR::CreateByTypeName(std::string("faker_tsn::Step"), std::string("Step"), 1002);
    p2->Say();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    REFLECT_OBJECT* hello = REFLECTOR::CreateByTypeName(std::string("faker_tsn::Hello"), 1002);
    hello->Say();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    REFLECT_OBJECT* gcl = REFLECTOR::CreateByTypeName(std::string("faker_tsn::GateControlList"), 2U);  // 无符号整形
    gcl->Say();
    std::cout << "----------------------------------------------------------------------" << std::endl;
    REFLECT_OBJECT* queue = REFLECTOR::CreateByTypeName(std::string("faker_tsn::IEEE8021Queue"), (unsigned short)1, (uint8_t)1);
    queue->Say();
}

TEST(TEST_REFLECTOR, TEST_REFLECTOR) {
    TestReflector();
    TestDynamicCreate();
}