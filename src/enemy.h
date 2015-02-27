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
	EnemyLifeBar LBar;

	Enemy(
		char *modelPath,
		char *neutralPath,
		char *rootPath,
		char *weaponPath,
		char *handFrameName, 
		char *damageAnimName
		);
	//void initDamageAnim();
	void initDamageAnim();
	void endAnim();
	void update(Chara player, int attackKeyPressed, Chara* otherVec, int vecSize);
	void update(Chara player, int attackKeyPressed);
	void onceUpdate(float angle, Chara* otherVec, int vecSize);
	void continuationUpdate(float continueActionAngle, Chara* otherVec, int vecSize);
	void draw();
	void terminateModel();
	void think();
	VECTOR getPostion();
};

class Goblin :public Enemy{
public:
	Goblin(VECTOR pos);
};

class Human :public Enemy{
public:
	Human(VECTOR pos);
};


#endif