#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
    static Logger &getInstance()
    {
        static Logger log;
        return log;
    }

    virtual ~Logger()
    {
        spdlog::drop(LOGGER_NAME_FILE.c_str());
        spdlog::drop(LOGGER_NAME_CONSOLE.c_str());
        asyncLogger = nullptr;
        consoleLogger = nullptr;
    }

    Logger()
    {
        spdlog::set_pattern("[%C:%m:%d-%H:%M:%S.%e][%s:%#:%!][tid:%t][%^%l%$] %v");
        spdlog::set_level(spdlog::level::debug); // Set global log level to debug
        //    spdlog::init_thread_pool(8 * 1024, 1); // queue with 8k items and 1 backing thread.
        spdlog::flush_every(std::chrono::seconds(5));

        asyncLogger = spdlog::daily_logger_mt<spdlog::async_factory>("camera_daily_async_logger", "logs/camera/daily.txt");
        asyncLogger->flush_on(spdlog::level::err);

        consoleLogger = spdlog::stdout_color_mt("camera_console");
    }
    std::shared_ptr<spdlog::logger> asyncLogger;
    std::shared_ptr<spdlog::logger> consoleLogger;

private:
    std::string LOGGER_NAME_FILE = "camera_daily_async_logger";
    std::string LOGGER_NAME_CONSOLE = "camera_console";
};

#define LOGI(...) {Logger::getInstance().consoleLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::info,  __VA_ARGS__);Logger::getInstance().asyncLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::info,  __VA_ARGS__);}
#define LOGD(...) {Logger::getInstance().consoleLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::debug,  __VA_ARGS__);Logger::getInstance().asyncLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::debug,  __VA_ARGS__);}
#define LOGW(...) {Logger::getInstance().consoleLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::warn,  __VA_ARGS__);Logger::getInstance().asyncLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::warn,  __VA_ARGS__);}
#define LOGE(...) {Logger::getInstance().consoleLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::err,  __VA_ARGS__);Logger::getInstance().asyncLogger->log(spdlog::source_loc{__FILE__, __LINE__, __func__},spdlog::level::level_enum::err,  __VA_ARGS__);}
