#pragma once

#include "../../model/model.h"
#include "base_unit.hpp"

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

class field : public Unit {
private:
  std::vector<Model> models;

public:
  field(int x = 0, int y = 0) { load("../scene.txt"); }

  void load(const std::string &path) {
    std::ifstream file(path);

    if (!file) {
      throw std::runtime_error("Can't open file : {" + path + "}");
    }

    int w, h;
    file >> w >> h;
    models.reserve(w * h);
    std::string input;
    float delta_pos_y = 1, delta_pos_x = 1, scale = 2,
          pos_y = -1 * (float(h) / 2),
          pos_x = -1 * (float(w) / 2);

    std::cout << pos_y << ", " << pos_y << std::endl;
    while (file >> input) {
      if (input == "q") {
        break;
      }
      pos_x = -1 * (float(w) / 2);
      for (const auto x : input) {
        if (x == '*') {
          models.emplace_back("models/box", "box.obj");
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        } else if (x == 'p') {
          models.emplace_back(Model::GeometryType::Plane);
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        } else if (x == '1') {
          models.emplace_back(Model::GeometryType::Plane);
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        } else if (x == '2') {
          models.emplace_back(Model::GeometryType::Plane);
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        } else if (x == '3') {
          models.emplace_back(Model::GeometryType::Plane);
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        } else if (x == '4') {
          models.emplace_back(Model::GeometryType::Plane);
          models.back().scale(glm::vec3(scale, scale, scale));
          models.back().translate(glm::vec3(pos_x, 0, pos_y));
        }
        pos_x += delta_pos_x;
      }
      pos_y += delta_pos_y;
    }
    file.close();
  }

  void draw() override {
    for (auto &model : models) {
      model.draw();
    }
  }
  void update() override {}

  // char *serealize() override {}
  // Unit deserealize() override {}

  ~field() {}
};
