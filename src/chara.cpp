#include "math.h"
#include "chara.h"

// �L�����N�^�[���m�œ��������Ƃ��̉����o������
#define CHARA_HIT_PUSH_POWER            (12.0f)

LifeBar::LifeBar(){
	DamageColor = GetColor(255, 0, 0);    // �_���[�W�o�[�̐F
	HPColor = GetColor(0, 0, 255);    // HP�o�[�̐F
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
	DamageColor = GetColor(255, 0, 0);    // �_���[�W�o�[�̐F
	HPColor = GetColor(0, 0, 255);    // HP�o�[�̐F
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

void Chara::changeAnim(int anim, int shortFlag){
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// �V�����A�j���[�V�������A�^�b�`
	AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	// �A�j���[�V�����̑����Ԃ̔������擾
	if (shortFlag){
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex) / 2;
	}
	else{
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

void Chara::updatePosition(float angle, VECTOR moveVector, Chara other){
	//�ύX����O�̃L�����̍��W��ۑ����Ă���
	VECTOR tempPos = Position;

	//�U�������͏���
	if (!AttackFlag){
		// �L�����N�^�[�̌����ɍ��킹�Ĉړ��x�N�g���̊p�x����]�����āA�L�����N�^�[���f���̍��W�ɉ��Z
		SinParam = sin(-angle / 180.0f * DX_PI_F);
		CosParam = cos(-angle / 180.0f * DX_PI_F);
		Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
		Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;
	}

	//�Փˏ���
	CollisionOther(other);

	// �R�c���f���ɐV�������W���Z�b�g
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//����̈ʒu���X�V
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// �V�����������Z�b�g
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}

void Chara::updatePosition(float angle, Chara* otherArray,int vecSize){
	//�ύX����O�̃L�����̍��W��ۑ����Ă���
	VECTOR tempPos = Position;

	//�U�������͏���
	if (!AttackFlag){
		// �L�����N�^�[�̌����ɍ��킹�Ĉړ��x�N�g���̊p�x����]�����āA�L�����N�^�[���f���̍��W�ɉ��Z
		SinParam = sin(-angle / 180.0f * DX_PI_F);
		CosParam = cos(-angle / 180.0f * DX_PI_F);
		Position.x += MoveVector.x * CosParam - MoveVector.z * SinParam;
		Position.z += MoveVector.x * SinParam + MoveVector.z * CosParam;
	}

	//�Փˏ���
	CollisionOther(otherArray, vecSize);

	// �R�c���f���ɐV�������W���Z�b�g
	MV1SetPosition(ModelHandle, Position);

	if (WeaponHandle){
		//����̈ʒu���X�V
		SetModelFramePosition(ModelHandle, HandFrameName, WeaponHandle);
	}

	// �V�����������Z�b�g
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, angle / 180.0f * DX_PI_F, 0.0f));
}

void Chara::continuationUpdate(float continueActionAngle, Chara other){
	// �A�j���[�V�������Ԃ�i�߂�O�́w�A�j���[�V�����ňړ������Ă���t���[���̍��W�x���擾���Ă���
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// �A�j���[�V�����Đ����Ԃ�i�߂�
	AnimNowTime += 0.6f;

	// �A�j���[�V�����Đ����Ԃ��A�j���[�V�����̑����Ԃ��z���Ă��邩�ǂ����ŏ����𕪊�
	if (AnimNowTime >= AnimTotalTime)
	{
		//���̏��������q�N���X�ɂ���ĈႤ
		endAnim();
	}
	else
	{
		continueAnim();
	}

	updatePosition(continueActionAngle, MoveVector, other);

}

void Chara::continuationUpdate(float continueActionAngle){
	// �A�j���[�V�������Ԃ�i�߂�O�́w�A�j���[�V�����ňړ������Ă���t���[���̍��W�x���擾���Ă���
	PrevPosition = MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex);

	// �A�j���[�V�����Đ����Ԃ�i�߂�
	AnimNowTime += 0.6f;

	// �A�j���[�V�����Đ����Ԃ��A�j���[�V�����̑����Ԃ��z���Ă��邩�ǂ����ŏ����𕪊�
	if (AnimNowTime >= AnimTotalTime)
	{
		//���̏��������q�N���X�ɂ���ĈႤ
		endAnim();
	}
	else
	{
		continueAnim();
	}
}

void Chara::update(float angle, Chara other){
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
void Chara::onceUpdate(float angle){
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
}


void Chara::draw(){
	MV1DrawModel(ModelHandle);
	CapInfo cap = getHitCapInfo();
	//�J�v�Z���`��
	DrawCapsule3D(cap.CapsulePosition1, cap.CapsulePosition2, DamageHitWidth, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);

	if (WeaponHandle){
		MV1DrawModel(WeaponHandle);
	}
}

void Chara::terminateModel(){
	// ���f���̍폜
	MV1DeleteModel(ModelHandle);
}

int Chara::attackedkJudge(Chara other){
	int HitResult = FALSE;
	VECTOR nearPos[2];
	VECTOR farPos[2];
	//�G�̍U�����W�v�Z
	MATRIX AttackPosMatrix =
		MV1GetFrameLocalWorldMatrix(other.ModelHandle, other.HandFrameIndex);

	// �U������Ŏg�p����Q�_���Z�o
	nearPos[0] = VTransform(VGet(0.0f, 0.0f, 0.0f), AttackPosMatrix);
	farPos[0] = VTransform(other.EndLocalPosition, AttackPosMatrix);

	//�����̍U�������蔻��p�J�v�Z�������擾
	CapInfo selfCinfo = getDamageCapInfo();

	//���̃L�����̍U���Ǝ����̃J�v�Z�����Փ˂�����
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
	//�U�����󂯂Ă���t���O�𗧂Ă�
	DamageFlag = TRUE;
	//�_���[�W�A�j���[�V�����ɕύX(ShortOption��TRUE�Ȃ�_���[�W�A�j���̒ʏ�̔����̎��ԂɁ@���R:�܂�܂��Ɩ��G���Ԃ��������邩��j
	Chara::changeAnim(DamageAnim, ShortOption);
	//�_���[�W�p�̉��y�𗬂�
	PlaySoundFile(DamageSound, DX_PLAYTYPE_BACK);
}

//�U�����󂯂Ă��邩
int Chara::isHitted(Chara other){
	int HitResult = attackedkJudge(other);

	if (HitResult && other.AttackFlag){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

//�U�����󂯂Ă��邩
int Chara::isHitted(Chara* otherVec, int vecSize){
	int HitResult = FALSE;
	for (int i=0; i < vecSize; i++){
		Chara other = otherVec[i];
		//�U�����󂯂Ă����瑦����TRUE��Ԃ�
		if (attackedkJudge(other) && other.AttackFlag){
			HitResult = TRUE;
			break;
		}
	}
	return HitResult;
}

// �L�������m�̓����蔻�菈�����s��
void Chara::CollisionOther(Chara other){

	VECTOR otherToThisVec;
	VECTOR PushVec;
	float  Distance;
	float  TotalWidth;
	float  TempY;
	CapInfo thisCInfo;
	CapInfo otherCInfo;

	// �L�������m�̕��̍��v���Z�o
	TotalWidth =
		CharaHitWidth + other.CharaHitWidth;

	// �e�L�����̓����蔻��p�̃J�v�Z�����W���Z�o
	thisCInfo = getHitCapInfo();
	otherCInfo = other.getHitCapInfo();

	// �������Ă��邩����
	if (HitCheck_Capsule_Capsule(
		thisCInfo.CapsulePosition1, thisCInfo.CapsulePosition2,
		CharaHitWidth,
		otherCInfo.CapsulePosition1, otherCInfo.CapsulePosition2,
		other.CharaHitWidth) == TRUE)
	{
		// �������Ă����� this�� other���痣��鏈��������

		// other����this�ւ̃x�N�g�����Z�o
		otherToThisVec = VSub(Position, other.Position);

		// �x���͌��Ȃ�
		otherToThisVec.y = 0.0f;

		// ��l�̋������Z�o
		Distance = VSize(otherToThisVec);

		// other���� this �ւ̃x�N�g���𐳋K��
		PushVec = VScale(otherToThisVec, 1.0f / Distance);

		// �����o���������Z�o�A������l�̋��������l�̑傫�����������l�ɉ����o���͂�
		// �����ė���Ă��܂��ꍇ�́A�҂����肭���������Ɉړ�����
		if (Distance - TotalWidth + CHARA_HIT_PUSH_POWER > 0.0f)
		{
			TempY = Position.y;
			Position = VAdd(other.Position, VScale(PushVec, TotalWidth));

			// �x���W�͕ω������Ȃ�
			Position.y = TempY;
		}
		else
		{
			// �����o��
			Position = VAdd(Position, VScale(PushVec, CHARA_HIT_PUSH_POWER));
		}
	}

}
//player�p
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

		// �L�������m�̕��̍��v���Z�o
		TotalWidth =
			CharaHitWidth + other.CharaHitWidth;

		// �e�L�����̓����蔻��p�̃J�v�Z�����W���Z�o
		thisCInfo = getHitCapInfo();
		otherCInfo = other.getHitCapInfo();

		// �������Ă��邩����
		if (HitCheck_Capsule_Capsule(
			thisCInfo.CapsulePosition1, thisCInfo.CapsulePosition2,
			CharaHitWidth,
			otherCInfo.CapsulePosition1, otherCInfo.CapsulePosition2,
			other.CharaHitWidth) == TRUE)
		{

			// �������Ă����� this�� other���痣��鏈��������

			// other����this�ւ̃x�N�g�����Z�o
			otherToThisVec = VSub(Position, other.Position);

			// �x���͌��Ȃ�
			otherToThisVec.y = 0.0f;

			// ��l�̋������Z�o
			Distance = VSize(otherToThisVec);

			// other���� this �ւ̃x�N�g���𐳋K��
			PushVec = VScale(otherToThisVec, 1.0f / Distance);

			// �����o���������Z�o�A������l�̋��������l�̑傫�����������l�ɉ����o���͂�
			// �����ė���Ă��܂��ꍇ�́A�҂����肭���������Ɉړ�����
			if (Distance - TotalWidth + CHARA_HIT_PUSH_POWER > 0.0f)
			{
				TempY = Position.y;
				Position = VAdd(other.Position, VScale(PushVec, TotalWidth));

				// �x���W�͕ω������Ȃ�
				Position.y = TempY;
			}
			else
			{
				// �����o��
				Position = VAdd(Position, VScale(PushVec, CHARA_HIT_PUSH_POWER));
			}
		}
	}
}

