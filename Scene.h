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
	std::map<std::string, std::shared_ptr<Unit>> containing_units;
	int width = 100;
	int heigh = 100;
public:
	Scene(std::string sw);
	~Scene();

	void save_to_file();
	void draw();
	void update();
	std::string serealize();
	void add_unit(std::string name, std::shared_ptr<Unit> ptr);
	void delete_unit(std::string name);


};

class Person : Unit {
private:
	int position_x = 0;
	int position_y = 0;
	std::vector<std::string> phrases;
	Tree tree_of_phrases;
public:
	virtual Vec2 get_positions();
	virtual void talk(Unit* talker);

	void draw();
	void update();
	char* serealize();
	Unit deserealize();
	
	Person(int x = 0, int y = 0);
	~Person();
};

class Object : Unit {
private:
	int position_x = 0;
	int position_y = 0;
public:
	virtual Vec2 get_positions();

	void draw();
	void update();
	char* serealize();
	Unit deserealize();

	Object(int x = 0, int y = 0);
	~Object();
};