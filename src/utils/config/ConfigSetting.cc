#include "ConfigSetting.h"

ConfigSetting::ConfigSetting() : m_delimiter(std::string(1, '=')), m_commet(std::string(1, '#')) {
    std::cout << "请输入配置文件 config.ini 的绝对路径："; 
    std::cin >> this->m_filename;
    std::ifstream in(this->m_filename.c_str());
    if (!in) throw FileNotFoundException(this->m_filename.c_str());
    in >> (*this);
}

ConfigSetting::~ConfigSetting() {
}

void ConfigSetting::trim(std::string& inout_s) {
    static const char s_whiteSpaces[] = " \n\t\v\r\f";
    inout_s.erase(0, inout_s.find_first_not_of(s_whiteSpaces));
    inout_s.erase(inout_s.find_last_not_of(s_whiteSpaces) + 1U);
}

void ConfigSetting::remove(const std::string& key) {
    this->m_contents.erase(this->m_contents.find(key));
}