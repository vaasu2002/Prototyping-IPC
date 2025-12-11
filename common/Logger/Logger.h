// Logger.h
#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdint>  
#include <cstring>  
#include <unordered_map>
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
        
        Core::String toString(){
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

    class Logger {
        // helper: format with va_list
        static std::string vformat(const char* fmt, va_list args) {
            if (!fmt) return std::string();
            va_list argsCopy;
            va_copy(argsCopy, args);
            int len = std::vsnprintf(nullptr, 0, fmt, argsCopy);
            va_end(argsCopy);
            if (len < 0) return std::string();

            std::string result;
            result.resize(static_cast<size_t>(len));
            std::vsnprintf(result.data(), result.size() + 1, fmt, args);
            return result;
        }

        // convenience wrapper
        static std::string format(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string s = vformat(fmt, args);
            va_end(args);
            return s;
        }
    public:

        // static void log(LogLevel level, const std::string& msg, const char* file, const char* func, uint64_t line) {
        //     Log log(level,msg,file,func,line);
        //     Core::String str = log.toString();
        //     std::cout << str.toString() << std::endl;
        // }

        static void log(LogLevel level, const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);

            Log log(level, msg, file ? file : "", func ? func : "", line);
            Core::String str = log.toString();
            std::cout << str.toString() << std::endl;
        }

        static void debug(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::DEBUG, file, func, line, "%s", msg.c_str());
        }
        static void trace(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::TRACE, file, func, line, "%s", msg.c_str());
        }
        static void info(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::INFO, file, func, line, "%s", msg.c_str());
        }
        static void warn(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::WARNING, file, func, line, "%s", msg.c_str());
        }
        static void error(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::ERROR, file, func, line, "%s", msg.c_str());
        }
        static void fatal(const char* file, const char* func, uint64_t line, const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            std::string msg = vformat(fmt, args);
            va_end(args);
            log(LogLevel::FATAL, file, func, line, "%s", msg.c_str());
        }
    };

}

#define LOG_DEBUG(...) Exchange::Core::Logger::debug(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_TRACE(...) Exchange::Core::Logger::trace(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  Exchange::Core::Logger::info(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  Exchange::Core::Logger::warn(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Exchange::Core::Logger::error(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) Exchange::Core::Logger::fatal(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)