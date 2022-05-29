#include "ErrorLogger.h"

namespace faker_tsn {

ErrorLogger::ErrorLogger(/* args */) {
    this->m_level = Logger::ERROR;
    this->m_nextLogger = nullptr;
}

ErrorLogger::~ErrorLogger() {
}

void ErrorLogger::write(const char* message, const char* filename, int line) {
    std::cout << "[ERROR] " << filename << ":" << line << ": " << message << std::endl;
}

}  // namespace faker_tsn
