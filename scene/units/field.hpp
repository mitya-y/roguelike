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
  field(int x = 0, int y = 0) {
    load("../scene.txt");
  }

  void load(const std::string &path) {
    std::ifstream file(path);

    if (!file) {
      throw std::runtime_error("Can't open file : {" + path + "}");
    }

    std::string input;
    while (file >> input) {
      if (input == "q") {
        break;
      }
      for (const auto x : input) {
        if (x == '*') {
          // auto model = Model(Model::GeometryType::Plane);
          // models.emplace_back({std::move(model), glm::mat4()});
          // models.push_back({Model(Model::GeometryType::Plane), glm::mat4()});
          // Model model(Model::GeometryType::Plane);
          models.emplace_back(Model::GeometryType::Plane);
          // models.emplace_back(Model(Model::GeometryType::Plane));
        } else if (x == 'c') {
          models.emplace_back(Model::GeometryType::Plane);
        } else if (x == 'p') {
          models.emplace_back(Model::GeometryType::Plane);
        } else if (x == '1') {
          models.emplace_back(Model::GeometryType::Plane);
        } else if (x == '2') {
          models.emplace_back(Model::GeometryType::Plane);
        } else if (x == '3') {
          models.emplace_back(Model::GeometryType::Plane);
        } else if (x == '4') {
          models.emplace_back(Model::GeometryType::Plane);
        }
      }
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
