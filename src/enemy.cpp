#include "enemy.h"
#include <math.h>
#include<stdlib.h>
#include<time.h>
#include <random>
#include <functional>
#include <ctime>

Enemy::Enemy(
	char *modelPath,
	char *neutralPath,
	char *rootPath,
	char *weaponPath,
	char *handFrameName,
	char *damageAnimName)
	:Chara(modelPath, neutralPath, rootPath, weaponPath, handFrameName, damageAnimName)
{
	//Position = pos;
	//MV1SetPosition(ModelHandle,Position);
	ShortOption = TRUE;
	DamageSound = "Data/sound/VO_dmg_01.wav";
	Chara::animInit();
}

void Enemy::initDamageAnim(){
	Chara::initDamageAnim();
}
//continutionUpdate����endAnim���v���C���[�̍U���A�j���[�V�����悤�ɏ���������
void Enemy::endAnim(){
	Chara::endAnim();
	//�_���[�t���O��܂�
	if (DamageFlag){
		DamageFlag = FALSE;
	}
	else if (AttackFlag){
		AttackFlag = FALSE;
	}
}

void Enemy::update(Chara player,int attackKeyPressed){
	int AttackHitted = isHitted(player);
	//�U�����󂯂Ă��Ă܂��_���[�W�t���O�������Ă��Ȃ�������_���[�W�A�j���[�V�����̗p�ӂ�����
	if (AttackHitted && !DamageFlag){
		initDamageAnim();
	}
	//�U�����󂯂Ă��邩�@�܂��͍U�������Ă��邩
	if (DamageFlag || AttackFlag){
		//�_���[�W�A�j���[�V�������n�߂�B�܂��͌p��
		Chara::continuationUpdate(Angle,player);
	}
	else{
		//�󂯂Ă��Ȃ������烉���_���ŃA�N�V����
		Enemy::think();
		Chara::update(Angle,player);
	}
}

void Enemy::draw(){
	Chara::draw();
}

void Enemy::terminateModel(){
	Chara::terminateModel();
}

void Enemy::think(){
	auto myrand1 = std::bind(std::uniform_int_distribution<int>(0, 2), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	auto myrand2 = std::bind(std::uniform_int_distribution<int>(0, 7), std::mt19937(static_cast<unsigned int>(time(nullptr))));

	float AngleArray[] = { 180.0f, 0.0f, -90.0f, 90.0f, 45.0f, -45.0f, -135.0f, 135.0f };
	//�V����rand�ł��ƃv���C���[�����������Ƃ��ɓ����蔻��ɂȂ�
	srand((unsigned)time(NULL));
	int AnimRandNum = rand() % 3;
	//int AnimRandNum = myrand1();
	int AngleRandNum = rand() % 8;
	//int AngleRandNum = myrand2();
	
	int NewAnimState = AnimArray[AnimRandNum];
	Angle = AngleArray[AngleRandNum];
	//�����_���ŏo���̂��U���Ȃ�A�U���t���O�𗧂Ă�
	if (NewAnimState == AttackAnim){
		AttackFlag = TRUE;
	}

	//�O��̃A�j���[�V�����ƈႤ�Ȃ�X�V
	if (AnimState != NewAnimState){
		AnimState = NewAnimState;
		changeAnim(AnimState);
	}
}

Goblin::Goblin(VECTOR pos) 
	:Enemy(
	"Data/Goblin/Goblin.mv1",
	"Data/Goblin/Anim_Neutral.mv1",
	"root",
	"Data/Weapon/Axe/Axe.mv1",
	"hansocketR",
	"Data/Goblin/Anim_Damage.mv1")
{
	WalkAnim = MV1LoadModel("Data/Goblin/Anim_Walk.mv1");
	AttackAnim = MV1LoadModel("Data/Goblin/Anim_Attack1.mv1");
	//�����_���A�N�V�����p�z��ɃA�j���[�V����������
	AnimArray[0] = NeutralAnim;
	AnimArray[1] = WalkAnim;
	AnimArray[2] = AttackAnim;
	//�A�j���[�V�����̏�Ԃ��j���[�g�����ŏ�����
	AnimState = NeutralAnim;

	EndLocalPosition = { 0.0, 100.0, 0.0 };

	//�퓖���蔻��p
	DamageHitWidth = 50.0;
	DamageHitHeight = 80.0;
	DamageHitCenterPosition = { 0.0, 90.0, 0.0 };
	AttackSphereSize = 40.0;

	Position = pos;
	MV1SetPosition(ModelHandle, Position);
}

Human::Human(VECTOR pos)
	:Enemy(
	"Data/stable/testModel4.mv1",
	"Data/stable/testModel4_Anim.mv1",
	"models_root",
	"Data/Weapon/Axe/Axe.mv1",
	"models_hand_ik_R",
	"Data/stable/testModel4_Anim.mv1")
{
	WalkAnim = MV1LoadModel("Data/stable/testModel4_Anim.mv1");
	AttackAnim = MV1LoadModel("Data/stable/testModel4_Anim.mv1");
	//�����_���A�N�V�����p�z��ɃA�j���[�V����������
	AnimArray[0] = NeutralAnim;
	AnimArray[1] = WalkAnim;
	AnimArray[2] = AttackAnim;
	//�A�j���[�V�����̏�Ԃ��j���[�g�����ŏ�����
	AnimState = NeutralAnim;

	EndLocalPosition = { 0.0, 100.0, 0.0 };

	//�퓖���蔻��p
	DamageHitWidth = 50.0;
	DamageHitHeight = 80.0;
	DamageHitCenterPosition = { 0.0, 90.0, 0.0 };
	AttackSphereSize = 40.0;

	Position = pos;
	MV1SetPosition(ModelHandle, Position);
}




