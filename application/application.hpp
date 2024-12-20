#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"

class Scene;

class Application {
private:
  GLFWwindow *_window = nullptr;

  glm::highp_mat4 _view {};
  glm::highp_mat4 _projection {};
  glm::highp_mat4 _view_projection {};

  glm::vec3 _camera_position {};

  // Scene *_scene;
  std::unique_ptr<Scene> scene;

  Application();

public:
  static Application &get_app() {
    static Application app;

    return app;
  }

  friend class Model;
  ~Application();

  void start(std::unique_ptr<Scene> scene);

  void render(void);

  Scene & get_scene();
  void set_scene(std::unique_ptr<Scene> scene);

  double timer();

  std::shared_ptr<Model> create_model();
  void delete_model(std::shared_ptr<Model> model);

  void draw_model(const Model &model, const Vec3 &scale, const Vec3 &translate,
                  const Rotation &rotate);

  // key is GLFW_KEY_*
  bool key_pressed(int key);

  const glm::highp_mat4 & view_projection();
  const glm::vec3 & camera_position();

private:
  static void APIENTRY glDebugOutput(GLenum Source, GLenum Type, GLuint Id, GLenum Severity,
                                     GLsizei Length, const GLchar *Message, const void *UserParam);

  static void window_size_callback(GLFWwindow* window, int width, int height);
};
