#include "InfoLogger.h"

namespace faker_tsn {

InfoLogger::InfoLogger(/* args */) {
    this->m_level = Logger::INFO;
    this->m_nextLogger.reset(new WarnLogger());
}

InfoLogger::~InfoLogger() {
}

void InfoLogger::write(const char* message, const char* filename, int line) {
    std::cout << "[INFO] " << filename << ":" << line << ": " << message << std::endl;
}

}  // namespace faker_tsn
