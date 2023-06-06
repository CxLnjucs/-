#pragma once
#define WIDTH 1400
#define HEIGHT 800
#define BULLET_NUM 15
#define ENEMY_BULLET_NUM 15
#define ENEMY_NUM 10
#define OBSTACLE_NUM 5
#define DROP_NUM 10
extern int role_choose;
class Object {
public:
	double x;
	double y;
	bool live;
	int wight;
	int height;
	bool hit(Object x);
	Object();
};
class Player :public Object {
public:
	int before_hp;
	int now_hp;
	double speed;
	double bloodsucking;
	int lv;
	int exp;
	Player();
};
class Enemy :public Object {
public:
	int before_hp;
	int hp;
	int hurt;
	double speed;
	int type;
	int move_direction;
	Enemy();
};
class Bullet :public Object {
public:
	double speed;
	int hurt;
	Bullet();
};
class Obstacles :public Object {
public:
	Obstacles();
};
class Drops : public Object {
public:	
	Drops();
};

