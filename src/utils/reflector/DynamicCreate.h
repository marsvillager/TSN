#pragma once

#include <cxxabi.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace faker_tsn {

/* REFLECT_OBJECT */
class REFLECT_OBJECT {
  public:
    REFLECT_OBJECT() { INFO("Construct REFLECT_OBJECT"); }

    virtual ~REFLECT_OBJECT() = default;

    virtual void Say() {
        std::cout << "REFLECT_OBJECT" << std::endl;
    }
};

/* REFLECT_OBJECTFactory */
template <typename... Targs>
class REFLECT_OBJECTFactory {
  public:
    static REFLECT_OBJECTFactory* Instance() {
        if (nullptr == s_pREFLECT_OBJECTFactory) {
            s_pREFLECT_OBJECTFactory = new REFLECT_OBJECTFactory();
        }
        return (s_pREFLECT_OBJECTFactory);
    }

    virtual ~REFLECT_OBJECTFactory() = default;

    bool Regist(const std::string& strTypeName, std::function<REFLECT_OBJECT*(Targs&&... args)> pFunc);

    bool UnRegist(const std::string& strTypeName);

    REFLECT_OBJECT* Create(const std::string& strTypeName, Targs&&... args);

  private:
    REFLECT_OBJECTFactory() = default;
    static REFLECT_OBJECTFactory<Targs...>* s_pREFLECT_OBJECTFactory;
    std::unordered_map<std::string, std::function<REFLECT_OBJECT*(Targs&&...)> > m_mapCreateFunction;
};

template <typename... Targs>
REFLECT_OBJECTFactory<Targs...>* REFLECT_OBJECTFactory<Targs...>::s_pREFLECT_OBJECTFactory = nullptr;

template <typename... Targs>
REFLECT_OBJECT* REFLECT_OBJECTFactory<Targs...>::Create(const std::string& strTypeName, Targs&&... args) {
    auto iter = m_mapCreateFunction.find(strTypeName);
    if (iter == m_mapCreateFunction.end()) {
        return (nullptr);
    } else {
        return (iter->second(std::forward<Targs>(args)...));
    }
}

template <typename... Targs>
bool REFLECT_OBJECTFactory<Targs...>::Regist(const std::string& strTypeName, std::function<REFLECT_OBJECT*(Targs&&... args)> pFunc) {
    if (nullptr == pFunc) {
        return (false);
    }
    bool bReg = m_mapCreateFunction.insert(std::make_pair(strTypeName, pFunc)).second;
    return (bReg);
}

template <typename... Targs>
bool REFLECT_OBJECTFactory<Targs...>::UnRegist(const std::string& strTypeName) {
    auto iter = m_mapCreateFunction.find(strTypeName);
    if (iter == m_mapCreateFunction.end()) {
        return (false);
    } else {
        m_mapCreateFunction.erase(iter);
        return (true);
    }
}

/* DynamicCreator */
template <typename T, typename... Targs>
class DynamicCreator {
  public:
    struct Register {
        Register() {
            char* szDemangleName = nullptr;
            std::string strTypeName;
#ifdef __GNUC__
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
#else
            // in this format?:     szDemangleName =  typeid(T).name();
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
#endif
            if (nullptr != szDemangleName) {
                strTypeName = szDemangleName;
                free(szDemangleName);
            }
            REFLECT_OBJECTFactory<Targs...>::Instance()->Regist(strTypeName, CreateObject);
        }

        ~Register() {
            char* szDemangleName = NULL;
            std::string strTypeName;
#ifdef __GNUC__
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
#else
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
#endif
            if (NULL != szDemangleName) {
                strTypeName = szDemangleName;
                free(szDemangleName);
            }
            REFLECT_OBJECTFactory<Targs...>::Instance()->UnRegist(strTypeName);
        }

        inline void do_nothing() const {};
    };

    DynamicCreator() {
        s_oRegister.do_nothing();
    }

    virtual ~DynamicCreator() = default;

    static T* CreateObject(Targs&&... args) {
        T* pT = nullptr;
        try {
            pT = new T(std::forward<Targs>(args)...);
        } catch (std::bad_alloc& e) {
            // std::cerr << e.what() << std::endl;     // TODO write log
            return (nullptr);
        }
        return (pT);
        // return new T(std::forward<Targs>(args)...);
    }

  private:
    static Register s_oRegister;
};

template <typename T, typename... Targs>
typename DynamicCreator<T, Targs...>::Register DynamicCreator<T, Targs...>::s_oRegister;

/* Reflector */
class REFLECTOR {
  public:
    template <typename... Targs>
    static REFLECT_OBJECT* CreateByTypeName(const std::string& strTypeName, Targs&&... args) {
        REFLECT_OBJECT* p = REFLECT_OBJECTFactory<Targs...>::Instance()->Create(strTypeName, std::forward<Targs>(args)...);
        return (p);
    }
};

}  // namespace faker_tsn