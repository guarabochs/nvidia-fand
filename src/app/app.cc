#include <app/app.hh>
#include <app/util.hh>

#include <string>
#include <ranges>
#include <iostream>
#include <cstdio>

namespace app {
    app::app(const program_arguments&& args) : launch_arguments { std::move(args) } {
        // program constructor
    }

    auto app::run() -> void {
        // program execution
        fmt::println("hi.");

        auto concat_args = launch_arguments
            | std::views::join_with(std::string { ", " });

        fmt::println("{}", std::string { concat_args.cbegin(), concat_args.cend() });

        // error handling
        auto err = error_from_std_exception(std::runtime_error { "ouch my leg" });
        scream_and_abort(std::move(err));
    }
}
