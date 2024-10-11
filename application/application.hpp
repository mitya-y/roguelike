#pragma once

#include "../model/model.hpp"

class Scene {};

class Application {
private:
  Scene scene;

public:
  friend class Model;
  Application();
  ~Application();

  void start(const Scene &scene);
  Scene &get_scene();
  void set_scene(const Scene &scene);
  size_t timer();
  Model create_model();
  void delete_model(Model *ptr);
  void draw_model(const Model &model, const Vec3 &scale, const Vec3 &translate,
                  const Rotation &rotate);
};
