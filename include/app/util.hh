#pragma once

#include <vector>
#include <stacktrace>
#include <string_view>
#include <optional>
#include <ranges>
#include <cstdio>

#include <fmt/std.h>

namespace app {
    template<typename T>
    concept good_exception = std::derived_from<T, std::exception>;

    template<good_exception T>
    struct program_error {
        const std::remove_cvref_t<T> exception;
        const std::stacktrace stack;
    };

    using program_arguments = const std::vector<std::string_view>;
    using general_error = const program_error<std::runtime_error>;

    template<good_exception T>
    [[noreturn]] constexpr inline auto scream_and_abort(const program_error<T>&& error) -> void {
        fmt::println("scream_and_die: called with exception of type {}", typeid(error.exception).name());
        fmt::println("  what(): {}", error.exception.what());
        fmt::println("  stack:");

        for (const auto [i, entry] : error.stack | std::views::enumerate) {
            fmt::println("    #{} -> @ {}:{} ({})",
                (error.stack.size() - i),
                entry.description(), entry.source_line(), entry.source_file()
            );
        }

        std::abort();
    }

    constexpr inline auto error_from_std_exception(good_exception auto&& exception) -> const program_error<std::remove_cvref_t<decltype(exception)>> {
        return {
            std::move(exception),
            std::move(std::stacktrace::current(1))
        };
    }

    inline auto make_program_arguments(const int argc, const char* const* argv) -> program_arguments {
        return program_arguments { argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)) };
    }

    inline auto environment_variable(const std::string variable) -> std::optional<const std::string_view> {
        if (const char* result = std::getenv(variable.data())) {
            return { result };
        }

        return std::nullopt;
    }
}
