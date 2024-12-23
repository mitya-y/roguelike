#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "application.hpp"
#include "scene.h"

Application::Application() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // set scene
  _window = glfwCreateWindow(800, 600, "roguelike", nullptr, nullptr);
  if (_window == nullptr) {
    throw std::runtime_error("Failed to create GLFW window");
    glfwTerminate();
  }
  glfwSetWindowSizeCallback(_window, window_size_callback);
  glfwMakeContextCurrent(_window);

  // init OpenGL
  GLenum glewStatus = glewInit();

  if (glewStatus != GLEW_OK) {
    std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewStatus)
              << std::endl;
    exit(0);
  }

  glClearColor(0.30, 0.47, 0.8, 1);
  //glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // wglSwapIntervalEXT - vertical sync
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  /*glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
  GL_TRUE);*/

}

Application::~Application() {}

void Application::window_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

void Application::start(std::unique_ptr<Scene> scene) {

  int width, height;
  glfwGetWindowSize(_window, &width, &height);
  glViewport(0, 0, width, height);

  float rx, ry;
  rx = ry = 0.1;
  if (width > height) {
    rx *= float(width) / float(height);
  } else {
    ry *= float(height) / float(width);
  }

  _projection = glm::frustum(-rx / 2, rx / 2, -ry / 2, ry / 2, 0.1f, 1500.0f);
  _camera_position = glm::vec3(2.3, 1.5, 0);
  _view =
      glm::lookAt(_camera_position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
  _view_projection = _projection * _view;

  // set Camera
  //  wglSwapIntervalEXT - enable vertical sync

  std::cout << __LINE__ << std::endl;
  _scene = std::move(scene);
  //scene->create_unit<field>("field");

  while (not glfwWindowShouldClose(_window)) {
    // render scene
    render();
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

void Application::render() {
  // timer responce
  // input responce

  // clear frame
  int modes[2];
  glGetIntegerv(GL_POLYGON_MODE, modes);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float time = timer();
  float dist = 3;
  float x = cos(time) * dist, y = sin(time) * dist;
  _camera_position = glm::vec3(x, 3.0, y);
  glm::vec3 at = glm::vec3(0.2, 2, 0.2);

  _camera_position = glm::vec3(300);
  // _camera_position = glm::vec3(3);
  at = glm::vec3(0, 0, 0);

  _camera_position = glm::vec3(0, -300, 100);

  _view =
      glm::lookAt(_camera_position, at, glm::vec3(0.0, 1.0, 0.0));
  _view_projection = _projection * _view;
  // Render all units

  static Model pln(Model::GeometryType::Plane);
  static Model sph(Model::GeometryType::Sphere);
  static Model cub(Model::GeometryType::Cube);
  static Model model("models/chair", "chair.obj");
  _scene->update();
  _scene->draw();

  glFinish();
  // draw all units in current scene for (auto unit : scene.units) unit.draw();
}

Scene &Application::get_scene() { return *_scene; }

const glm::highp_mat4 &Application::view_projection() {
  return _view_projection;
}

const glm::vec3 &Application::camera_position() { return _camera_position; }

void Application::set_scene(std::unique_ptr<Scene> scene) {}

double Application::timer() { return clock() / 100000.0; }

std::shared_ptr<Model> Application::create_model() { return {}; }

void Application::delete_model(std::shared_ptr<Model> model) {}

void Application::draw_model(const Model &model, const Vec3 &scale,
                             const Vec3 &translate, const Rotation &rotate) {}
