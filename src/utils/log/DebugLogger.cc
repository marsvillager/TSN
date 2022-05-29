#include "DebugLogger.h"

namespace faker_tsn {

DebugLogger::DebugLogger(/* args */) {
    this->m_level = Logger::DEBUG;
    this->m_nextLogger.reset(new InfoLogger());
}

DebugLogger::~DebugLogger() {
}

void DebugLogger::write(const char* message, const char* filename, int line) {
    std::cout << "[DEBUG] " << filename << ":" << line << ": " << message << std::endl;
}

}  // namespace faker_tsn
