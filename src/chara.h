#include "DxLib.h"
#include "util.h"
#include <vector>

#ifndef CHARA_CLASS
#define CHARA_CLASS

struct KeyInfo
{
	int MoveKeyPressed;
	int AttackKeyPressed;
	int RockOnKeyPressed;
};

struct CapInfo
{
	VECTOR CapsulePosition1;
	VECTOR CapsulePosition2;
};

class LifeBar{
public:
	int HPColor;
	int DamageColor;
	int excist;
	int HP;
	LifeBar();
	virtual void updateHP(int point);
	void draw();
};

class EnemyLifeBar :public LifeBar{

public:
	int StartX;
	int StartY;
	EnemyLifeBar();
	void updateHP(int point, int startX, int startY);
	void draw();
};

class Chara{
private:

public:
	int ModelHandle;
	int WeaponHandle;
	int RunAnim;
	int    AnimAttachIndex;
	int HandFrameIndex;
	int    MoveAnimFrameIndex;
	int    NeutralAnim;
	int    AttackFlag;
	int DamageFlag;
	int DamageAnim;
	int ShortOption;
	int HP;
	char *HandFrameName;
	char *DamageSound;
	char *RootFrame;
	float  AnimTotalTime;
	float  AnimNowTime;
	float  SinParam;
	float  CosParam;
	float  DamageHitWidth;
	float  DamageHitHeight;
	float  CharaHitWidth;
	float  CharaHitHeight;
	float AttackSphereSize;
	VECTOR Position;
	VECTOR PrevPosition;
	VECTOR NowPosition;
	VECTOR MoveVector;
	VECTOR EndLocalPosition;
	VECTOR DamageHitCenterPosition;
	VECTOR CharaHitCenterPosition;

	Chara(char *modelPath, char *neutralPath, char *rootPath, char *weaponPath, char *handFrameName,char *damageAnimName);
	void animInit();
	void changeAnim(int anim);
	void changeAnim(int anim, int shortFlag);
	virtual void endAnim();
	void continueAnim();
	//攻撃アニメーションやダメージアニメーションなど続けて呼ばれるアニメーションのためのアップデート
	//continueActionAngleは、continuationAnimが呼ばれている間変化しない値を想定　例 player.AttackAngleなど
	void updatePosition(float angle, VECTOR moveVector, Chara other);
	void updatePosition(float angle, Chara* otherVec, int vecSize);
	void updatePosition(float angle, std::vector<Chara> otherVec);
	void RockOnupdatePosition(float angle, Chara* otherVec, int vecSize);
	void RockOnupdatePosition(float angle, std::vector<Chara> otherVec);
	void continuationUpdate(float continueActionAngle, Chara other);
	void continuationUpdate(float continueActionAngle);
	void continuationUpdate(float continueActionAngle, Chara* otherVec, int vecSize);
	void continuationUpdate(float continueActionAngle, std::vector<Chara> otherVec);
	void RockOncontinuationUpdate(float continueActionAngle, Chara* otherVec, int vecSize);
	void RockOncontinuationUpdate(float continueActionAngle, std::vector<Chara> otherVec);
	void update(float angle, Chara other);
	void onceUpdate(float angle);
	void onceUpdate(float angle, Chara* otherVec, int vecSize);
	void onceUpdate(float angle, std::vector<Chara> otherVec);
	void RockOnOnceUpdate(float angle, Chara* otherVec, int vecSize);
	void RockOnOnceUpdate(float angle, std::vector<Chara> otherVec);
	int isHitted(Chara other);
	int isHitted(Chara* otherVec, int vecSize);
	int isHitted(std::vector<Chara> otherVec);
	void draw();
	void terminateModel();
	int attackedkJudge(Chara other);
	CapInfo getCapInfo(VECTOR centerPos, float hitHeight);
	CapInfo getHitCapInfo();
	CapInfo getDamageCapInfo();
	void initDamageAnim();
	//int CollisionOther(Chara other);
	void CollisionOther(Chara other);
	void CollisionOther(Chara* otherVec, int vecSize);
	void CollisionOther(std::vector<Chara> otherVec);
};

#endif