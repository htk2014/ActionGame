#include "DxLib.h"
#include "chara.h"

#ifndef ENEMY_CLASS
#define ENEMY_CLASS

class Enemy :public Chara{
public:
	int WalkAnim;
	int AttackAnim;
	int AnimArray[3];
	int AnimState;
	float Angle;

	Enemy(char *modelPath, char *neutralPath, char *rootPath, char *weaponPath, char *handFrameName, char *damageAnimName,VECTOR pos);
	//void initDamageAnim();
	void initDamageAnim();
	void endAnim();
	void update(Chara player, int attackKeyPressed);
	void draw();
	void terminateModel();
	void think();
};

class Goblin :public Enemy{
public:
	Goblin(VECTOR pos);
};

#endif