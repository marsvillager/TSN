#include "Logger.h"

namespace faker_tsn {

const uint8_t Logger::DEBUG = 1;
const uint8_t Logger::INFO = 2;
const uint8_t Logger::WARN = 3;
const uint8_t Logger::ERROR = 4;

Logger::Logger(/* args */) {
}

Logger::~Logger() {
}

void Logger::log(uint8_t level, const char* message, const char* filename, int line) {
    if (this->m_level >= level) {
        this->write(message, filename, line);
        return;
    }
    if (m_nextLogger != nullptr) {
        this->m_nextLogger->log(level, message, filename, line);
    }
}

}  // namespace faker_tsn
