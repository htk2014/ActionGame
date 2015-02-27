#include "player.h"
#include <math.h>
#include "util.h"

Player::Player() 
	:Chara(
	"Data/PC.mv1",
	"Data/Anim_Neutral.mv1",
	"root",
	"Data/Weapon/Sabel/Sabel.mv1",
	"wp",
	"Data/Anim_Damage.mv1")
{
	//前回攻撃キーが押されているか
	LastAttackKeyPressed = FALSE;
	//走っているか
	RunFlag = FALSE;
	ShortOption = FALSE;
	//攻撃しているアニメーション
	AttackState = FALSE;
	AttackContinueFlag = FALSE;
	AttackContinueNum = 0;
	//走りアニメーション読み込み
	RunAnim = MV1LoadModel("Data/Anim_Run.mv1");
	//攻撃アニメーション読み込み
	AttackAnim = MV1LoadModel("Data/Anim_Attack1.mv1");
	AttackAnim2 = MV1LoadModel("Data/Anim_Attack2.mv1");
	AttackAnim3 = MV1LoadModel("Data/Anim_Attack3.mv1");
	//連続攻撃用アニメーション配列
	AttackAnimArray[0] = AttackAnim;
	AttackAnimArray[1] = AttackAnim2;
	AttackAnimArray[2] = AttackAnim3;

	EndLocalPosition = { 0.0, -100, 0.0 };

	//被攻撃判定用
	DamageHitWidth = 50.0;
	DamageHitHeight = 80.0;
	DamageHitCenterPosition = {0.0,90.0, 0.0};

	DamageSound = "Data/sound/dmg_byAxe_00.wav";
	
	AttackSphereSize = 1.0;

	Chara::animInit();
}

void Player::changeAnim(int AnimFlag, int Anim){
	// 今までアタッチしていたアニメーションをデタッチ
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// 新しいアニメーションをアタッチ
	if (AnimFlag)
	{
		AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, Anim, FALSE);
	}
	else
	{
		AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, NeutralAnim, FALSE);
	}

	// アニメーションの総時間を取得しておく
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// アニメーション再生時間を初期化
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

//連続攻撃用アニメーション変更を行う
void Player::changeAnim(int AnimNum){
	int Anim = AttackAnimArray[AnimNum];
	Chara::changeAnim(Anim);
}


void Player::attackAnimInit(float angle){
	//攻撃をしているフラグを立てる
	AttackFlag = TRUE;
	//攻撃アニメーション状態を攻撃アニメーション１にする
	AttackState = AttackAnimArray[AttackContinueNum];
	//攻撃アングルを初期化
	AttackAngle = angle;
	//最初の攻撃アニメーションに変更
	Chara::changeAnim(AttackAnim);
}

void Player::setupContinueAttack(){
	AttackContinueFlag = FALSE;
	//攻撃回数更新
	AttackContinueNum++;
	//次の攻撃アニメーションに変更
	changeAnim(AttackContinueNum);
}

//continutionUpdate内のendAnimをプレイヤーの攻撃アニメーションように書き換える
void Player::endAnim(){
	Chara::endAnim();

	if (DamageFlag){
		DamageFlag = FALSE;
	}
	else if (AttackFlag){
		//攻撃フラグを折る
		AttackFlag = FALSE;
		//攻撃アニメーションの状態初期化
		AttackState = FALSE;
		//連続攻撃回数初期化
		AttackContinueNum = 0;
		AttackContinueFlag = FALSE;
	}
}
void Player::setupAttack(KeyInfo KInfo, float angle){
	//攻撃キーが押されていて、攻撃フラグがまだたっていなかったら攻撃アニメーションの準備をする
	if (KInfo.AttackKeyPressed && !AttackFlag){
		attackAnimInit(angle);
	}
	//攻撃キーが押されていて、攻撃フラグがすでに立っているなら連続攻撃フラグを立てる
	//連続攻撃はキーを一度離してからじゃないとできないようにする(前回のキーがFALSE -> !LastAttackKeyPressed )
	else if (!LastAttackKeyPressed && KInfo.AttackKeyPressed && AttackFlag){
		AttackContinueFlag = TRUE;
	}
}

void Player::update(KeyInfo KInfo, float angle,Enemy enemy){
	int AttackHitted = isHitted(enemy);
	setupAttack(KInfo, angle);

	if (AttackHitted && !DamageFlag){
		initDamageAnim();
	}

	//攻撃フラグがたっているなら攻撃を始める。または攻撃を継続
	if (AttackFlag || DamageFlag){
		//連続攻撃フラグが立っているなら次のアニメーションに移行
		if (!LastAttackKeyPressed && KInfo.AttackKeyPressed && AttackFlag){
			//連続攻撃がまだ続けられるか？
			if (AttackContinueFlag && AttackContinueNum < 2){
				//アニメーション変更
				setupContinueAttack();
			}
		}
		Chara::continuationUpdate(AttackAngle,enemy);

	}
	else{
		//移動フラグに変更があるか？
		if (RunFlag != KInfo.MoveKeyPressed)
		{
			RunFlag = KInfo.MoveKeyPressed;
			changeAnim(RunFlag, RunAnim);
		}
		Chara::update(angle,enemy);
	}

	
	//連続攻撃用過去キー情報更新
	LastAttackKeyPressed = KInfo.AttackKeyPressed;

}

void Player::update(KeyInfo KInfo, float angle, Chara* enemyVec,int vecSize){
	int AttackHitted = isHitted(enemyVec,vecSize);
	setupAttack(KInfo, angle);

	if (AttackHitted && !DamageFlag){
		initDamageAnim();
	}

	//攻撃フラグがたっているなら攻撃を始める。または攻撃を継続
	if (AttackFlag || DamageFlag){
		//連続攻撃フラグが立っているなら次のアニメーションに移行
		if (!LastAttackKeyPressed && KInfo.AttackKeyPressed && AttackFlag){
			//連続攻撃がまだ続けられるか？
			if (AttackContinueFlag && AttackContinueNum < 2){
				//アニメーション変更
				setupContinueAttack();
			}
		}
		continuationUpdate(AttackAngle, enemyVec,vecSize);

	}
	else{
		//移動フラグに変更があるか？
		if (RunFlag != KInfo.MoveKeyPressed)
		{
			RunFlag = KInfo.MoveKeyPressed;
			changeAnim(RunFlag, RunAnim);
		}
		onceUpdate(angle, enemyVec,vecSize);
	}


	//連続攻撃用過去キー情報更新
	LastAttackKeyPressed = KInfo.AttackKeyPressed;

}

void Player::onceUpdate(float angle,Chara* enemyVec,int vecSize){
	Chara::onceUpdate(angle);
	Chara::updatePosition(angle, enemyVec, vecSize);
}

void Player::continuationUpdate(float continueActionAngle, Chara* enemyVec, int vecSize){
	Chara::continuationUpdate(continueActionAngle);
	Chara::updatePosition(continueActionAngle, enemyVec, vecSize);
}



