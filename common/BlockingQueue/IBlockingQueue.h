#pragma once

#include <cstddef>
#include <memory>


namespace Exchange::Core {

    /*
    * @class IBlockingQueue
    * @brief 
    * A thread-safe blocking queue with a fixed capacity. It automates synchronization 
    * between producer and consumer threads.
    */
    template <typename T>
    class IBlockingQueue {
    public:
        virtual ~IBlockingQueue() = default;

        // blocking
        virtual void push(const T& value) = 0;
        virtual void push(T&& value) = 0;
        virtual T pop() = 0;
        virtual bool pop(T& out) = 0;

        // lifecycle
        virtual void close() = 0;
        virtual bool isClosed() const = 0;
    };
}