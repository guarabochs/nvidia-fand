#include <app/app.hh>
#include <app/util.hh>

auto main(int argc, char** argv) -> int {
    app::app application_inst { app::make_program_arguments(argc, argv) };
    application_inst.hello_world();
}
