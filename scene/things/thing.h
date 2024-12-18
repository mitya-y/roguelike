#pragma once

#include <string>

class Thing {
private:
  std::string _name;
  u_int32_t _weight;
  std::string _description;
public:
  Thing(const std::string &name, const u_int32_t weight, const std::string &description);
  ~Thing();

  std::string view_description();

  std::string get_name();

  u_int32_t get_weight();
};

class Note : Thing {
private:
  std::string _containing;
public:
  Note(const std::string &containing, const std::string &name, const std::string &description);
  ~Note();

  std::string view_containings();
};

//other things