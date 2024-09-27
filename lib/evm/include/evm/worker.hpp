#pragma once

#include "macro_definition.hpp"

#include <atomic>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace evm {

    class EXPORT_EVM Void {};

    template<class D>
    class EXPORT_EVM WorkingSet {

    public: // TODO: Hide
        std::condition_variable _conditionVariable;
        std::mutex _mutex;

        void insert(D data) {

            _mutex.lock();
            _queue.emplace(data);
            _mutex.unlock();
            _conditionVariable.notify_one();
        }

        void insert(std::vector<D> data) {

            _mutex.lock();
            for (auto iter = data.begin(); iter != data.end(); iter++) {
                _queue.push(*iter);
            }
            _mutex.unlock();
            _conditionVariable.notify_one();
        }

        D front() {

            if (_queue.empty()) {
                return D{};
            }

            auto front = _queue.front();
            _queue.pop();
            return front;
        }

        void notify() {

            _conditionVariable.notify_all();
        }

        bool hasData() {

            return !_queue.empty();
        }

        bool isDone() {

            return done;
        }

        void setDone() {

            done = true;
            notify();
        }

    private:
        std::queue<D> _queue;
        bool done = false;
    };

    template<class D>
    class EXPORT_EVM Worker {

    public:
        explicit Worker(std::shared_ptr<WorkingSet<D>> data) :
                _running(true),
                _thread(&Worker::execute, this, std::move(data), std::ref(_running)) {
        }

        ~Worker() {

            stop();
            join();
        }

        void join() {

            if (_thread.joinable()) {
                _thread.join();
            }
        }

        void stop() {

            _running = false;
        }

    protected:
        virtual void execute(std::shared_ptr<WorkingSet<D>> data, std::atomic<bool>& running) = 0;

    private:
        std::atomic<bool> _running;
        std::thread _thread;
    };
}