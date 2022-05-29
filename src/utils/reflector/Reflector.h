#ifndef _REFLECTOR_H
#define _REFLECTOR_H

#include <string>
#include <unordered_map>

class ReflectObject {
  public:
    virtual ~ReflectObject() = default;
};

class ObjectFactory {
  public:
    ObjectFactory() = default;

    virtual ~ObjectFactory() = default;

    virtual ::ReflectObject* newInstance() = 0;
};

class Reflector {
  private:
    std::unordered_map<std::string, ObjectFactory*> objectFactories;

    Reflector() = default;

    ::ReflectObject* getNewInstance(const std::string& className);

  public:
    Reflector(const Reflector&) = delete;

    ~Reflector();

    void registerFactory(const std::string& className, ObjectFactory* objectFactory);

    template <typename T>
    static T* getNewInstance(const std::string& className) {
        return dynamic_cast<T*>(Reflector::getReflector().getNewInstance(className));
    }

    static Reflector& getReflector() {
        static Reflector reflector;
        return reflector;
    }
};

#define REFLECT(name)                                                                       \
    class ObjectFactory_##name : public ObjectFactory {                                     \
      public:                                                                               \
        ObjectFactory_##name() = default;                                                   \
        virtual ~ObjectFactory_##name() = default;                                          \
        ::ReflectObject* newInstance() {                                                    \
            return new name();                                                              \
        }                                                                                   \
    };                                                                                      \
    class Register_##name {                                                                 \
      public:                                                                               \
        Register_##name() {                                                                 \
            ::Reflector::getReflector().registerFactory(#name, new ObjectFactory_##name()); \
        }                                                                                   \
    };                                                                                      \
    Register_##name register_##name;

#endif  // _REFLECTOR_H