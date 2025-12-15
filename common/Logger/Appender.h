#pragma once
#include "Log.h"

namespace Exchange::Core {


    class IAppender {
    public:
        virtual ~IAppender() = default;
        virtual void append(const Log& log) = 0;
    };


    class ConsoleAppender : public IAppender {
        void append(const Log& log) override {
            std::printf(log.toString());
        }
    };

    class FileAppender : public IAppender {
        String path;
        String name;
    public:
        /** @brief Constructor */
        FileAppender(String path, String name) 
            : path(std::move(path)), name(std::move(name)) {}
        
        void append(const Log& log) override {
            // todo
        }
    };


    class DatabaseAppender : public IAppender {
        Core::String url;
        Core::String dbName;
        Core::String password;
        Core::String tableName;
    public:
        void append(const Log& log) override {
            // todo
        }
    };
}