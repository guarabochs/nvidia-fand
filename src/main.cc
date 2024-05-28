#include <app/app.hh>
#include <app/util.hh>

#include <exception>
#include <stacktrace>
#include <print>

#include <cxxabi.h>

[[noreturn]] auto global_exception_handler() -> void {
    // Emergency version of app::util::scream_and_abort
    const auto stacktrace = std::stacktrace::current(1);
    const auto current_exception = std::current_exception();

#ifdef __GNUG__
    const auto current_exception_type_name = abi::__cxa_current_exception_type()->name();
    std::println("uncaught exception of type {}",
        app::util::cxx_demangle(current_exception_type_name)
    );
#else
    std::println("uncaught exception (type is not known)");
#endif
    
    // worst c++ mechanisms
    try {
        if (current_exception)
            std::rethrow_exception(current_exception);
    } catch (const std::exception& e) {
        std::println("  what: {}", e.what());
    } catch (...) {
        std::println("  exception message is not known.");
    }

    app::util::print_stacktrace(std::move(stacktrace));
    std::abort();
}

auto main(int argc, char** argv) -> int {
    // Install exception handlers
    std::set_terminate(global_exception_handler);

    app::app application_inst { app::util::make_program_arguments(argc, argv) };
    application_inst.run();
}
