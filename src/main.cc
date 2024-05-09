#include <app/app.hh>
#include <app/util.hh>

#include <exception>
#include <stacktrace>
#include <print>

#include <cxxabi.h>

auto global_exception_handler() -> void {
    // Emergency version of app::util::scream_and_abort
    const auto stacktrace = std::stacktrace::current(1);

#ifdef __GNUG__
    const auto current_exception_type = abi::__cxa_current_exception_type()->name();
    std::println("uncaught exception of type {}",
        app::util::cxx_demangle(current_exception_type)
    );
#else
    std::println("uncaught exception.");
#endif

    app::util::print_stacktrace(std::move(stacktrace));
    std::terminate();
}

auto main(int argc, char** argv) -> int {
    // Install exception handlers
    std::set_terminate(global_exception_handler);

    app::app application_inst { app::util::make_program_arguments(argc, argv) };
    application_inst.run();
}
