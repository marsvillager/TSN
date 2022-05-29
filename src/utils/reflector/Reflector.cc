#include "Reflector.h"

Reflector::~Reflector() {
    for (auto it = this->objectFactories.begin(); it != this->objectFactories.end(); it++) {
        delete it->second;
    }
    this->objectFactories.clear();
}

void Reflector::registerFactory(const std::string& className, ObjectFactory* objectFactory) {
    if (this->objectFactories.find(className) != this->objectFactories.end()) {
    } else {
        this->objectFactories[className] = objectFactory;
    }
}

ReflectObject* Reflector::getNewInstance(const std::string& className) {
    auto it = this->objectFactories.find(className);
    if (it != this->objectFactories.end()) {
        ObjectFactory* objectFactory = it->second;
        return objectFactory->newInstance();
    }
    return NULL;
}