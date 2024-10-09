#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#define txt "scene.txt";

struct tnode {
  	std::string phrase;
  	struct tnode *left;
  	struct tnode *right;
};
typedef struct tnode tree;

class Scene {
private: 
	char* data; //мб там хранить все в виде эффективного массива 
									//плюс туда (де)сереализация
	//std::map<std::string, std::shared_ptr<Unit*>> containing_units; тут вылазит ошибка синтаксисиса я хз
	int width = 100;
	int heigh = 100;
public:
	void load_from_file();
	void save_to_file();
	void draw_the_scene();
	void update_the_scene();
};

class Unit {
protected:
	int position_x = 0;
	int position_y = 0;
public:
	virtual int* get_positions(); //массив из двух x и y
	virtual void draw();
	virtual void update();
	virtual void serealize();
	virtual Unit deserealize();

	Unit(int x = 0, int y = 0);
	virtual ~Unit();

};

class Person : Unit {
private:
	int position_x = 0;
	int position_y = 0;
	std::vector<std::string> phrases;
	tree tree_of_phrases;
public:
	int* get_positions(); //массив из двух x и y
	void draw();
	void update();
	void serealize();
	Unit deserealize();
	void talk(Unit* talker);
	void move(int direction);
	Person(int x = 0, int y = 0);
	~Person();
};

class Object : Unit {
private:
	int position_x = 0;
	int position_y = 0;
public:
	int* get_positions(); //массив из двух x и y
	void draw();
	void update();
	void serealize();
	Unit deserealize();
	Object(int x = 0, int y = 0);
	~Object();
};