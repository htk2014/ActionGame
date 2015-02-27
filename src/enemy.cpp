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
//continutionUpdate内のendAnimをプレイヤーの攻撃アニメーションように書き換える
void Enemy::endAnim(){
	Chara::endAnim();
	//ダメーフラグを折る
	if (DamageFlag){
		DamageFlag = FALSE;
	}
	else if (AttackFlag){
		AttackFlag = FALSE;
	}
}

void Enemy::update(Chara player,int attackKeyPressed){
	int AttackHitted = isHitted(player);
	//攻撃を受けていてまだダメージフラグが立っていなかったらダメージアニメーションの用意をする
	if (AttackHitted && !DamageFlag){
		initDamageAnim();
	}
	//攻撃を受けているか　または攻撃をしているか
	if (DamageFlag || AttackFlag){
		//ダメージアニメーションを始める。または継続
		Chara::continuationUpdate(Angle,player);
	}
	else{
		//受けていなかったらランダムでアクション
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
	//新しいrandでやるとプレイヤーがおかしいときに当たり判定になる
	srand((unsigned)time(NULL));
	int AnimRandNum = rand() % 3;
	//int AnimRandNum = myrand1();
	int AngleRandNum = rand() % 8;
	//int AngleRandNum = myrand2();
	
	int NewAnimState = AnimArray[AnimRandNum];
	Angle = AngleArray[AngleRandNum];
	//ランダムで出たのが攻撃なら、攻撃フラグを立てる
	if (NewAnimState == AttackAnim){
		AttackFlag = TRUE;
	}

	//前回のアニメーションと違うなら更新
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
	//ランダムアクション用配列にアニメーションを入れる
	AnimArray[0] = NeutralAnim;
	AnimArray[1] = WalkAnim;
	AnimArray[2] = AttackAnim;
	//アニメーションの状態をニュートラルで初期化
	AnimState = NeutralAnim;

	EndLocalPosition = { 0.0, 100.0, 0.0 };

	//被当たり判定用
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
	//ランダムアクション用配列にアニメーションを入れる
	AnimArray[0] = NeutralAnim;
	AnimArray[1] = WalkAnim;
	AnimArray[2] = AttackAnim;
	//アニメーションの状態をニュートラルで初期化
	AnimState = NeutralAnim;

	EndLocalPosition = { 0.0, 100.0, 0.0 };

	//被当たり判定用
	DamageHitWidth = 50.0;
	DamageHitHeight = 80.0;
	DamageHitCenterPosition = { 0.0, 90.0, 0.0 };
	AttackSphereSize = 40.0;

	Position = pos;
	MV1SetPosition(ModelHandle, Position);
}




