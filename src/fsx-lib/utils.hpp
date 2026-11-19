#pragma once

#include <print>
#include <string>
#include <utility>
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

class fsx::Utils {
    
public:

    enum class LogLevel {
        NORMAL,
        ERROR_ONLY,
        VERBOSE
    };

    static void setLogLevel(LogLevel level) { logLevel_ = level; }

    template <typename... Args>
    static void log(std::format_string<Args...> fmt, Args&&... args) {
        if (logLevel_ >= LogLevel::VERBOSE)
            std::println("[fsx-lib] {}", std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void showError(std::format_string<Args...> fmt, Args&&... args) {
        if (logLevel_ == LogLevel::NORMAL || logLevel_ >= LogLevel::ERROR_ONLY)
            std::println(stderr, "[fsx-lib] error: {}", std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void showWarning(std::format_string<Args...> fmt, Args&&... args) {
        if (logLevel_ == LogLevel::NORMAL || logLevel_ >= LogLevel::VERBOSE)
        std::println(stderr, "[fsx-lib] warning: {}", std::format(fmt, std::forward<Args>(args)...));
    }

    static VkResult checkResult(VkResult result, std::string message = "") {

        if (result < 0) {
            showError(
                "a vulkan function returned negative status code: {} | {}", 
                string_VkResult(result),
                message
            );
        }

        if (result > 0) {
            showWarning(
                "a vulkan function returned positive status code: {}", 
                string_VkResult(result)
            );
        }

        return result;

    }

    static void toggle(bool *thing) {
        if (*thing) *thing = false;
        else *thing = true;
    }

private: 

    Utils() = delete;
    static inline LogLevel logLevel_ = LogLevel::NORMAL;

};