#pragma once

#include "../model/model.h"
#include <memory>

class Scene {};

class Application {
private:
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
  Scene &get_scene();
  void set_scene(std::unique_ptr<Scene> scene);
  double timer();
  std::shared_ptr<Model> create_model();
  void delete_model(std::shared_ptr<Model> model);
  void draw_model(const Model &model, const Vec3 &scale, const Vec3 &translate,
                  const Rotation &rotate);
};