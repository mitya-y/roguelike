#include <iostream>
#include <stdexcept>
#include <memory>

#include <GLFW/glfw3.h>

#include "application.hpp"

Application::Application() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Application::~Application() {}

void Application::start(std::unique_ptr<Scene> scene) {
  // set scene

  _window = glfwCreateWindow(800, 600, "roguelike", nullptr, nullptr);
  if (_window == nullptr) {
    throw std::runtime_error("Failed to create GLFW window");
    glfwTerminate();
  }
  glfwMakeContextCurrent(_window);

  while (not glfwWindowShouldClose(_window)) {
    // render scene

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  glfwTerminate();
}

bool Application::key_pressed(int key) {
  if (_window == nullptr) {
    return false;
  }
  return glfwGetKey(_window, key) == GLFW_PRESS;
}

Scene &Application::get_scene() { return *scene; }

void Application::set_scene(std::unique_ptr<Scene> scene) {}

double Application::timer() { return 0.0; }

std::shared_ptr<Model> Application::create_model() { return {}; }

void Application::delete_model(std::shared_ptr<Model> model) {}

void Application::draw_model(const Model &model, const Vec3 &scale,
                             const Vec3 &translate, const Rotation &rotate) {}
