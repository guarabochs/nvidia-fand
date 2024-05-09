#include <app/app.hh>
#include <app/util.hh>

#include <string>
#include <ranges>
#include <print>

namespace app {
    app::app(const program_arguments&& args) : launch_arguments { std::move(args) } {
        // program constructor
    }

    auto app::run() -> void {
        // program execution
        std::println("hi.");

        auto concat_args = launch_arguments
            | std::views::join_with(std::string { ", " })
            | std::ranges::to<std::string>();

        std::println("{}", concat_args);

        // error handling
        auto err = error_from_std_exception(std::runtime_error { "ouch my leg" });
        scream_and_abort(std::move(err));
    }
}
