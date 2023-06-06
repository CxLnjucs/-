#include <iostream>
#include <fstream>
#include<math.h>
#include<time.h>
#include"object.h"
#include"button.h"
#include"intensify.h"
using namespace std;
Player player;
Bullet bull[BULLET_NUM];
//Bullet enemy_bull[ENEMY_BULLET_NUM];
Enemy enemy[ENEMY_NUM];
Obstacles obstacle[OBSTACLE_NUM];
Drops drop[DROP_NUM];
int role_choose=0;
int now_relive_num;
//计时器
bool Timer(int ms,int id) {
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
}
//图片类
IMAGE background;
IMAGE img_gameover;
IMAGE img_role1;//100x100
IMAGE img_role2;//100x100
IMAGE img_bull1;//25x25
IMAGE img_bull2;//80x80
IMAGE img_enemy1;//100x100
IMAGE img_enemy2;//100x100
IMAGE img_obstacle;//100x100
IMAGE img_drop;//40x40
IMAGE img_pojun;//200x200
IMAGE img_qixue;//200x200
IMAGE img_jixing;//200x200
//加载具体图片到图片类
void loadImg() {
	loadimage(&background, "./images/map.jpg");
	loadimage(&img_gameover, "./images/gameover.jpg");
	loadimage(&img_role1, "./images/player1.png");
	loadimage(&img_role2, "./images/player2.png");
	loadimage(&img_bull1, "./images/bull1.png");
	loadimage(&img_bull2, "./images/bull2.png");
	loadimage(&img_enemy1, "./images/enemy1.png");
	loadimage(&img_enemy2, "./images/enemy2.png");
	loadimage(&img_obstacle, "./images/obstacle.jpg");
	loadimage(&img_drop, "./images/drop.png");
	loadimage(&img_pojun, "./images/pojun.jpg");
	loadimage(&img_qixue, "./images/qixue.png");
	loadimage(&img_jixing, "./images/jixing.png");

}
//画按钮
void drawButton(Button temp) {
	setlinecolor(BLACK);
	setfillcolor(temp.color);
	fillrectangle(temp.x, temp.y, temp.x+temp.wight, temp.y+temp.height);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(15, 0, "黑体");
	int textw = textwidth(temp.button_text);
	int texth = textheight(temp.button_text);
	int xx = temp.x + (temp.wight - textw) / 2;
	int yy = temp.y + (temp.height - texth) / 2;
	outtextxy(xx, yy, temp.button_text);
}
//鼠标是否在button内
bool ifinbutton(Button temp, ExMessage m) {
	if (m.x > temp.x && m.x<temp.x + temp.wight && m.y>temp.y && m.y < temp.y + temp.height) {
		return true;
	}
	return false;
}
//鼠标点击
bool ifclickbutton(Button temp, ExMessage m) {
	if (ifinbutton(temp, m) && m.message == WM_LBUTTONDOWN) {
		return true;
	}
	return false;
}
//创造掉落物
void createDrop(int x,int y) {
	for (int i = 0; i < DROP_NUM; i++) {
		if (!drop[i].live) {
			drop[i].live = true;
			drop[i].x = x;
			drop[i].y = y;
			break;
		}
	}
}
//创造子弹
void createBullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (!bull[i].live) {
			bull[i].x = player.x + player.wight / 2 - bull[i].wight / 2;
			bull[i].y = player.y + player.height / 2 - bull[i].height / 2;
			bull[i].live = true;
			break;
		}
	}
}
//创造敌人
void createEnemy() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			enemy[i].live = true;
			enemy[i].hp = enemy[i].before_hp;
			enemy[i].type = rand() % 2;
			if (enemy[i].type == 0) {
				int side_flag = rand() % 4;
				if (side_flag == 0) {
					enemy[i].x = 0;
					enemy[i].y = rand() % (HEIGHT - enemy[i].height);
				}
				else if (side_flag == 1) {
					enemy[i].y = 0;
					enemy[i].x = rand() % (WIDTH - enemy[i].wight);
				}
				else if (side_flag == 2) {
					enemy[i].x = WIDTH - enemy[i].wight;
					enemy[i].y = rand() % (HEIGHT - enemy[i].height);
				}
				else {
					enemy[i].y = HEIGHT - enemy[i].height;
					enemy[i].x = rand() % (WIDTH - enemy[i].wight);
				}
			}
			else {
				enemy[i].move_direction = 0;
				enemy[i].y = 0;
				enemy[i].x = rand() % (WIDTH - enemy[i].wight);
				while ((enemy[i].x > 199 && enemy[i].x < 401) || (enemy[i].x > 649 && enemy[i].x < 851) || 
					(enemy[i].x > 899 && enemy[i].x < 1101)) {
					enemy[i].x = rand() % (WIDTH - enemy[i].wight);
				}
			}
			break;
		}
	}
}
//创造障碍物
void createObstacle() {
	obstacle[0].x = 750;
	obstacle[0].y = 450;
	obstacle[1].x = 300;
	obstacle[1].y = 150;
	obstacle[2].x = 1000;
	obstacle[2].y = 150;
	obstacle[3].x = 300;
	obstacle[3].y = 550;
	obstacle[4].x = 1000;
	obstacle[4].y = 550;
}
//子弹移动
void BulletMove(double speed) {
	if (Timer(200, 0)) createBullet();
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull[i].live) {
			bull[i].y -= speed;
			//子弹的各种碰撞检测
			//检测子弹与敌人的碰撞情况
			for (int j = 0; j < ENEMY_NUM; j++) {
				//判断碰撞目标是否存活非常重要，否则会被HP为0的目标阻挡,已在hit中实现判断
				if (bull[i].hit(enemy[j])) {
					enemy[j].hp-=bull[i].hurt;
					player.now_hp += bull[i].hurt * player.bloodsucking;
					while (player.now_hp > player.before_hp) {
						player.now_hp = player.before_hp;
					}
					if (enemy[j].hp <= 0) {
						enemy[j].live = false;
						gold_num += 100*(1+0.1*k_gold_num);
						player.exp += 10;
						if (player.exp >= 50) {
							player.exp -= 50;
							player.lv ++;
							//这之后定义局内强化
							Button a(200, 600, 200, 100, RGB(235, 234, 234), "破军：增加伤害");
							Button b(600, 600, 200, 100, RGB(235, 234, 234), "泣血：增加吸血效果");
							Button c(1000, 600, 200, 100, RGB(235, 234, 234), "急行：增加移动速度");
							ExMessage m;
							int IntensifyOver_Flag = 1;
							BeginBatchDraw();
							while (IntensifyOver_Flag) {
								drawButton(a);
								putimage(200, 400, &img_pojun);
								drawButton(b);
								putimage(600, 400, &img_qixue);
								drawButton(c);
								putimage(1000, 400, &img_jixing);
								FlushBatchDraw();
								peekmessage(&m, EX_MOUSE);
								if (ifclickbutton(a, m)) {
									IntensifyOver_Flag = 0;
									for (int k = 0; k < BULLET_NUM; k++) {
										bull[k].hurt += 3;
									}
								}
								if (ifclickbutton(b, m)) {
									IntensifyOver_Flag = 0;
									player.bloodsucking += 0.2;
								}
								if (ifclickbutton(c, m)) {
									IntensifyOver_Flag = 0;
									player.speed += 0.01;
								}
							}
							EndBatchDraw();
						}
						int drop_flag = rand() % 4;
						if (drop_flag == 0) {
							createDrop(enemy[j].x, enemy[j].y);
						}
					}
					bull[i].live = false;
				}
			}
			//检测子弹与障碍物的碰撞情况
			for (int j = 0; j < OBSTACLE_NUM; j++) {
				if (bull[i].hit(obstacle[j])) {
					bull[i].live = false;
				}
			}
			//碰撞检测结束
			if (bull[i].y < 0) {
				bull[i].live = false;
			}
		}
	}
}
//敌人移动
void EnemyMove(double speed) {
	if (Timer(2500,1)) createEnemy();
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live) {
			if (enemy[i].type == 0) {
				//跟踪类敌人与障碍物的碰撞检测
				int hit_flag;
				double distance = sqrt((enemy[i].y - player.y) * (enemy[i].y - player.y) + (enemy[i].x - player.x) * (enemy[i].x - player.x));
				double sin = (player.y - enemy[i].y) / distance;
				double cos = (player.x - enemy[i].x) / distance;
				enemy[i].y += speed * sin;
				enemy[i].x += speed * cos;
				if (sin >= 0) {
					hit_flag = 0;
				}
				else {
					hit_flag = 1;
				}
				for (int j = 0; j < OBSTACLE_NUM; j++) {
					if (enemy[i].hit(obstacle[j])) {
						if (hit_flag == 0) {
							enemy[i].y = obstacle[j].y - enemy[i].height;
						}
						else  {
							enemy[i].y = obstacle[j].y + obstacle[j].height;
						}
					}
				}
			}
			else {
				if (enemy[i].move_direction == 0) {
					enemy[i].y += speed;
					/*if (Timer(2000, 4)) {
						for (int j = 0; j < ENEMY_BULLET_NUM; j++) {
							if (enemy_bull[j].live == false) {
								enemy_bull[j].x=enemy[i].x + enemy[i].wight / 2 - enemy_bull[j].wight / 2;
								enemy_bull[j].y = enemy[i].y + enemy[i].height / 2 - enemy_bull[j].height / 2;
								enemy_bull[j].live == true;
							}
						}
					}
					for (int k = 0; k < ENEMY_BULLET_NUM; k++) {
						if (enemy_bull[k].live) {
							enemy_bull[k].y += speed;
							if (enemy_bull[k].hit(player)) {
								enemy_bull[k].live = false;
								player.now_hp = 0;
								//扣血
							}
							if (enemy_bull[k].y > HEIGHT - enemy_bull[k].height) {
								enemy_bull[k].live = false;
							}
						}
					}*/
					//直线类敌人与障碍物碰撞检测
					if (enemy[i].y > HEIGHT - enemy[i].height) {
						enemy[i].y = HEIGHT - enemy[i].height;
						enemy[i].move_direction = 1;
					}
					for (int j = 0; j < OBSTACLE_NUM; j++) {
						if (enemy[i].hit(obstacle[j])) {
							enemy[i].y = obstacle[j].y - enemy[i].height;
							enemy[i].move_direction = 1;
						}
					}
				}
				else {
					enemy[i].y -= speed;
					if (enemy[i].y < 0) {
						enemy[i].y = 0;
						enemy[i].move_direction = 0;
					}
				}
			}
		}
	}
}
//角色移动
void playerMove(double speed) {
	if (GetAsyncKeyState(VK_UP)|| GetAsyncKeyState('W')) {
		if (player.y > 0) {
			player.y -= speed;
			for (int j = 0; j < OBSTACLE_NUM; j++) {
				if (player.hit(obstacle[j])) {
					player.y = obstacle[j].y + obstacle[j].height+1;
				}
			}
		}
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
		if(player.y <HEIGHT-100){
			player.y += speed;
			for (int j = 0; j < OBSTACLE_NUM; j++) {
				if (player.hit(obstacle[j])) {
					player.y = obstacle[j].y - player.height-1;
				}
			}
		}
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
		if (player.x > 0) {
			player.x -= speed;
			for (int j = 0; j < OBSTACLE_NUM; j++) {
				if (player.hit(obstacle[j])) {
					player.x = obstacle[j].x + obstacle[j].wight+1;
				}
			}
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
		if (player.x<WIDTH-100) {
			player.x += speed;
			for (int j = 0; j < OBSTACLE_NUM; j++) {
				if (player.hit(obstacle[j])) {
					player.x = obstacle[j].x - player.wight-1;
				}
			}
		}
	}
	for (int i = 0; i < DROP_NUM; i++) {
		if (player.hit(drop[i])) {
			if (player.now_hp < player.before_hp - 10) {
				player.now_hp += 10;
			}
			else {
				player.now_hp = player.before_hp;
			}
			drop[i].live = false;
		}
	}
	if (Timer(100, 2)) {
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (player.hit(enemy[i])) {
				player.now_hp -= enemy[i].hurt;
				/*TCHAR all_time_text[1000];
				_stprintf_s(all_time_text, _T("%d"), enemy[i].hurt);
				setbkmode(TRANSPARENT);
				settextstyle(40, 0, _T("黑体"));
				outtextxy(player.x, player.y, all_time_text);*/
			}
		}
	}
}
//绘制图片到窗口
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{

	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
void gameDraw() {
	//画背景
	putimage(0, 0, &background);
	//画障碍物
	createObstacle();
	for (int i = 0; i < OBSTACLE_NUM; i++) {
		putimage(obstacle[i].x, obstacle[i].y, &img_obstacle);
	}
	//画人物
	if (role_choose == 1) {
		drawAlpha(&img_role1, player.x, player.y);
	}
	else {
		drawAlpha(&img_role2, player.x, player.y);
	}
	//画子弹
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull[i].live) {
			if (role_choose == 1) {
				drawAlpha(&img_bull1, bull[i].x, bull[i].y);
			}
			else {
				drawAlpha(&img_bull2, bull[i].x, bull[i].y);
			}
		}
	}
	/*for (int i = 0; i < ENEMY_BULLET_NUM; i++) {
		if (enemy_bull[i].live) {
			if (role_choose == 1) {
				drawAlpha(&img_bull1, enemy_bull[i].x, enemy_bull[i].y);
			}
			else {
				drawAlpha(&img_bull2, enemy_bull[i].x, enemy_bull[i].y);
			}
		}
	}*/
	//画掉落物
	for (int i = 0; i < DROP_NUM; i++) {
		if (drop[i].live) {
			drawAlpha(&img_drop, drop[i].x, drop[i].y);
		}
	}
	//画敌人
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live) {
			if(enemy[i].type==0)
			drawAlpha(&img_enemy2, enemy[i].x, enemy[i].y);
			else
			drawAlpha(&img_enemy1, enemy[i].x, enemy[i].y);
		}
	}
	//画血量文字
	TCHAR time_text1[50];
	_stprintf_s(time_text1,_T("HP:%d/%d"),player.now_hp, player.before_hp);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("黑体"));
	outtextxy(0, 0, time_text1);
	//画lv
	TCHAR time_text2[50];
	_stprintf_s(time_text2, _T("lv:%d"), player.lv);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("黑体"));
	outtextxy(0, 40, time_text2);
	//画可复活次数
	TCHAR time_text3[50];
	_stprintf_s(time_text3, _T("剩余可复活次数:%d"), now_relive_num);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("黑体"));
	outtextxy(0, 80, time_text3);
}
//游戏开始界面
void showbegin() {
	ifstream infile("save.dat");
	if (infile.is_open()) {
		infile >> gold_num >> hp_lv >> hurt_lv >> speed_lv>>k_gold_num>>relive_num;
		infile.close();
	}
	now_relive_num = relive_num;
	BeginBatchDraw();
	while (role_choose==0) {
		putimage(0, 0, &background);
		//画金币数
		TCHAR time_text1[50];
		_stprintf_s(time_text1, _T("金币数:%d"), gold_num);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 0, time_text1);
		//画强化等级
		//1
		TCHAR time_text2[50];
		_stprintf_s(time_text2, _T("血量 lv:%d"), hp_lv);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 40, time_text2);
		//2
		TCHAR time_text3[50];
		_stprintf_s(time_text3, _T("伤害 lv:%d"), hurt_lv);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 80, time_text3);
		//3
		TCHAR time_text4[50];
		_stprintf_s(time_text4, _T("速度 lv:%d"), speed_lv);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 120, time_text4);
		//4
		TCHAR time_text5[50];
		_stprintf_s(time_text5, _T("金币获取系数 lv:%d"), k_gold_num);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 160, time_text5);
		//5
		TCHAR time_text6[50];
		_stprintf_s(time_text6, _T("当前可复活次数:%d"), relive_num);
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, _T("黑体"));
		outtextxy(0, 200, time_text6);
		Button a(300, 250, 300, 200, RGB(235, 234, 234), "选择玩家1");
		Button b(800, 250, 300, 200, RGB(235, 234, 234), "选择玩家2");
		Button c(1150, 50, 250, 100, RGB(235, 234, 234), "增加HP等级，消耗50金币");
		Button d(1150, 200, 250, 100, RGB(235, 234, 234), "增加伤害等级，消耗50金币");
		Button e(1150, 350, 250, 100, RGB(235, 234, 234), "增加移动等级，消耗50金币");
		Button f(1150, 500, 250, 100, RGB(235, 234, 234, ), "增加金币获取系数，消耗1000金币");
		Button g(1150, 650, 250, 100, RGB(235, 234, 234, ), "增加复活次数，消耗1000金币");
		ExMessage m;
		drawButton(a);
		drawButton(b);
		drawButton(c);
		drawButton(d);
		drawButton(e);
		drawButton(f);
		drawButton(g);
		peekmessage(&m, EX_MOUSE);
		if (ifclickbutton(a, m)) {
			role_choose = 1;
		}
		if (ifclickbutton(b, m)) {
			role_choose = 2;
		}
		if (Timer(120, 3)) {
			if (ifclickbutton(c, m)) {
				if (gold_num >= 50) {
					gold_num -= 50;
					hp_lv++;
				}
			}
			if (ifclickbutton(d, m)) {
				if (gold_num >= 50) {
					gold_num -= 50;
					hurt_lv++;
				}
			}
			if (ifclickbutton(e, m)) {
				if (gold_num >= 50) {
					gold_num -= 50;
					speed_lv++;
				}
			}
			if (ifclickbutton(f, m)) {
				if (gold_num >= 1000) {
					gold_num -= 1000;
					k_gold_num++;
				}
			}
			if (ifclickbutton(g, m)) {
				if (gold_num >= 1000) {
					gold_num -= 1000;
					relive_num++;
					now_relive_num = relive_num;
				}
			}
			ofstream outfile("save.dat");
			if (outfile.is_open()) {
				outfile << gold_num << " " << hp_lv << " " << hurt_lv << " " << speed_lv <<" " <<k_gold_num<<" " <<relive_num<< endl;
				outfile.close();
			}

		}
		FlushBatchDraw();
	}
	EndBatchDraw();
}
//游戏结束界面
void showover() {
	ofstream outfile("save.dat");
	if (outfile.is_open()) {
		outfile << gold_num << " " << hp_lv << " " << hurt_lv << " " << speed_lv << " " << k_gold_num << " " << relive_num << endl;
		outfile.close();
	}
	putimage(0, 0, &img_gameover);
}
//游戏初始化
void gameinit() {
	if (role_choose == 1) {
		player.speed = 0.5+0.01*speed_lv;
		player.before_hp = 100+5*hp_lv;
		player.now_hp = 100+ 5 * hp_lv;
		for (int i = 0; i < BULLET_NUM; i++) {
			bull[i].wight = 25;
			bull[i].height = 25;
			bull[i].hurt = 10+hurt_lv;
			bull[i].live = false;
		}
	}
	else {
		player.speed = 0.3+0.01 * speed_lv;
		player.before_hp = 120 + 5 * hp_lv;
		player.now_hp = 120 + 5 * hp_lv;
		for (int i = 0; i < BULLET_NUM; i++) {
			bull[i].wight = 80;
			bull[i].height = 80;
			bull[i].hurt = 20+hurt_lv;
			bull[i].live = false;
		}
	}
}
int main() {
	initgraph(WIDTH,HEIGHT);//创建窗口
	loadImg();
	showbegin();
	gameinit();
	//双缓冲绘图
	BeginBatchDraw();
	while (player.now_hp>0) {
		gameDraw();
		FlushBatchDraw();
		playerMove(player.speed);//玩家移动
		BulletMove(1.2);//子弹移动
		EnemyMove(0.1);//敌人移动
		if (player.now_hp <= 0 && now_relive_num > 0) {
			now_relive_num--;
			player.now_hp = player.before_hp;
		}
	}
	EndBatchDraw();
	showover();
	while (1) {}

}