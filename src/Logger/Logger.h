#pragma once
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fmt/base.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/chrono.h>

#define LAMBDA_ASSERT(condition) do{\
    if(!condition){\
        LAMBDA_LOG(LogLevel::CRITICAL, "Assertion {} failed!", #condition);\
        exit(-1);\
    }\
} while(false);

#define LAMBDA_LOG(level, fmt, ...) do {log_message(level, fmt __VA_OPT__(,) __VA_ARGS__);} while(false);

namespace Lambda{
    enum class LogLevel: uint8_t{
        INFO,
        WARNING,
        DEBUG,
        ERROR,
        CRITICAL
    };

    inline const std::string LogLeveltoString(LogLevel level){
        switch(level){
            case Lambda::LogLevel::INFO:
                return "INFO";
                break;
            case Lambda::LogLevel::WARNING:
                return "WARNING";
                break;
            case Lambda::LogLevel::DEBUG:
                return "DEBUG";
                break;
            case Lambda::LogLevel::ERROR:
                return "ERROR";
                break;
            case Lambda::LogLevel::CRITICAL:
                return "CRITICAL";
                break;
            default:
                return "ASAMBHAV!!";
                break;
        }
    }

    template<typename... Args>
    inline void log_message(LogLevel level, const std::string& fmt, Args&&... args){
        try{
            std::string message = fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...));
            auto now = std::chrono::system_clock::now();
            fmt::print("[{:%d/%m/%Y %H:%M:%S}] ", now);
            std::cout<<"["<<LogLeveltoString(level)<<"]: ";
            std::cout<<message<<std::endl;
        }
        catch(const fmt::format_error& e){
            std::cerr<<"Formatting error: "<<e.what()<<std::endl;
        }
    }
}