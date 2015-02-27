#include "math.h"
#include "chara.h"

// キャラクター同士で当たったときの押し出される力
#define CHARA_HIT_PUSH_POWER            (12.0f)

LifeBar::LifeBar(){
	DamageColor = GetColor(255, 0, 0);    // ダメージバーの色
	HPColor = GetColor(0, 0, 255);    // HPバーの色
	HP = 100;
}

void LifeBar::updateHP(int point){
	HP = point;
}

void LifeBar::draw(){
	DrawBox(0, 450, 400, 550, DamageColor, TRUE);
	DrawBox(0, 450, HP * 4, 550, HPColor, TRUE);
}

EnemyLifeBar::EnemyLifeBar():LifeBar(){
	DamageColor = GetColor(255, 0, 0);    // ダメージバーの色
	HPColor = GetColor(0, 0, 255);    // HPバーの色
	HP = 100;
	StartX = 0;
	StartY = 0;
}

void EnemyLifeBar::updateHP(int point, int startX, int startY){
	HP = point;
	StartX = startX;
	StartY = startY;
}

void EnemyLifeBar::draw(){
	DrawBox(StartX, StartY, StartX + 40, StartY + 10, DamageColor, TRUE);
	DrawBox(StartX, StartY, StartX + HP * 4/10, StartY + 10, HPColor, TRUE);
}

Chara::Chara(
	char *modelPath,
	char *neutralPath,
	char *rootPath,
	char *weaponPath,
	char *handFrameName,
	char *damageAnimName)
{
	ModelHandle = MV1LoadModel(modelPath);
	WeaponHandle = MV1LoadModel(weaponPath);
	NeutralAnim = MV1LoadModel(neutralPath);
	DamageAnim = MV1LoadModel(damageAnimName);
	RootFrame = rootPath;
	HandFrameName = handFrameName;
	HandFrameIndex = MV1SearchFrame(ModelHandle, handFrameName);
	DamageFlag = FALSE;
	AttackFlag = FALSE;

	CharaHitWidth = 60.0f;
	CharaHitHeight = 60.0f;
	CharaHitCenterPosition = { 0.0f, 90.0f, 0.0f };

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

void Chara::changeAnim(int anim, int shortFlag){
	// 今までアタッチしていたアニメーションをデタッチ
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// 新しいアニメーションをアタッチ
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// アニメーションの総時間の半分を取得
	if (shortFlag){
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex) / 2;
	}
	else{
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

void Chara::updatePosition(float angle, VECTOR moveVector, Chara other){
	//変更する前のキャラの座標を保存しておく
	VECTOR tempPos = Position;

	//攻撃処理は除く
	if (!AttackFlag){
		// キャラクターの向きに合わせて移動ベクトルの角度を回転させて、キャラクターモデルの座標に加算
		SinParam = sin(-angle / 180.0f * DX_PI_F);
		CosParam = cos(-angle / 180.0f * DX_PI_F);
		Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
		Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;
	}

	//衝突処理
	CollisionOther(other);

	// ３Ｄモデルに新しい座標をセット
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//武器の位置を更新
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// 新しい向きをセット
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}

void Chara::updatePosition(float angle, Chara* otherArray,int vecSize){
	//変更する前のキャラの座標を保存しておく
	VECTOR tempPos = Position;

	//攻撃処理は除く
	if (!AttackFlag){
		// キャラクターの向きに合わせて移動ベクトルの角度を回転させて、キャラクターモデルの座標に加算
		SinParam = sin(-angle / 180.0f * DX_PI_F);
		CosParam = cos(-angle / 180.0f * DX_PI_F);
		Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
		Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;
	}

	//衝突処理
	CollisionOther(otherArray, vecSize);

	// ３Ｄモデルに新しい座標をセット
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//武器の位置を更新
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// 新しい向きをセット
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}

void Chara::continuationUpdate(float continueActionAngle, Chara other){
	// アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// アニメーション再生時間を進める
	AnimNowTime += 0.6f;

	// アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
	if (AnimNowTime >= AnimTotalTime)
	{
		//この処理がが子クラスによって違う
		endAnim();
	}
	else
	{
		continueAnim();
	}

	updatePosition(continueActionAngle, MoveVector, other);

}

void Chara::continuationUpdate(float continueActionAngle){
	// アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// アニメーション再生時間を進める
	AnimNowTime += 0.6f;

	// アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
	if (AnimNowTime >= AnimTotalTime)
	{
		//この処理がが子クラスによって違う
		endAnim();
	}
	else
	{
		continueAnim();
	}
}

void Chara::update(float angle, Chara other){
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
void Chara::onceUpdate(float angle){
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
}


void Chara::draw(){
	MV1DrawModel(ModelHandle);
	CapInfo cap = getHitCapInfo();
	//カプセル描画
	DrawCapsule3D(cap.CapsulePosition1, cap.CapsulePosition2, DamageHitWidth, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);

	if (WeaponHandle){
		MV1DrawModel(WeaponHandle);
	}
}

void Chara::terminateModel(){
	// モデルの削除
	MV1DeleteModel(ModelHandle);
}

int Chara::attackedkJudge(Chara other){
	int HitResult = FALSE;
	VECTOR nearPos[2];
	VECTOR farPos[2];
	//敵の攻撃座標計算
	MATRIX AttackPosMatrix =
		MV1GetFrameLocalWorldMatrix(other.ModelHandle, other.HandFrameIndex);

	// 攻撃判定で使用する２点を算出
	nearPos[0] = VTransform(VGet(0.0f, 0.0f, 0.0f), AttackPosMatrix);
	farPos[0] = VTransform(other.EndLocalPosition, AttackPosMatrix);

	//自分の攻撃当たり判定用カプセル情報を取得
	CapInfo selfCinfo = getDamageCapInfo();

	//他のキャラの攻撃と自分のカプセルが衝突したか
	HitResult =
		HitCheck_Capsule_Capsule(
		selfCinfo.CapsulePosition1,
		selfCinfo.CapsulePosition2,
		DamageHitWidth,
		nearPos[0],
		nearPos[1],
		other.AttackSphereSize
		);

	return HitResult;
}

CapInfo Chara::getHitCapInfo(){
	VECTOR CapsulePosition1;
	VECTOR CapsulePosition2;

	CapsulePosition1 =
		CapsulePosition2 = VAdd(Position, CharaHitCenterPosition);

	CapsulePosition1.y -= CharaHitHeight / 2.0f;
	CapsulePosition2.y += CharaHitHeight / 2.0f;

	return{ CapsulePosition1, CapsulePosition2 };
}

CapInfo Chara::getDamageCapInfo(){

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
	Chara::changeAnim(DamageAnim, ShortOption);
	//ダメージ用の音楽を流す
	PlaySoundFile(DamageSound, DX_PLAYTYPE_BACK);
}

//攻撃を受けているか
int Chara::isHitted(Chara other){
	int HitResult = attackedkJudge(other);

	if (HitResult && other.AttackFlag){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

//攻撃を受けているか
int Chara::isHitted(Chara* otherVec, int vecSize){
	int HitResult = FALSE;
	for (int i=0; i < vecSize; i++){
		Chara other = otherVec[i];
		//攻撃を受けていたら即座にTRUEを返す
		if (attackedkJudge(other) && other.AttackFlag){
			HitResult = TRUE;
			break;
		}
	}
	return HitResult;
}

// キャラ同士の当たり判定処理を行う
void Chara::CollisionOther(Chara other){

	VECTOR otherToThisVec;
	VECTOR PushVec;
	float  Distance;
	float  TotalWidth;
	float  TempY;
	CapInfo thisCInfo;
	CapInfo otherCInfo;

	// キャラ同士の幅の合計を算出
	TotalWidth =
		CharaHitWidth + other.CharaHitWidth;

	// 各キャラの当たり判定用のカプセル座標を算出
	thisCInfo = getHitCapInfo();
	otherCInfo = other.getHitCapInfo();

	// 当たっているか判定
	if (HitCheck_Capsule_Capsule(
		thisCInfo.CapsulePosition1, thisCInfo.CapsulePosition2,
		CharaHitWidth,
		otherCInfo.CapsulePosition1, otherCInfo.CapsulePosition2,
		other.CharaHitWidth) == TRUE)
	{
		// 当たっていたら thisが otherから離れる処理をする

		// otherからthisへのベクトルを算出
		otherToThisVec = VSub(Position, other.Position);

		// Ｙ軸は見ない
		otherToThisVec.y = 0.0f;

		// 二人の距離を算出
		Distance = VSize(otherToThisVec);

		// otherから this へのベクトルを正規化
		PushVec = VScale(otherToThisVec, 1.0f / Distance);

		// 押し出す距離を算出、もし二人の距離から二人の大きさを引いた値に押し出し力を
		// 足して離れてしまう場合は、ぴったりくっつく距離に移動する
		if (Distance - TotalWidth + CHARA_HIT_PUSH_POWER > 0.0f)
		{
			TempY = Position.y;
			Position = VAdd(other.Position, VScale(PushVec, TotalWidth));

			// Ｙ座標は変化させない
			Position.y = TempY;
		}
		else
		{
			// 押し出し
			Position = VAdd(Position, VScale(PushVec, CHARA_HIT_PUSH_POWER));
		}
	}

}
//player用
void Chara::CollisionOther(Chara* otherVec, int vecSize){

	VECTOR otherToThisVec;
	VECTOR PushVec;
	float  Distance;
	float  TotalWidth;
	float  TempY;
	CapInfo thisCInfo;
	CapInfo otherCInfo;

	for (int i = 0; i < vecSize; i++){

		Chara other = otherVec[i];

		// キャラ同士の幅の合計を算出
		TotalWidth =
			CharaHitWidth + other.CharaHitWidth;

		// 各キャラの当たり判定用のカプセル座標を算出
		thisCInfo = getHitCapInfo();
		otherCInfo = other.getHitCapInfo();

		// 当たっているか判定
		if (HitCheck_Capsule_Capsule(
			thisCInfo.CapsulePosition1, thisCInfo.CapsulePosition2,
			CharaHitWidth,
			otherCInfo.CapsulePosition1, otherCInfo.CapsulePosition2,
			other.CharaHitWidth) == TRUE)
		{

			// 当たっていたら thisが otherから離れる処理をする

			// otherからthisへのベクトルを算出
			otherToThisVec = VSub(Position, other.Position);

			// Ｙ軸は見ない
			otherToThisVec.y = 0.0f;

			// 二人の距離を算出
			Distance = VSize(otherToThisVec);

			// otherから this へのベクトルを正規化
			PushVec = VScale(otherToThisVec, 1.0f / Distance);

			// 押し出す距離を算出、もし二人の距離から二人の大きさを引いた値に押し出し力を
			// 足して離れてしまう場合は、ぴったりくっつく距離に移動する
			if (Distance - TotalWidth + CHARA_HIT_PUSH_POWER > 0.0f)
			{
				TempY = Position.y;
				Position = VAdd(other.Position, VScale(PushVec, TotalWidth));

				// Ｙ座標は変化させない
				Position.y = TempY;
			}
			else
			{
				// 押し出し
				Position = VAdd(Position, VScale(PushVec, CHARA_HIT_PUSH_POWER));
			}
		}
	}
}

