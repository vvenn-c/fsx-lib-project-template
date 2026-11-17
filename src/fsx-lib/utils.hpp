#pragma once

#include <print>
#include <string>
#include <utility>
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

class fsx::Utils {
    
public:

    Utils() = delete;

    template <typename... Args>
    static void log(std::format_string<Args...> fmt, Args&&... args) {
        std::println("[fsx-lib] {}", std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void showError(std::format_string<Args...> fmt, Args&&... args) {
        std::println(stderr, "[fsx-lib] error: {}", std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void showWarning(std::format_string<Args...> fmt, Args&&... args) {
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

};