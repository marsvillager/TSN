#ifndef _CONFIG_SETTING_H
#define _CONFIG_SETTING_H

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class KeyNotFoundException : public std::exception {
  private:
    const std::string& m_key;

  public:
    KeyNotFoundException(const std::string& key)
        : m_key(key) {
    }

    const char* what() noexcept {
        std::string _error;
        _error = "Key [" + std::string(this->m_key) + "] Not Found";
        return _error.c_str();
    }
};

class FileNotFoundException : public std::exception {
  private:
    const std::string m_filename;

  public:
    FileNotFoundException(const std::string filename)
        : m_filename(filename) {}

    const char* what() noexcept {
        std::string _error;
        _error = "File [" + std::string(this->m_filename) + "] Not Found";
        return _error.c_str();
    }
};

class ConfigSetting {
  private:
    std::string m_delimiter;
    std::string m_commet;
    std::string m_filename;
    std::unordered_map<std::string, std::string> m_contents;

    typedef std::unordered_map<std::string, std::string>::iterator mapi;
    typedef std::unordered_map<std::string, std::string>::const_iterator mapci;

    ConfigSetting();

  protected:
    /* cast string to T */
    template <class T>
    static T string_as_T(const std::string& key);

    /* cast T to string */
    template <class T>
    static std::string T_as_string(const T& t);

    /* earse white spaces */
    static void trim(std::string& inout_s);

  public:
    ~ConfigSetting();

    inline static ConfigSetting& getInstance() {
        static ConfigSetting configSetting;
        return configSetting;
    }

    /* search for key and get value, call as get<T>(key) */
    template <class T>
    T get(const std::string& key) const;

    /* search for key and get value or optional default value, call as get<T>(key) */
    template <class T>
    T get(const std::string& key, const T& value) const;

    /* set value by key */
    template <class T>
    void set(const std::string& key, const T& vaule);

    /* remove key-value pair */
    void remove(const std::string& key);

    /* output config items */
    friend std::ostream& operator<<(std::ostream& os, const ConfigSetting& cs) {
        for (auto it = cs.m_contents.begin(); it != cs.m_contents.end(); it++) {
            os << it->first << " " << cs.m_delimiter << " ";
            os << it->second << std::endl;
        }
        return os;
    }

    /* input config items */
    friend std::istream& operator>>(std::istream& is, ConfigSetting& cs) {
        auto delimiteLength = cs.m_delimiter.size();
        std::string newLine = "";

        while (is || newLine.size() > 0) {
            /* get entire line */
            std::string line;
            if (newLine.size() > 0) {
                line = newLine;
                newLine = "";
            } else {
                std::getline(is, line);
            }
            /* ignore comment */
            line = line.substr(0, line.find(cs.m_commet));
            /* parse the line */
            auto delimiterPos = line.find(cs.m_delimiter);
            if (delimiterPos < std::string::npos) {
                /* extract key */
                std::string key = line.substr(0, delimiterPos);
                line.replace(0, delimiterPos + delimiteLength, "");
                /* store key and value */
                ConfigSetting::trim(key);
                ConfigSetting::trim(line);
                cs.m_contents[key] = line;
            }
        }

        return is;
    }
};

template <class T>
T ConfigSetting::string_as_T(const std::string& key) {
    // Convert from a string to a T
    // Type T must support >> operator
    T t;
    std::istringstream ist(key);
    ist >> t;
    return t;
}

template <>
inline std::string ConfigSetting::string_as_T(const std::string& key) {
    // do nothing
    return key;
}

template <>
inline const char* ConfigSetting::string_as_T(const std::string& key) {
    // do nothing
    return key.c_str();
}

template <>
inline bool ConfigSetting::string_as_T<bool>(const std::string& s) {
    // Convert from a string to a bool
    // Interpret "false", "F", "no", "n", "0" as false
    // Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
    bool b = true;
    std::string sup = s;
    for (std::string::iterator p = sup.begin(); p != sup.end(); ++p)
        *p = toupper(*p);  // make string all caps
    if (sup == std::string("FALSE") || sup == std::string("F") ||
        sup == std::string("NO") || sup == std::string("N") ||
        sup == std::string("0") || sup == std::string("NONE") ||
        sup == std::string("false") || sup == std::string("f"))
        b = false;
    return b;
}

template <class T>
std::string ConfigSetting::T_as_string(const T& t) {
    // Convert from a T to a string
    // Type T must support << operator
    std::ostringstream ost;
    ost << t;
    return ost.str();
}

template <class T>
T ConfigSetting::get(const std::string& key) const {
    auto it = this->m_contents.find(key);
    if (it == this->m_contents.end())
        throw KeyNotFoundException(key);
    return string_as_T<T>(it->second);
}

/* search for key and get value or optional default value, call as get<T>(key) */
template <class T>
T ConfigSetting::get(const std::string& key, const T& value) const {
    auto it = this->m_contents.find(key);
    if (it == this->m_contents.end())
        return value;
    return string_as_T<T>(it->second);
}

/* set value by key */
template <class T>
void ConfigSetting::set(const std::string& key, const T& vaule) {
    std::string _v = T_as_string(vaule);
    std::string _k = key;
    ConfigSetting::trim(_k);
    ConfigSetting::trim(_v);
    this->m_contents[_k] = _v;
}

#endif  // _CONFIG_SETTING_H