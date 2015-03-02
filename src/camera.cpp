#include "camera.h"
#include <math.h>

// カメラの回転速度
#define CAMERA_ANGLE_SPEED		3.0f
// カメラの注視点の高さ
#define CAMERA_LOOK_AT_HEIGHT		80.0f
// カメラと注視点の距離
#define CAMERA_LOOK_AT_DISTANCE		250.0f

Camera::Camera(VECTOR playerPos){
	// カメラのクリッピング距離を設定
	HAngle = 0.0f;
	VAngle = 10.0f;
	// カメラの位置と向きを設定
	//SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 500.0f, -1000.0f), VGet(0.0f, 400.0f, 0.0f));
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 500.0f, -1000.0f), playerPos);
	SetCameraNearFar(100.0f, 5000.0f);

}
void Camera::update(VECTOR playerPos){
	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;

	// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
	CameraLookAtPosition = playerPos;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// カメラの位置はカメラの水平角度と垂直角度から算出

	// 最初に垂直角度を反映した位置を算出
	SinParam = sin(VAngle / 180.0f * DX_PI_F);
	CosParam = cos(VAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0;
	TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

	// 次に水平角度を反映した位置を算出
	SinParam = sin(HAngle / 180.0f * DX_PI_F);
	CosParam = cos(HAngle / 180.0f * DX_PI_F);
	TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// 算出した座標に注視点の位置を加算したものがカメラの位置
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(CameraPosition, VGet(playerPos.x, 0.0f, playerPos.z));
}

void Camera::update(VECTOR playerPos,float hAngle){
	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;

	// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
	CameraLookAtPosition = playerPos;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// 最初に垂直角度を反映した位置を算出
	SinParam = sin(VAngle / 180.0f * DX_PI_F);
	CosParam = cos(VAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0.0f;
	TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE+100;
	TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

	// 次に水平角度を反映した位置を算出
	float hoge = hAngle / 180.0f * DX_PI_F - DX_PI_F;

	SinParam = sin(hAngle / 180.0f * DX_PI_F);
	CosParam = cos(hAngle / 180.0f * DX_PI_F);
	//TempPosition2.x = (SinParam * TempPosition1.z - CosParam * TempPosition1.x);
	TempPosition2.x = (SinParam * TempPosition1.z - CosParam * TempPosition1.x);
	//TempPosition2.x = -(CosParam * TempPosition1.x - SinParam * TempPosition1.z);
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// 算出した座標に注視点の位置を加算したものがカメラの位置
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);
	
	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
}

