#pragma once

#include <fmt/std.h>

#include <app/util.hh>

namespace app {
    namespace {
        using consolidated_program_arguments = std::vector<std::string_view>;
    };

    class app final {
        private:
            consolidated_program_arguments launch_arguments;

        public:
            app(program_arguments&& arguments);
            auto hello_world() -> void;
    };
}
