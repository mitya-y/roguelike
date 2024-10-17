#include "Scene.h"
class Application {
public: 
	static Application & get();
	Scene * get_current_scene();

	bool key_pressed(char key) { return false; }
};

struct Vec3 {
	int x = 0;
	int y = 0;
	int z = 0;
};

class Player : public Person {
public:
	Player();

	void talk(Person* talker) override {}

	void draw() override {}

	void update() override {
		auto &app = Application::get();
		auto* scene = app.get_current_scene();

		for (auto &&[name, unit] : *scene) {
			Person *pers = dynamic_cast<Person *>(unit.get());
			if (pers == nullptr) {
				continue;
			}
			if (check_next(pers->get_positions())) {
				pers->talk(this);
			}
		}

		if (Application::get().key_pressed('W')) {
			if (positions.x < scene->get_bound().y - 1) {
				positions.y++;
			}
		}

		if (Application::get().key_pressed('A')) {
			if (positions.x > 1) {
				positions.x--;
			}
		}

		if (Application::get().key_pressed('S')) {
			if (positions.x > 1) {
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
	bool check_next(Vec2 position) { return false; }
};