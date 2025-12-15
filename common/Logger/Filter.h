#pragma once

#include "Log.h"


namespace Exchange::Core {

    class IFilter {
    public:
        virtual ~IFilter() = default;
        virtual bool filter(const Log& log) = 0;
    };


    class LevelFilter : public IFilter {
        LogLevel minLevel;
    public:
        /** @brief Constructor */
        LevelFilter(LogLevel minLevel): minLevel(minLevel) {}
        bool filter(const Log& log) override {
            return log.getLevel() > minLevel;
        }
    };
};
