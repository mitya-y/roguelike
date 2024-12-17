#pragma once
#include "inventory.h"
#include "scene.h"

class Application {
public:
	static Application & get();
	Scene * get_current_scene();

	bool key_pressed(char key) { return false; }
};

class Player : Unit {
public:
	Player(glm::vec2 pos) :
		Unit(pos), _inventory(20) {};

	~Player() = default;

	void pick_an_item() {

	}

	void talk(Person* talker) {}

	void draw() override {}

	void update() override {
		auto &app = Application::get();
		auto* scene = app.get_current_scene();
		glm::vec2 positions = this->get_positions();

		for (auto &&[name, unit] : *scene) {
			Person *pers = dynamic_cast<Person *>(unit.get());
			if (pers == nullptr) {
				continue;
			}
			if (check_next(pers->get_positions())) {
				pers->talk();
			}
		}

		if (Application::get().key_pressed('W')) {
			if (positions.y < scene->get_bound().y - 1) {
				positions.y++;
			}
		}

		if (Application::get().key_pressed('A')) {
			if (positions.x > 1) {
				positions.x--;
			}
		}

		if (Application::get().key_pressed('S')) {
			if (positions.y > 1) {
				positions.y--;
			}
		}

		if (Application::get().key_pressed('D')) {
			if (positions.x < scene->get_bound().x - 1) {
				positions.x++;
			}
		}
	}

private:
	Inventory _inventory;
	bool check_next(glm::vec2 position) { return false; }
};