#pragma once

#include <vector>
#include <stacktrace>
#include <string_view>
#include <optional>
#include <ranges>
#include <memory>
#include <print>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

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

    namespace util {
        inline auto make_program_arguments(const int argc, const char* const* argv) -> program_arguments {
            return program_arguments { argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)) };
        }

        inline auto environment_variable(const std::string variable) -> std::optional<const std::string_view> {
            if (const char* result = std::getenv(variable.data())) {
                return { result };
            }

            return std::nullopt;
        }

        inline auto cxx_demangle(const char* input) -> const std::string {
#ifdef __GNUG__
            int cxa_demangle_result { -1 };
            std::unique_ptr<char, void(*)(void*)> demangled_name {
                abi::__cxa_demangle(input, nullptr, nullptr, &cxa_demangle_result),
                std::free
            };

            return cxa_demangle_result == 0 ? demangled_name.get() : input;
#else
            return input;
#endif
        }

        inline auto print_stacktrace(const std::stacktrace&& stacktrace) -> void {
            std::println("  stack:");
            for (const auto [i, entry] : stacktrace | std::views::enumerate) {
                std::println("    #{} -> @ {}:{} ({})",
                    (stacktrace.size() - i),
                    entry.description(), entry.source_line(), entry.source_file()
                );
            }
        }
    }

    template<good_exception T>
    [[noreturn]] constexpr inline auto scream_and_abort(const program_error<T>&& error) -> void {
        std::println("scream_and_die: called with exception of type {}",
            util::cxx_demangle(typeid(error.exception).name())
        );

        std::println("  what(): {}", error.exception.what());
        util::print_stacktrace(std::move(error.stack));

        std::abort();
    }

    constexpr inline auto error_from_std_exception(good_exception auto&& exception) -> const program_error<std::remove_cvref_t<decltype(exception)>> {
        return {
            std::move(exception),
            std::move(std::stacktrace::current())
        };
    }
}
