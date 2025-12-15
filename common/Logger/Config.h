#pragma once

#include <map>
#include <vector>
#include "Appender.h"
#include "Filter.h"

namespace Exchange::Core {

    class Config {
        std::map<uint16_t, IAppender*> mAppenders;
        std::map<uint16_t, std::vector<IFilter*>> mFilters;
    public:
        Config() = default;
        void addAppender(IAppender* appender) {
            mAppenders.insert({mAppenders.size(),std::move(appender)});
        }
        void addAppender(IAppender* appender, IFilter* filter) {
            mAppenders.insert({mAppenders.size(),std::move(appender)});
            mFilters.insert({mAppenders.size()-1, std::vector<IFilter*>{filter}});
        }
        void addAppender(IAppender* appender, std::vector<IFilter*> filters) {
            mAppenders.insert({mAppenders.size(),std::move(appender)});
            mFilters.insert({mAppenders.size()-1, filters});
        }
    };
    
}