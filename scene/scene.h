#pragma once

#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include "application.hpp"

#define txt "scene.txt";

class Tree {
public:
  struct Node {
    std::string phrase;
    struct tnode *left;
    struct tnode *right;
  };
};

class Unit {
public:
  virtual void draw();
  virtual void update();
  virtual char* serealize();
  virtual Unit deserealize();

  Unit(int x = 0, int y = 0);
  virtual ~Unit();
};

class Scene {
private:
  using UnitContainer = std::map<std::string, std::shared_ptr<Unit>>;
  using UnitIter = UnitContainer::iterator;

  UnitContainer _units;

  int _width = 100;
  int _heigh = 100;

public:
  Scene(std::string_view sw);
  ~Scene();

  void save_to_file(std::string_view filename);
  void draw();
  void update();

  template<typename UnitType, typename ...Args>
  std::shared_ptr<UnitType> create_unit(std::string name, Args &&...args);

  void delete_unit(std::string name);

  glm::vec2 get_bound() { return {0, 0}; }

  UnitIter begin() noexcept {
    return _units.begin();
  }
  UnitIter end() noexcept {
    return _units.end();
  }
};

class Person : Unit {
protected:
  glm::vec2 _positions;
  std::vector<std::string> _phrases;
  Tree _tree_of_phrases;

public:
  virtual glm::vec2 get_positions() = 0;
  virtual void talk(Person* talker) = 0;

  Person(int x = 0, int y = 0);
  ~Person();
};

class Object : Unit {
private:
  glm::vec2 positions;
public:
  virtual glm::vec2 get_positions() = 0;

  Object(int x = 0, int y = 0);
  ~Object();
};