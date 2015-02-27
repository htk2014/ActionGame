#include "DxLib.h"
#include "util.h"

void SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle)
{
	MATRIX FrameMatrix;
	int FrameIndex;

	// フレーム名からフレーム番号を取得する
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// フレームの現在のワールドでの状態を示す行列を取得する
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// セットするモデルの状態を示す行列をフレームの状態を示す行列と同じにする
	MV1SetMatrix(SetModelHandle, FrameMatrix);
}
float getAnimTotalTime(int ModelHandle, int AnimHandle){
	int AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, AnimHandle, FALSE);
	float AttackTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);
	MV1DetachAnim(ModelHandle, AnimAttachIndex);
	return AttackTotalTime;
}


	// 指定の範囲からランダムで決定した値を取得する( 整数値 )
	//     戻り値 : ランダムで決定した値
	int GetRandomInt(
		// 最大値
		int MaxI,

		// 最小値
		int MinI
		)
	{
		return GetRand(MaxI - MinI) + MinI;
	}