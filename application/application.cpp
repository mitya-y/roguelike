#include "application.hpp"

Application::Application() {}
Application::~Application() {}

void Application::start(const Scene &Scene) {}

Scene &Application::get_scene() { return scene; }

void Application::set_scene(const Scene &scene) {}

size_t Application::timer() {}

Model Application::create_model() {}

void Application::delete_model(Model *ptr) {}

void Application::draw_model(const Model &model, const Vec3 &scale,
                             const Vec3 &translate, const Rotation &rotate) {}
