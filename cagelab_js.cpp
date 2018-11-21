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
        .function("getCoords4cageV", &App::getCoords4cageV)
        .function("test", &App::test)
        .function("debugMC", &App::debugMC)
        .function("debugMT", &App::debugMT)
        .function("debugCC", &App::debugCC)
        .function("debugCT", &App::debugCT)
        ;

    class_<Trimesh>("Trimesh")
        .constructor<>()
        .constructor<std::string>()
        ;

    register_vector<double>("vector<double>");
    register_vector<int>("vector<int>");
    enum_<BarycentricMode>("BarycentricMode")
            .value("NONE_COORDS", NONE_COORDS)
            .value("GREEN_COORDS", GREEN_COORDS)
            .value("MVC_COORDS", MVC_COORDS)
            ;
}
#endif
