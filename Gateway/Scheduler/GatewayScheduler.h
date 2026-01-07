#pragma once

#include "Scheduler.h"
#include "String.h"
#include "Network/TcpEpollListener.h"
#include "FixMessageDispatcher.h"

namespace Exchange::Gateway {

    enum Threads {
        Listener,
        Dispatcher
    };

    class GatewayScheduler : public Scheduler {
        Core::String mWorkerPrefix;
        size_t mWorkerCnt;
        std::map<Threads, std::string> mThreads;
        std::shared_ptr<std::atomic<bool>> mStopNetwork;

    public:

        /**
         * @brief Constructor
         * @details
         * Initializes the scheduler with a given name prefix. Creates and starts two dedicated worker threads:
         * - Listener thread: responsible for receiving and accepting incoming connections or messages.
         * - Dispatcher thread: responsible for processing and routing received data to appropriate handlers.
         * Also initializes a shared atomic flag used to signal shutdown to both worker threads.
         * 
         * @param prefix Base name prefix for the worker threads (e.g., "gateway").
         *               Thread names will be formed as "{prefix}_listener" and "{prefix}_dispatcher".
         */
        GatewayScheduler(const Core::String prefix) : mWorkerPrefix(std::move(prefix)),
            mStopNetwork(std::make_shared<std::atomic<bool>>(false))
        {
            // Register human-readable names for the two main worker threads
            mThreads.insert({Threads::Listener, (mWorkerPrefix + "_listener").toString()});
            mThreads.insert({Threads::Dispatcher, (mWorkerPrefix + "_dispatcher").toString()});

            // Spawn the listener and dispatcher workers
            createWorker(mThreads[Threads::Listener]);
            createWorker(mThreads[Threads::Dispatcher]);
        }

        /**
         * @brief Initializes and launches the core Gateway processing loops.
         * This function orchestrates the startup of the Gateway by offloading the blocking network listener
         * and the message dispatcher to dedicated worker threads.
         * @param listener   The TCP Epoll handler responsible for accepting client connections.
         * @param dispatcher The FIX message handler that routes validated requests to the sequencer.
         */
        void start(Network::TcpEpollListener& listener, Exchange::Gateway::FixMessageDispatcher& dispatcher) {
            LOG_INFO("Starting Gateway Scheduler workers...");

            // Start the worker threads
            Scheduler::start();

            auto stopNet = mStopNetwork;
            
            // Offload the blocking Epoll loop to a specific thread designated for network I/O.
            submitTo(
                mThreads[Threads::Listener], 
                [stopNet, &listener](const CancelToken& token) {
                    // listener.run() is an event loop that blocks until stopNet signals an exit.
                    listener.run(stopNet.get());
                }, 
                "This thread listens to network request from clients."
            );
            
            // Offload the FIX message processing loop to a separate thread.
            // This separates "receiving bytes" from "processing business logic" (decoupling).
            submitTo(
                mThreads[Threads::Dispatcher], 
                [&dispatcher](const CancelToken& token) {
                    // dispatcher.run() processes the incoming FIX message queue and forwards
                    // them to the matching engine/sequencer.
                    dispatcher.run();
                }, 
                "This thread dispatches the valid requests to sequence process"
            );
            
            LOG_INFO("Gateway loops submitted to workers");
        }

        void shutdown() {
            LOG_INFO("Initiating Gateway shutdown...");
            mStopNetwork->store(true, std::memory_order_release);
            LOG_INFO("Network stop signal sent, waiting for clean shutdown...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            Scheduler::shutdown();
            LOG_INFO("Gateway shutdown complete");
        }
    };
}