#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "base_unit.hpp"
#include "thing.h"

class Tree {
public:
  struct Node {
    std::string phrase;
    struct tnode *left;
    struct tnode *right;
  };
};

class Person : public Unit {
protected:
  Tree _tree_of_phrases;
public:
  virtual void talk() = 0;

  glm::vec2 get_positions() { return glm::vec2(0); }

  Person(glm::vec2 pos, Tree tree_of_phrases) :
    Unit((int)pos.x, (int)pos.y), _tree_of_phrases(tree_of_phrases) {};
  ~Person() override = default;
};


class Object : public Unit {
private:
  std::map<std::string, Thing> _container;
public:
	bool has_thing() {
		return _container.size() > 0 ? true : false;
	}

  Thing take_thing(std::string& name) {
    auto iter = _container.find(name);
    if (iter == _container.end()) {
      throw std::runtime_error("incorrect name to take");
    }
    Thing tmp = std::move(iter->second);
    _container.erase(name);
    return tmp;
  }

  std::string view_things() {
    std::string s = "";

    for(auto &item : _container) {
      s += item.first;
      s += ";";
    }

    return s;
  }

	Object(glm::vec2 pos, std::map<std::string, Thing> container) :
    Unit((int)pos.x, (int)pos.y), _container(container) {};
	~Object() override = default;
};
