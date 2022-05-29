#include "Log.h"

namespace faker_tsn {

const uint8_t Log::INFO_LEVEL = Logger::INFO;
const uint8_t Log::DEBUG_LEVEL = Logger::DEBUG;
const uint8_t Log::WARN_LEVEL = Logger::WARN;
const uint8_t Log::ERROR_LEVEL = Logger::ERROR;
uint8_t Log::level = Logger::INFO;

Logger& Log::getLogger() {
    static DebugLogger m_logger;
    return m_logger;
}

void Log::setLoggerLevel(const uint8_t& level) {
    Log::level = level;
}

void Log::info(const char* message, const char* filename, int line) {
    if (Log::level <= Log::INFO_LEVEL) {
        Log::getLogger().log(Log::INFO_LEVEL, message, filename, line);
    }
}

void Log::info(const std::string& message, const char* filename, int line) {
    if (Log::level <= Log::INFO_LEVEL) {
        Log::getLogger().log(Log::INFO_LEVEL, message.c_str(), filename, line);
    }
}

void Log::debug(const char* message, const char* filename, int line) {
    if (Log::level <= Log::DEBUG_LEVEL) {
        Log::getLogger().log(Log::DEBUG_LEVEL, message, filename, line);
    }
}

void Log::debug(const std::string& message, const char* filename, int line) {
    if (Log::level <= Log::DEBUG_LEVEL) {
        Log::getLogger().log(Log::DEBUG_LEVEL, message.c_str(), filename, line);
    }
}

void Log::warn(const char* message, const char* filename, int line) {
    if (Log::level <= Log::WARN_LEVEL) {
        Log::getLogger().log(Log::WARN_LEVEL, message, filename, line);
    }
}

void Log::warn(const std::string& message, const char* filename, int line) {
    if (Log::level <= Log::WARN_LEVEL) {
        Log::getLogger().log(Log::WARN_LEVEL, message.c_str(), filename, line);
    }
}

void Log::error(const char* message, const char* filename, int line) {
    if (Log::level <= Log::ERROR_LEVEL) {
        Log::getLogger().log(Log::ERROR_LEVEL, message, filename, line);
    }
}

void Log::error(const std::string& message, const char* filename, int line) {
    if (Log::level <= Log::ERROR_LEVEL) {
        Log::getLogger().log(Log::ERROR_LEVEL, message.c_str(), filename, line);
    }
}

}  // namespace faker_tsn
