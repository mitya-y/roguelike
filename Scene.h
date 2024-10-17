#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

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

	Unit(int x = 0, int y = 0);
	virtual ~Unit();

};

struct Vec2 {
	int x = 0;
	int y = 0;
};

class Scene {
private: 
	char* data; //мб там хранить все в виде эффективного массива 
									//плюс туда (де)сереализация

	using UnitContainer = std::map<std::string, std::shared_ptr<Unit>>;
	using UnitIter = UnitContainer::iterator;
	UnitContainer containing_units;
	int width = 100;
	int heigh = 100;
public:
	Scene(std::string sw);
	~Scene();

	void save_to_file();
	void draw();
	void update();
	std::string serealize();

	template<typename UnitType, typename ...Args>
		std::shared_ptr<UnitType> create_unit(std::string name, Args &&...args);

		Vec2 get_bound() { return {0, 0}; }
		void delete_unit(std::string name);

		UnitIter begin() {
			return containing_units.begin();
		}
		std::map<std::string, std::shared_ptr<Unit>>::iterator end() {
			return containing_units.end();
		}

};

class Person : Unit {
protected:
	Vec2 positions;
	std::vector<std::string> phrases;
	Tree tree_of_phrases;
public:
	virtual Vec2 get_positions();
	virtual void talk(Person* talker);

	Person(int x = 0, int y = 0);
	~Person();
};

class Object : Unit {
private:
	Vec2 positions;
public:
	virtual Vec2 get_positions();

	Object(int x = 0, int y = 0);
	~Object();
};