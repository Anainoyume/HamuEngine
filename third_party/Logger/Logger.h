#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <optional>

namespace hamu
{

enum LogLevel
{
    TRACE,
    DEBUF,
    INFO,
    WARN,
    ERROR,
    FATAL,
    OFF
};

struct LogOptions
{
    LogLevel log_level = INFO;
};

class Logger
{
private:
    struct Buffer;

public:
    Logger(const std::string& filename);
    ~Logger();

    void log(const std::string& message);

private:
    void workFunction();

    void flushToFile(Buffer* buffer);

    Buffer* getBuffer();
    void returnBuffer(Buffer* buffer);

    Buffer* generateNewBuffer();

    void setLogOptions(LogOptions options) {
        log_optios = options;
    }

private:
    struct Buffer
    {
        std::vector<std::string> line;
        void clear() {
            line.clear();
        }
    };

    template <typename T, typename Mutex = std::mutex>
    class SafeQueue
    {
    public:
        bool empty() const {
            std::lock_guard<Mutex> lock(mtx);
            return que.empty();
        }

        std::optional<T> try_pop() {
            std::lock_guard<Mutex> lock(mtx);
            if (que.empty()) return std::nullopt;

            auto front = std::move(que.front());
            que.pop();
            return front;
        }

        std::optional<T> wait_and_pop(std::atomic<bool>& terminated_flag, bool reverse = false) {
            std::unique_lock<Mutex> lock(mtx);
            cv.wait(lock, [&] {
                return !que.empty() || (terminated_flag.load() ^ reverse) || terminated;
            });

            if (que.empty()) {
                return std::nullopt;
            }

            auto front = std::move(que.front());
            que.pop();
            return front;
        }

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U&&, T>>>
        void push(U&& element) {
            {
                std::lock_guard<Mutex> lock(mtx);
                que.push(std::forward<U>(element));
            }
            cv.notify_one();
        }

        void terminate() {
            {
                std::lock_guard<Mutex> lock(mtx);
                terminated = true;
            }
            cv.notify_all();
        }

        size_t size() const {
            std::lock_guard<Mutex> lock(mtx);
            return que.size();
        }

    private:
        mutable Mutex mtx;
        std::condition_variable cv;
        std::queue<T> que;
        bool terminated = false;
    };

private:
    std::atomic<bool> thread_started;
    std::atomic<bool> is_running;
    std::ofstream log_file;

    SafeQueue<Buffer*> free_queue;
    SafeQueue<Buffer*> work_queue;
    Buffer* current_buffer;

    std::mutex check_mutex;

    std::condition_variable cv;
    std::thread worker_thread;
    size_t buffer_limit = 500;
    size_t init_buffer_cache = 10;

    LogOptions log_optios;
};

} // namespace hamu
