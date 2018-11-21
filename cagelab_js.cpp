#include <app.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;
EMSCRIPTEN_BINDINGS(Cagelab) {
    class_<App>("App")
        .constructor<>()
        .function("compute_coords", &App::computeCoords)
        .function("deformMVC", &App::deformMVC)
        .function("deformGreen", &App::deformGreen)
        .function("computeMVC", &App::computeMVC)
        .function("test", &App::test)
        ;

    class_<Trimesh>("Trimesh")
        .constructor<>()
        .constructor<std::string>()
        ;

    register_vector<double>("vector<double>");
    register_vector<int>("vector<int>");
}
#endif
