#include"object.h"
Object::Object() {}
Player::Player() {
	wight = 100;
	height = 100;
	bloodsucking = 0;
	lv = 1;
	exp = 0;
	x = WIDTH / 2 - wight / 2;
	y = HEIGHT / 2 - height / 2;
	live = true;
}
Enemy::Enemy() {
	before_hp = 20;
	hp = 20;
	wight = 100;
	height = 100;
	hurt = 20;
	live = false;
}
Bullet::Bullet() {
	wight = 25;
	height = 25;
	speed = 0.8;
	live = false;
}
Obstacles::Obstacles(){
	wight = 100;
	height = 100;
	live = true;
}
Drops::Drops(){
	wight = 40;
	height = 40;
	live = false;
}
bool Object::hit(Object temp) {
	int a, b, c, d, e, f, g, h;
	a = this->x; b = this->y; c = a + this->wight; d = b + this->height;
	e = temp.x; f = temp.y; g = e + temp.wight; h = f + temp.height;
	if (this->live == true && temp.live == true) {
		if (e > c || f > d || g < a || h < b)return false;
		else return true;
	}
	else return false;
}
