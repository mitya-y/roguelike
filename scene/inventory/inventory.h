#pragma once

#include <map>
#include "thing.h"

class Inventory {
private:
  std::map<std::string, Thing> _inventory;
  u_int32_t _capacity = 20;
public:
  Inventory(u_int32_t capacity);
  ~Inventory();

  void add_to_inventory(Thing t);

  void delete_from_inventory(std::string name);

  std::string view_inventory();

  u_int32_t size_of_inventory();
};