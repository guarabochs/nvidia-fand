#include <app/app.hh>
#include <app/util.hh>

#include <string>
#include <ranges>

namespace app {
    app::app(program_arguments&& args) {
        launch_arguments = auto { std::move(args) };
    }

    auto app::hello_world() -> void {
        fmt::println("hi.");

        auto concat_args = launch_arguments
            | std::views::join_with(std::string { ", " });

        fmt::println("{}", std::string { concat_args.cbegin(), concat_args.cend() });
    }
}
