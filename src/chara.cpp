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
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// �V�����A�j���[�V�������A�^�b�`
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// �A�j���[�V�����̑����Ԃ��擾���Ă���
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// �A�j���[�V�����Đ����Ԃ�������
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

void Chara::changeAnim(int anim,int shortFlag){
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// �V�����A�j���[�V�������A�^�b�`
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// �A�j���[�V�����̑����Ԃ̔������擾
	if (shortFlag){
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex)/2;
	} else{
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);
	}
	// �A�j���[�V�����Đ����Ԃ�������
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

void Chara::animInit(){
	// �ҋ@�A�j���[�V�������A�^�b�`
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, NeutralAnim, FALSE);

	// �ҋ@�A�j���[�V�����̑����Ԃ��擾���Ă���
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// �A�j���[�V�����Đ����Ԃ�������
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

	// �A�j���[�V�����ňړ������Ă���t���[���̔ԍ�����������
	MoveAnimFrameIndex = MV1SearchFrame(ModelHandle, RootFrame);

	// �A�j���[�V�����ňړ������Ă���t���[���𖳌��ɂ���
	MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, MV1GetFrameLocalMatrix(ModelHandle, MoveAnimFrameIndex));

	// �R�c���f���̍��W��������
	Position = VGet(0.0f, 0.0f, 0.0f);
}

void Chara::endAnim(){
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime);
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VSub(NowPosition, PrevPosition);

	// �V�����A�j���[�V�����Đ����Ԃ́A�A�j���[�V�����Đ����Ԃ���A�j���[�V���������Ԃ�����������
	AnimNowTime -= AnimTotalTime;

	// ���Ɂw�V�����A�j���[�V�����Đ����Ԃł́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v��
	// �w�A�j���[�V�����Đ����ԂO�ł́u�A�j���[�V�����ňړ����Ă���t���[���̍��W�v�x�Ƃ̍������ړ��x�N�g���ɉ��Z����
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0f);
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VAdd(MoveVector, VSub(NowPosition, PrevPosition));

	// �j���[�g�����A�j���[�V�����ɕύX
	changeAnim(NeutralAnim);
}

void Chara::continueAnim(){
	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

	// �w�V�����A�j���[�V�����Đ����Ԃł́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v�x�ƁA
	// �w�A�j���[�V�����Đ����Ԃ�i�߂�O�́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v�x�Ƃ̍������ړ��x�N�g���Ƃ���
	NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
	MoveVector = VSub(NowPosition, PrevPosition);


}

void Chara::updatePosition(float angle, VECTOR moveVector,Chara other){
	//�ύX����O�̃L�����̍��W��ۑ����Ă���
	VECTOR tempPos = Position;

	// �L�����N�^�[�̌����ɍ��킹�Ĉړ��x�N�g���̊p�x����]�����āA�L�����N�^�[���f���̍��W�ɉ��Z
	SinParam = sin(-angle / 180.0f * DX_PI_F);
	CosParam = cos(-angle / 180.0f * DX_PI_F);
	Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
	Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;

	int ColliWithOther = CollisionOther(other);
	//���̃L�����ƏՓ˂��Ă���Ȃ�A���W�����ɖ߂�
	if (ColliWithOther){
		Position = tempPos;
		Position.x -= (MoveVector.x * CosParam - MoveVector.z * SinParam)/5;
		Position.z -= (MoveVector.x * SinParam + MoveVector.z * CosParam)/5;
	}
	// �R�c���f���ɐV�������W���Z�b�g
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//����̈ʒu���X�V
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// �V�����������Z�b�g
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}
/*
void Chara::updatePosition(float angle, VECTOR moveVector){
	//�ύX����O�̃L�����̍��W��ۑ����Ă���
	VECTOR tempPos = Position;

	// �L�����N�^�[�̌����ɍ��킹�Ĉړ��x�N�g���̊p�x����]�����āA�L�����N�^�[���f���̍��W�ɉ��Z
	SinParam = sin(-angle / 180.0f * DX_PI_F);
	CosParam = cos(-angle / 180.0f * DX_PI_F);
	Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
	Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;

	// �R�c���f���ɐV�������W���Z�b�g
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//����̈ʒu���X�V
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// �V�����������Z�b�g
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}
*/

void Chara::continuationUpdate(float continueActionAngle, Chara other){
	// �A�j���[�V�������Ԃ�i�߂�O�́w�A�j���[�V�����ňړ������Ă���t���[���̍��W�x���擾���Ă���
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// �A�j���[�V�����Đ����Ԃ�i�߂�
	AnimNowTime += 0.6f;

	// �A�j���[�V�����Đ����Ԃ��A�j���[�V�����̑����Ԃ��z���Ă��邩�ǂ����ŏ����𕪊�
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
	// �A�j���[�V�������Ԃ�i�߂�O�́w�A�j���[�V�����ňړ������Ă���t���[���̍��W�x���擾���Ă���
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// �A�j���[�V�����Đ����Ԃ�i�߂�
	AnimNowTime += 0.6f;

	// �A�j���[�V�����Đ����Ԃ��A�j���[�V�����̑����Ԃ��z���Ă��邩�ǂ����ŏ����𕪊�
	if (AnimNowTime >= AnimTotalTime)
	{
		// �����Ă���ꍇ�́A�܂��w�A�j���[�V�����Đ����Ԃ�i�߂�O�́u�A�j���[�V�����ňړ����Ă���t���[���̍��W�v�x�ƁA
		// �w�A�j���[�V�����̏I�[�ł́u�A�j���[�V�����ňړ����Ă���t���[���̍��W�v�x�Ƃ̍������ړ��x�N�g���Ƃ���
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime);
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VSub(NowPosition, PrevPosition);

		// �V�����A�j���[�V�����Đ����Ԃ́A�A�j���[�V�����Đ����Ԃ���A�j���[�V���������Ԃ�����������
		AnimNowTime -= AnimTotalTime;

		// ���Ɂw�V�����A�j���[�V�����Đ����Ԃł́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v��
		// �w�A�j���[�V�����Đ����ԂO�ł́u�A�j���[�V�����ňړ����Ă���t���[���̍��W�v�x�Ƃ̍������ړ��x�N�g���ɉ��Z����
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0f);
		PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VAdd(MoveVector, VSub(NowPosition, PrevPosition));
	}
	else
	{
		// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

		// �w�V�����A�j���[�V�����Đ����Ԃł́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v�x�ƁA
		// �w�A�j���[�V�����Đ����Ԃ�i�߂�O�́u�A�j���[�V�����ňړ������Ă���t���[���̍��W�v�x�Ƃ̍������ړ��x�N�g���Ƃ���
		NowPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);
		MoveVector = VSub(NowPosition, PrevPosition);
	}
	updatePosition(angle, MoveVector, other);


}

void Chara::draw(){
	MV1DrawModel(ModelHandle);
	CapInfo cap = getCapInfo();
	//�J�v�Z���`��
	DrawCapsule3D(cap.CapsulePosition1, cap.CapsulePosition2,DamageHitWidth, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
	
	if (WeaponHandle){
		MV1DrawModel(WeaponHandle);
	}
}

void Chara::terminateModel(){
	// ���f���̍폜
	MV1DeleteModel(ModelHandle);
}

int Chara::attackJudge(CapInfo Cinfo, float EnemyHitWidth){
	int HitResult = FALSE;
	VECTOR nearPos[2];
	VECTOR farPos[2];

	MATRIX AttackPosMatrix =
		MV1GetFrameLocalWorldMatrix(ModelHandle, HandFrameIndex);

	// �U������Ŏg�p����Q�_���Z�o
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
	//�U�����󂯂Ă���t���O�𗧂Ă�
	DamageFlag = TRUE;
	//�_���[�W�A�j���[�V�����ɕύX(ShortOption��TRUE�Ȃ�_���[�W�A�j���̒ʏ�̔����̎��ԂɁ@���R:�܂�܂��Ɩ��G���Ԃ��������邩��j
	Chara::changeAnim(DamageAnim,ShortOption);
	//�_���[�W�p�̉��y�𗬂�
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
	//���̃L�����̃J�v�Z�����
	CapInfo otherCinfo = other.getCapInfo();
	//�L�������m�̓����蔻��
	MV1SetupCollInfo(ModelHandle, -1, 8, 8, 8);
	//�J�v�Z���̑傫������
	otherCinfo.CapsulePosition1.x = otherCinfo.CapsulePosition1.x / 100;
	otherCinfo.CapsulePosition1.y = otherCinfo.CapsulePosition1.y / 100;
	otherCinfo.CapsulePosition2.x = otherCinfo.CapsulePosition2.x / 100;
	otherCinfo.CapsulePosition2.y = otherCinfo.CapsulePosition2.y / 100;
	//���̃L�����̃J�v�Z���Ǝ������Փ˂�����
	auto HitPolyDim = MV1CollCheck_Capsule(ModelHandle, -1, otherCinfo.CapsulePosition1, otherCinfo.CapsulePosition1, 100.0f);
	

	//�����蔻��p�\���̂̌�n��
	MV1CollResultPolyDimTerminate(HitPolyDim);

	//HitPolyDim.HitNum���P�ȏ�Ȃ�Փ˂��Ă���
	if (HitPolyDim.HitNum >= 1){
		return TRUE;
	}
	else{
		return FALSE;
	}
}