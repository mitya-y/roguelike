#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "base_unit.hpp"

class Tree {
public:
  struct Node {
    std::string phrase;
    struct tnode *left;
    struct tnode *right;
  };
};

class Person : Unit {
protected:
  glm::vec2 _positions;
  std::vector<std::string> _phrases;
  Tree _tree_of_phrases;

public:
  virtual glm::vec2 get_positions() = 0;
  virtual void talk(Person *talker) = 0;

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
