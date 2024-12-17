#include <iostream>

#include "scene.h"
#include "application.hpp"

int main() {
  Application::get_app().start(std::make_unique<Scene>("scene.txt"));
  std::cout << "cgsg forever\n";
}
