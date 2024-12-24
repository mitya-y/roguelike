#pragma once

#include "inventory.h"
#include "scene.h"
#include "application.hpp"

class Player : public Unit {
public:
	Player(glm::vec2 pos);

	~Player() override;

	void pick_an_item(Object _object);
	std::string choose_what_take(Object _object);

	void talk(Person* talker);

	void draw() override;

	void update() override;
private:
	Inventory _inventory;
	bool check_next(glm::vec2 position) { return false; }
};