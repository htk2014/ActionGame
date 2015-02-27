#include "DxLib.h"
#include "chara.h"
#include "enemy.h"

class Player: public Chara{
public:
	int    AttackAnim;
	int    AttackAnim2;
	int    AttackAnim3;
	int    AttackState;
	int    RunFlag;
	int AttackAnimArray[3];
	int AttackContinueFlag;
	int AttackContinueNum;
	int LastAttackKeyPressed;
	float AttackTotalTimes[3];
	float AttackAngle;

	Player();
	void changeAnim(int AnimFlag, int Anim);
	void changeAnim(int AnimNum);
	void attackAnimInit(float angle);
	void attackAnimUpdate();
	void setupContinueAttack();
	void endAnim();
	void setupAttack(KeyInfo KInfo, float angle);
	void update(KeyInfo KInfo, float angle, Enemy enemy);
	void update(KeyInfo KInfo, float angle, Chara* enemyVec, int vecSize);
	void onceUpdate(float angle, Chara* enemyVec,int vecSize);
	void continuationUpdate(float continueActionAngle, Chara* enemyVec, int vecSize);

};

