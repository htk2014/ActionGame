#include "DxLib.h"
#include <math.h>
#include "util.h"

void SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle)
{
	MATRIX FrameMatrix;
	int FrameIndex;

	// �t���[��������t���[���ԍ����擾����
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// �t���[���̌��݂̃��[���h�ł̏�Ԃ������s����擾����
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// �Z�b�g���郂�f���̏�Ԃ������s����t���[���̏�Ԃ������s��Ɠ����ɂ���
	MV1SetMatrix(SetModelHandle, FrameMatrix);
}
float getAnimTotalTime(int ModelHandle, int AnimHandle){
	int AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, AnimHandle, FALSE);
	float AttackTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);
	MV1DetachAnim(ModelHandle, AnimAttachIndex);
	return AttackTotalTime;
}


	// �w��͈̔͂��烉���_���Ō��肵���l���擾����( �����l )
	//     �߂�l : �����_���Ō��肵���l
	int GetRandomInt(
		// �ő�l
		int MaxI,

		// �ŏ��l
		int MinI
		)
	{
		return GetRand(MaxI - MinI) + MinI;
	}

	// �L�������w��̍��W�̕����Ɍ�������
	float getRockOnAngle(VECTOR selfPosition,VECTOR targetPosition){
		VECTOR targetVector;
		float angle;

		// �L�����̍��W����w��̍��W�ւ̃x�N�g�����Z�o
		targetVector = VSub(targetPosition, selfPosition);

		// atan2 ���g�p���ĕ����x�N�g������p�x�����߂�
		angle = atan2(targetVector.x, targetVector.z);

		return angle;
	}