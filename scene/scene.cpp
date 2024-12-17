#include <stdexcept>

#include "scene.h"

Scene::Scene(std::string_view sw) {}
Scene::~Scene() {}

void Scene::save_to_file(std::string_view filename) {}

void Scene::draw() {
  for (auto &[_, unit] : _units) {
    unit->draw();
  }
}

void Scene::update() {
  for (auto &[_, unit] : _units) {
    unit->update();
  }
}

template<typename UnitType, typename ...Args>
std::shared_ptr<UnitType> Scene::create_unit(std::string name, Args &&...args) {
  auto unit = std::make_shared<UnitType>(args...);
  _units[name] = unit;
  return unit;
}

void Scene::delete_unit(std::string name) {
  if (!_units.contains(name)) {
    throw std::runtime_error("unit now exists");
  }
  _units.erase(_units.find(name));
}

