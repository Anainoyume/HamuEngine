#include "Logger/Logger.h"
#include <atomic>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>

namespace hamu
{


Logger::Logger(const std::string& filename)
    : thread_started(false), is_running(true), log_file(filename, std::ios::app) {

    for (size_t i = 0; i < init_buffer_cache; i++) {
        auto buffer = new Buffer;
        buffer->line.reserve(buffer_limit);
        free_queue.push(buffer);
    }
    current_buffer = getBuffer();

    worker_thread = std::thread(&Logger::workFunction, this);

    // 注意线程实际启动是有时间的, 如果主线程中运行速度过快
    // 可能导致这里线程还没启动完, 直接就调用析构函数了, 所以我们在构造函数一定要确保线程真正启动了,
    // 用一个标志就可以
    while (!thread_started) {
        std::this_thread::yield(); // 让出 CPU, 让其他同优先级的线程跑一会儿
    }
}


Logger::~Logger() {
    is_running = false;
    work_queue.terminate();

    if (worker_thread.joinable()) {
        worker_thread.join();
    }

    while (auto optBuf = free_queue.try_pop()) {
        delete *optBuf;
    }

    log_file.close();
}


void Logger::log(const std::string& message) {
    if (current_buffer == nullptr) return;

    size_t buffer_size = current_buffer->line.size();
    if (buffer_size >= buffer_limit) {
        work_queue.push(current_buffer);

        current_buffer = getBuffer();
        if (current_buffer == nullptr) {
            // 拿一个新的 Buffer, 并进行监控
            current_buffer = generateNewBuffer();
        }
    }
    current_buffer->line.emplace_back("[Info] " + message);
}


void Logger::workFunction() {
    thread_started = true;

    while (is_running) {
        auto optBuf = work_queue.wait_and_pop(is_running, true);
        if (!optBuf.has_value()) {
            break;
        }
        
        Buffer* buffer = *optBuf;

        flushToFile(buffer);
        buffer->clear();

        returnBuffer(buffer);
    }

    std::cout << "当前 Buffer 数量: "
              << free_queue.size() + work_queue.size() + (current_buffer != nullptr) << std::endl;

    if (current_buffer != nullptr && !current_buffer->line.empty()) {
        work_queue.push(current_buffer);
        current_buffer = nullptr;
    }

    while (!work_queue.empty()) {
        Buffer* buffer = work_queue.try_pop().value();

        flushToFile(buffer);
        buffer->clear();

        returnBuffer(buffer);
    }
}


Logger::Buffer* Logger::generateNewBuffer() {
    Buffer* buffer = new Buffer;
    buffer->line.reserve(init_buffer_cache);
    return buffer;
}


Logger::Buffer* Logger::getBuffer() {
    if (free_queue.empty()) return nullptr;
    auto optBuf = free_queue.try_pop();
    if (!optBuf.has_value()) {
        // ...
        return nullptr;
    }
    Buffer* buffer = *optBuf;

    return buffer;
}


void Logger::returnBuffer(Buffer* buffer) {
    free_queue.push(buffer);
}


void Logger::flushToFile(Buffer* buffer) {
    if (buffer == nullptr) return;

    for (const auto& log_text : buffer->line) {
        log_file << log_text << '\n';
    }

    log_file.flush();
    buffer->clear();
}


} // namespace hamu