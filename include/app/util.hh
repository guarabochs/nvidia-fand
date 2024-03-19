#pragma once

#include <vector>
#include <string_view>

namespace app {
    using program_arguments = const std::vector<std::string_view>;

    constexpr inline auto make_program_arguments(const int argc, const char* const* argv) -> program_arguments {
        return program_arguments { argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)) };
    }
}
