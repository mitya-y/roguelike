#include "application.hpp"
#include <memory>

Application::Application() {}
Application::~Application() {}

void Application::start(std::unique_ptr<Scene> scene) {}

Scene &Application::get_scene() { return *scene; }

void Application::set_scene(std::unique_ptr<Scene> scene) {}

double Application::timer() { return 0.0; }

std::shared_ptr<Model> Application::create_model() { return {}; }

void Application::delete_model(std::shared_ptr<Model> model) {}

void Application::draw_model(const Model &model, const Vec3 &scale,
                             const Vec3 &translate, const Rotation &rotate) {}
