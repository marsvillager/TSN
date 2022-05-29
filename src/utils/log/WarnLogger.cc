#include "WarnLogger.h"

namespace faker_tsn {

WarnLogger::WarnLogger(/* args */) {
    this->m_level = Logger::WARN;
    this->m_nextLogger.reset(new ErrorLogger());
}

WarnLogger::~WarnLogger() {
}

void WarnLogger::write(const char* message, const char* filename, int line) {
    std::cout << "[WARN] " << filename << ":" << line << ": " << message << std::endl;
}

}  // namespace faker_tsn
