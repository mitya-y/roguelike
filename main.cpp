#include <iostream>

#include "application/application.hpp"

int main() {

  Application::get_app().start(std::make_unique<Scene>());

  std::cout << "cgsg forever\n";
}
