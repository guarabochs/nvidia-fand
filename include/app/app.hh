#pragma once

#include <fmt/std.h>

#include <app/util.hh>

namespace app {
    class app final {
        using consolidated_program_arguments = const std::vector<std::string_view>;

        private:
            const consolidated_program_arguments launch_arguments;

        public:
            app(program_arguments&& arguments);
            auto run() -> void;
    };
}
