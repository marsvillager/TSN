#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <iostream>

#include "../src/utils/reflector/DynamicCreate.h"

using namespace faker_tsn;
using namespace std;

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
    Cmd() { cout << "Create Cmd " << endl; }
    virtual void Say() {
        cout << "I am Cmd" << endl;
    }
};

class Step : public REFLECT_OBJECT, public DynamicCreator<Step, string, int> {
  public:
    Step(const string& strType, int iSeq) { cout << "Create Step " << strType << " with seq " << iSeq << endl; }
    virtual void Say() {
        cout << "I am Step" << endl;
    }
};

class Hello : public REFLECT_OBJECT, public DynamicCreator<Hello, int> {
  public:
    Hello(int iSeq) { cout << "Create Hello with seq " << iSeq << endl; }
    virtual void Say() {
        cout << "I am Hello" << endl;
    }
};

static void TestReflector() {
    Person* bob = ::Reflector::getNewInstance<Person>("Bob");
    Person* allen = ::Reflector::getNewInstance<Person>("Allen");
    bob->show();
    allen->show();
    cout << "----------------------------------------------------------------------" << endl;
    ITimer* timer = ::Reflector::getNewInstance<PQTimer>("PQTimer");
}

static void TestDynamicCreate() {
    cout << "----------------------------------------------------------------------" << endl;
    // Actor* p1 = ActorFactory<string, int>::Instance()->Create(string("Cmd"), string("Cmd"), 1001);
    // Actor* p3 = ActorFactory<>::Instance()->Create(string("Cmd"));
    REFLECT_OBJECT* p1 = REFLECTOR::CreateByTypeName(string("faker_tsn::Cmd"));
    p1->Say();
    // cout << abi::__cxa_demangle(typeid(Worker).name(), nullptr, nullptr, nullptr) << endl;
    cout << "----------------------------------------------------------------------" << endl;
    REFLECT_OBJECT* p2 = REFLECTOR::CreateByTypeName(string("faker_tsn::Step"), string("Step"), 1002);
    p2->Say();
    cout << "----------------------------------------------------------------------" << endl;
    REFLECT_OBJECT* hello = REFLECTOR::CreateByTypeName(string("faker_tsn::Hello"), 1002);
    hello->Say();
    cout << "----------------------------------------------------------------------" << endl;
    REFLECT_OBJECT* gcl = REFLECTOR::CreateByTypeName(string("faker_tsn::GateControlList"), 2U);  // 无符号整形
    gcl->Say();
    cout << "----------------------------------------------------------------------" << endl;
    REFLECT_OBJECT* queue = REFLECTOR::CreateByTypeName(string("faker_tsn::IEEE8021Queue"), (unsigned short)1, (uint8_t)1);
    queue->Say();
}

TEST(TEST_REFLECTOR, TEST_REFLECTOR) {
    TestReflector();
    TestDynamicCreate();
}