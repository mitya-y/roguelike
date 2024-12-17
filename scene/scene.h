#pragma once

#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include "inventory.h"
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

	Unit(glm::vec2 pos) :
    _pos(pos) {};
	virtual ~Unit();

  glm::vec2 get_positions() {
    return _pos;
  }
private:
	glm::vec2 _pos;
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

class Person : public Unit {
protected:
  Tree _tree_of_phrases;
public:
  virtual void talk() = 0;

  Person(glm::vec2 pos, Tree tree_of_phrases) :
    Unit(pos), _tree_of_phrases(tree_of_phrases) {};
  ~Person() = default;
};

class Object : Unit {
private:
  std::map<std::string, Thing> _container;
public:
	bool has_thing() {
		return _container.size() > 0 ? true : false;
	}
  
  Thing take_thing(std::string name) {
    auto iter = _container.find(name);
    if (iter == _container.end()) {
      throw std::runtime_error("incorrect name to take");
    }
    Thing tmp = iter->second;
    _container.erase(name);
    return tmp;
  }

	Object(glm::vec2 pos, std::map<std::string, Thing> container) :
    Unit(pos), _container(container) {};
	~Object() = default;
};