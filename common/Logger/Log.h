#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdint>  
#include <cstring>  
#include <atomic>
#include <vector>

#include "String.h"

namespace Exchange::Core {

    enum class LogLevel : uint8_t {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    class LogHelper {
    public:
        static std::string levelToString(LogLevel level) {
            switch(level) {
                case LogLevel::TRACE: return "TRACE";
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO:  return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                case LogLevel::FATAL: return "FATAL";
            }
            return levelToString(LogLevel::INFO); // default
        }
    };

    class Log {
        std::tm mTimestamp;
        LogLevel mLevel;
        Core::String mMessage;
        Core::String mFile;
        Core::String mFunc;
        uint64_t mLine;
    
    public:

        /** @brief Constructor */
        Log(LogLevel level, const std::string& message, const std::string& file, const std::string& function, uint64_t line)
            :mLevel(level), mMessage(message), mFile(file), mFunc(function), mLine(line) {

            // Timestamp
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);

            #ifdef _WIN32
                localtime_s(&mTimestamp, &t);
            #else
                localtime_r(&t, &mTimestamp);
            #endif
        }
        
        Core::String toString() const {
            std::ostringstream oss;
            oss << "[" << std::put_time(&mTimestamp, "%Y-%m-%d %H:%M:%S") << "]"
                << "[" << LogHelper::levelToString(mLevel) << "]"
                << "[" << mFunc << "]" 
                << mFile << ":" << mLine << " - "
                << mMessage;
            return oss.str();
        }

        LogLevel getLevel() const {
            return mLevel;
        }
    };


}