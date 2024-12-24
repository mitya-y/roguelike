#include "player.h"

Player::Player(glm::vec2 pos) :
	Unit(pos), _inventory(20) {};

Player::~Player() = default;

void Player::pick_an_item(Object _object) {
  std::string choosed = choose_what_take(_object);
  if (choosed == "\0") {
    return;
  }
  Thing t = _object.take_thing(choosed);
  _inventory.add_to_inventory(t);
}

std::string Player::choose_what_take(Object _object) {
  if (_object.has_thing() == 0) {
    return "\0";
  }
  std::string choose = _object.view_things();
  return choose.substr(0, choose.find(';'));
}
void Player::talk(Person* talker) {}

void Player::draw() {}

void Player::update() {
	auto &app = Application::get_app();
	auto scene = app.get_scene();
	glm::vec2 positions = this->get_positions();
	for (auto &&[name, unit] : scene) {
		Person *pers = dynamic_cast<Person *>(unit.get());
		if (pers == nullptr) {
			continue;
		}
		if (check_next(pers->get_positions())) {
			pers->talk();
		}
	}
	if (app.key_pressed('W')) {
		if (positions.y < scene.get_bound().y - 1) {
			positions.y++;
		}
	}
	if (app.key_pressed('A')) {
		if (positions.x > 1) {
			positions.x--;
		}
	}
	if (app.key_pressed('S')) {
		if (positions.y > 1) {
			positions.y--;
		}
	}
	if (app.key_pressed('D')) {
		if (positions.x < scene.get_bound().x - 1) {
			positions.x++;
		}
	}
}

