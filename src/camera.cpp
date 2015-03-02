#include "camera.h"
#include <math.h>

// �J�����̉�]���x
#define CAMERA_ANGLE_SPEED		3.0f
// �J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT		80.0f
// �J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE		250.0f

Camera::Camera(VECTOR playerPos){
	// �J�����̃N���b�s���O������ݒ�
	HAngle = 0.0f;
	VAngle = 10.0f;
	// �J�����̈ʒu�ƌ�����ݒ�
	//SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 500.0f, -1000.0f), VGet(0.0f, 400.0f, 0.0f));
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 500.0f, -1000.0f), playerPos);
	SetCameraNearFar(100.0f, 5000.0f);

}
void Camera::update(VECTOR playerPos){
	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;

	// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
	CameraLookAtPosition = playerPos;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// �J�����̈ʒu�̓J�����̐����p�x�Ɛ����p�x����Z�o

	// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
	SinParam = sin(VAngle / 180.0f * DX_PI_F);
	CosParam = cos(VAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0;
	TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

	// ���ɐ����p�x�𔽉f�����ʒu���Z�o
	SinParam = sin(HAngle / 180.0f * DX_PI_F);
	CosParam = cos(HAngle / 180.0f * DX_PI_F);
	TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

	// �J�����̐ݒ�ɔ��f����
	SetCameraPositionAndTarget_UpVecY(CameraPosition, VGet(playerPos.x, 0.0f, playerPos.z));
}

void Camera::update(VECTOR playerPos,float hAngle){
	VECTOR TempPosition1;
	VECTOR TempPosition2;
	VECTOR CameraPosition;
	VECTOR CameraLookAtPosition;

	// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
	CameraLookAtPosition = playerPos;
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
	SinParam = sin(VAngle / 180.0f * DX_PI_F);
	CosParam = cos(VAngle / 180.0f * DX_PI_F);
	TempPosition1.x = 0.0f;
	TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE+100;
	TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

	// ���ɐ����p�x�𔽉f�����ʒu���Z�o
	float hoge = hAngle / 180.0f * DX_PI_F - DX_PI_F;

	SinParam = sin(hAngle / 180.0f * DX_PI_F);
	CosParam = cos(hAngle / 180.0f * DX_PI_F);
	//TempPosition2.x = (SinParam * TempPosition1.z - CosParam * TempPosition1.x);
	TempPosition2.x = (SinParam * TempPosition1.z - CosParam * TempPosition1.x);
	//TempPosition2.x = -(CosParam * TempPosition1.x - SinParam * TempPosition1.z);
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
	CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);
	
	// �J�����̐ݒ�ɔ��f����
	SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
}

