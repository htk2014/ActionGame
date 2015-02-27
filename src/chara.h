#include "DxLib.h"
#include "util.h"

#ifndef CHARA_CLASS
#define CHARA_CLASS

struct KeyInfo
{
	int MoveKeyPressed;
	int AttackKeyPressed;
};

struct CapInfo
{
	VECTOR CapsulePosition1;
	VECTOR CapsulePosition2;
};

class Chara{
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
	//�U���A�j���[�V������_���[�W�A�j���[�V�����ȂǑ����ČĂ΂��A�j���[�V�����̂��߂̃A�b�v�f�[�g
	//continueActionAngle�́AcontinuationAnim���Ă΂�Ă���ԕω����Ȃ��l��z��@�� player.AttackAngle�Ȃ�
	void updatePosition(float angle, VECTOR moveVector,Chara other);
	void continuationUpdate(float continueActionAngle,Chara other);
	int isHitted(Chara other);
	void update(float angle,Chara other);
	void draw();
	void terminateModel();
	int attackedkJudge(Chara other);
	CapInfo getHitCapInfo();
	CapInfo getDamageCapInfo();
	void initDamageAnim();
	//int CollisionOther(Chara other);
	void CollisionOther(Chara other);
};

#endif