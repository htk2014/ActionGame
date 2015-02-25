#include "math.h"
#include "chara.h"

Chara::Chara(char *modelPath, char *neutralPath, char *rootPath, char *weaponPath, char *handFrameName,char *damageAnimName){
	ModelHandle = MV1LoadModel(modelPath);
	WeaponHandle = MV1LoadModel(weaponPath);
	NeutralAnim = MV1LoadModel(neutralPath);
	DamageAnim = MV1LoadModel(damageAnimName);
	RootFrame = rootPath;
	HandFrameName = handFrameName;
	HandFrameIndex = MV1SearchFrame(ModelHandle, handFrameName);
	DamageFlag = FALSE;
	AttackFlag = FALSE;
}

void Chara::changeAnim(int anim){
	// 今までアタッチしていたアニメーションをデタッチ
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// 新しいアニメーションをアタッチ
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// アニメーションの総時間を取得しておく
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// アニメーション再生時間を初期化
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

void Chara::changeAnim(int anim,int shortFlag){
	// 今までアタッチしていたアニメーションをデタッチ
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// 新しいアニメーションをアタッチ
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// アニメーションの総時間の半分を取得
	if (shortFlag){
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex)/2;
	} else{
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);
	}
	// アニメーション再生時間を初期化
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

void Chara::animInit(){
	// 待機アニメーションをアタッチ
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, NeutralAnim, FALSE);

	// 待機アニメーションの総時間を取得しておく
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// アニメーション再生時間を初期化
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

	// アニメーションで移動をしているフレームの番号を検索する
	MoveAnimFrameIndex = MV1SearchFrame(ModelHandle, RootFrame);

	// アニメーションで移動をしているフレームを無効にする
	MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, MV1GetFrameLocalMatrix(ModelHandle, MoveAnimFrameIndex));

	// ３Ｄモデルの座標を初期化
	Position = VGet(0.0f, 0.0f, 0.0f);
}

void Chara::endAnim(){
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime);
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VSub(NowPosition, PrevPosition);

	// 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
	AnimNowTime -= AnimTotalTime;

	// 次に『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」と
	// 『アニメーション再生時間０での「アニメーションで移動しているフレームの座標」』との差分を移動ベクトルに加算する
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0f);
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VAdd(MoveVector, VSub(NowPosition, PrevPosition));

	// ニュートラルアニメーションに変更
	changeAnim(NeutralAnim);
}

void Chara::continueAnim(){
	// 新しいアニメーション再生時間をセット
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

	// 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
	// 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分を移動ベクトルとする
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VSub(NowPosition, PrevPosition);


}

void Chara::updatePosition(float angle, VECTOR moveVector,Chara other){
	//変更する前のキャラの座標を保存しておく
	VECTOR tempPos = Position;

	// キャラクターの向きに合わせて移動ベクトルの角度を回転させて、キャラクターモデルの座標に加算
	SinParam = sin(-angle / 180.0f * DX_PI_F);
	CosParam = cos(-angle / 180.0f * DX_PI_F);
	Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
	Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;

	int ColliWithOther = CollisionOther(other);
	//他のキャラと衝突しているなら、座標を元に戻す
	if (ColliWithOther){
		Position = tempPos;
		Position.x -= (MoveVector.x * CosParam - MoveVector.z * SinParam)/5;
		Position.z -= (MoveVector.x * SinParam + MoveVector.z * CosParam)/5;
	}
	// ３Ｄモデルに新しい座標をセット
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//武器の位置を更新
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// 新しい向きをセット
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}
/*
void Chara::updatePosition(float angle, VECTOR moveVector){
	//変更する前のキャラの座標を保存しておく
	VECTOR tempPos = Position;

	// キャラクターの向きに合わせて移動ベクトルの角度を回転させて、キャラクターモデルの座標に加算
	SinParam = sin(-angle / 180.0f * DX_PI_F);
	CosParam = cos(-angle / 180.0f * DX_PI_F);
	Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
	Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;

	// ３Ｄモデルに新しい座標をセット
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//武器の位置を更新
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// 新しい向きをセット
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}
*/

void Chara::continuationUpdate(float continueActionAngle, Chara other){
	// アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// アニメーション再生時間を進める
	AnimNowTime += 0.6f;

	// アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
	if (AnimNowTime >= AnimTotalTime)
	{
		endAnim();
	}
	else
	{
		continueAnim();
	}

	updatePosition(continueActionAngle, MoveVector,other);

}

void Chara::update(float angle,Chara other){
	// アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// アニメーション再生時間を進める
	AnimNowTime += 0.6f;

	// アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
	if (AnimNowTime >= AnimTotalTime)
	{
		// 超えている場合は、まず『アニメーション再生時間を進める前の「アニメーションで移動しているフレームの座標」』と、
		// 『アニメーションの終端での「アニメーションで移動しているフレームの座標」』との差分を移動ベクトルとする
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime);
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VSub(NowPosition, PrevPosition);

		// 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
		AnimNowTime -= AnimTotalTime;

		// 次に『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」と
		// 『アニメーション再生時間０での「アニメーションで移動しているフレームの座標」』との差分を移動ベクトルに加算する
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0f);
		PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VAdd(MoveVector, VSub(NowPosition, PrevPosition));
	}
	else
	{
		// 新しいアニメーション再生時間をセット
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

		// 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
		// 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分を移動ベクトルとする
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VSub(NowPosition, PrevPosition);
	}
	updatePosition(angle, MoveVector, other);


}

void Chara::draw(){
	MV1DrawModel(ModelHandle);
	CapInfo cap = getCapInfo();
	//カプセル描画
	DrawCapsule3D(cap.CapsulePosition1, cap.CapsulePosition2,DamageHitWidth, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
	
	if (WeaponHandle){
		MV1DrawModel(WeaponHandle);
	}
}

void Chara::terminateModel(){
	// モデルの削除
	MV1DeleteModel(ModelHandle);
}

int Chara::attackJudge(CapInfo Cinfo, float EnemyHitWidth){
	int HitResult = FALSE;
	VECTOR nearPos[2];
	VECTOR farPos[2];

	MATRIX AttackPosMatrix =
		MV1GetFrameLocalWorldMatrix(ModelHandle, HandFrameIndex);

	// 攻撃判定で使用する２点を算出
	nearPos[0] = VTransform(VGet(0.0f, 0.0f, 0.0f), AttackPosMatrix);
	farPos[0] = VTransform(EndLocalPosition, AttackPosMatrix);

	HitResult = HitCheck_Capsule_Triangle(
		Cinfo.CapsulePosition1,
		Cinfo.CapsulePosition2,
		EnemyHitWidth,
		nearPos[0],
		farPos[1],
		nearPos[0]
		);
	if (!HitResult){
		HitResult = HitCheck_Capsule_Triangle(
			Cinfo.CapsulePosition1,
			Cinfo.CapsulePosition2,
			EnemyHitWidth,
			nearPos[0],
			farPos[1],
			nearPos[1]
			);
	}
	return HitResult;
}

CapInfo Chara::getCapInfo(){

	VECTOR CapsulePosition1;
	VECTOR CapsulePosition2;

	CapsulePosition1 =
		CapsulePosition2 = VAdd(Position, DamageHitCenterPosition);

	CapsulePosition1.y -= DamageHitHeight / 2.0f;
	CapsulePosition2.y += DamageHitHeight / 2.0f;

	return{ CapsulePosition1, CapsulePosition2 };
}

void Chara::initDamageAnim(){
	//攻撃を受けているフラグを立てる
	DamageFlag = TRUE;
	//ダメージアニメーションに変更(ShortOptionがTRUEならダメージアニメの通常の半分の時間に　理由:まんまだと無敵時間が長すぎるから）
	Chara::changeAnim(DamageAnim,ShortOption);
	//ダメージ用の音楽を流す
	PlaySoundFile(DamageSound, DX_PLAYTYPE_BACK);
}
//
int Chara::isHitted(Chara chara, int enemyAttaking){
	CapInfo Cinfo = getCapInfo();
	int HitResult = chara.attackJudge(Cinfo, DamageHitWidth);
	
	if (HitResult && enemyAttaking){
		return TRUE;
	}
	else{
		return FALSE;
	}
	
}

int Chara::CollisionOther(Chara other){
	//他のキャラのカプセル情報
	CapInfo otherCinfo = other.getCapInfo();
	//キャラ同士の当たり判定
	MV1SetupCollInfo(ModelHandle, -1, 8, 8, 8);
	//カプセルの大きさ調整
	otherCinfo.CapsulePosition1.x = otherCinfo.CapsulePosition1.x / 100;
	otherCinfo.CapsulePosition1.y = otherCinfo.CapsulePosition1.y / 100;
	otherCinfo.CapsulePosition2.x = otherCinfo.CapsulePosition2.x / 100;
	otherCinfo.CapsulePosition2.y = otherCinfo.CapsulePosition2.y / 100;
	//他のキャラのカプセルと自分が衝突したか
	auto HitPolyDim = MV1CollCheck_Capsule(ModelHandle, -1, otherCinfo.CapsulePosition1, otherCinfo.CapsulePosition1, 100.0f);
	

	//当たり判定用構造体の後始末
	MV1CollResultPolyDimTerminate(HitPolyDim);

	//HitPolyDim.HitNumが１以上なら衝突している
	if (HitPolyDim.HitNum >= 1){
		return TRUE;
	}
	else{
		return FALSE;
	}
}