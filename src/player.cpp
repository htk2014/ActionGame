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
	//�O��U���L�[��������Ă��邩
	LastAttackKeyPressed = FALSE;
	LastRockOnKeyPressed = FALSE;
	//�����Ă��邩
	RunFlag = FALSE;
	ShortOption = FALSE;
	//�U�����Ă���A�j���[�V����
	AttackState = FALSE;
	AttackContinueFlag = FALSE;
	AttackContinueNum = 0;
	//����A�j���[�V�����ǂݍ���
	RunAnim = MV1LoadModel("Data/Anim_Run.mv1");
	//�U���A�j���[�V�����ǂݍ���
	AttackAnim = MV1LoadModel("Data/Anim_Attack1.mv1");
	AttackAnim2 = MV1LoadModel("Data/Anim_Attack2.mv1");
	AttackAnim3 = MV1LoadModel("Data/Anim_Attack3.mv1");
	//�A���U���p�A�j���[�V�����z��
	AttackAnimArray[0] = AttackAnim;
	AttackAnimArray[1] = AttackAnim2;
	AttackAnimArray[2] = AttackAnim3;

	EndLocalPosition = { 0.0, -100, 0.0 };

	//��U������p
	DamageHitWidth = 50.0;
	DamageHitHeight = 80.0;
	DamageHitCenterPosition = { 0.0, 90.0, 0.0 };

	DamageSound = "Data/sound/dmg_byAxe_00.wav";

	AttackSphereSize = 1.0;
	HP = 100;
	//���C�t�o�[
	LBar = LifeBar::LifeBar();

	TargetNum = 0;
	TargetEnemyPos = VGet(0.0f, 0.0f, 0.0f);

	Chara::animInit();
}

void Player::changeAnim(int AnimFlag, int Anim){
	// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`
	MV1DetachAnim(ModelHandle, AnimAttachIndex);

	// �V�����A�j���[�V�������A�^�b�`
	if (AnimFlag)
	{
		AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, Anim, FALSE);
	}
	else
	{
		AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, NeutralAnim, FALSE);
	}

	// �A�j���[�V�����̑����Ԃ��擾���Ă���
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// �A�j���[�V�����Đ����Ԃ�������
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);
}

//�A���U���p�A�j���[�V�����ύX���s��
void Player::changeAnim(int AnimNum){
	int Anim = AttackAnimArray[AnimNum];
	Chara::changeAnim(Anim);
}


void Player::attackAnimInit(float angle){
	//�U�������Ă���t���O�𗧂Ă�
	AttackFlag = TRUE;
	//�U���A�j���[�V������Ԃ��U���A�j���[�V�����P�ɂ���
	AttackState = AttackAnimArray[AttackContinueNum];
	//�U���A���O����������
	AttackAngle = angle;
	//�ŏ��̍U���A�j���[�V�����ɕύX
	Chara::changeAnim(AttackAnim);
}

void Player::setupContinueAttack(){
	AttackContinueFlag = FALSE;
	//�U���񐔍X�V
	AttackContinueNum++;
	//���̍U���A�j���[�V�����ɕύX
	changeAnim(AttackContinueNum);
}

//continutionUpdate����endAnim���v���C���[�̍U���A�j���[�V�����悤�ɏ���������
void Player::endAnim(){
	Chara::endAnim();

	if (DamageFlag){
		DamageFlag = FALSE;
	}
	else if (AttackFlag){
		//�U���t���O��܂�
		AttackFlag = FALSE;
		//�U���A�j���[�V�����̏�ԏ�����
		AttackState = FALSE;
		//�A���U���񐔏�����
		AttackContinueNum = 0;
		AttackContinueFlag = FALSE;
	}
}

void Player::setupAttack(KeyInfo KInfo, float angle){
	//�U���L�[��������Ă��āA�U���t���O���܂������Ă��Ȃ�������U���A�j���[�V�����̏���������
	if (KInfo.AttackKeyPressed && !AttackFlag){
		attackAnimInit(angle);
		//rockOn();
	}
	//�U���L�[��������Ă��āA�U���t���O�����łɗ����Ă���Ȃ�A���U���t���O�𗧂Ă�
	//�A���U���̓L�[����x�����Ă��炶��Ȃ��Ƃł��Ȃ��悤�ɂ���(�O��̃L�[��FALSE -> !LastAttackKeyPressed )
	else if (!LastAttackKeyPressed && KInfo.AttackKeyPressed && AttackFlag){
		AttackContinueFlag = TRUE;
	}
}

void Player::update(KeyInfo KInfo, float angle, Chara* enemyVec, int vecSize){
	int AttackHitted = isHitted(enemyVec, vecSize);

	if (!DamageFlag){
		setupAttack(KInfo, angle);
	}

	if (AttackHitted && !DamageFlag){
		initDamageAnim();
		HP -= 10;
	}

	//�U���t���O�������Ă���Ȃ�U�����n�߂�B�܂��͍U�����_���[�W�A�j���[�V�������p��
	if (AttackFlag || DamageFlag){
		//�A���U���t���O�������Ă���Ȃ玟�̃A�j���[�V�����Ɉڍs
		if (LastAttackKeyPressed != KInfo.AttackKeyPressed && KInfo.AttackKeyPressed && AttackFlag){
			//�A���U�����܂��������邩�H
			if (AttackContinueFlag && AttackContinueNum < 2){
				//�A�j���[�V�����ύX
				setupContinueAttack();
			}
		}
		if (AttackFlag){
			rockOn();
		}
		//Chara::continuationUpdate(AttackAngle, enemyVec,vecSize);
		Chara::RockOncontinuationUpdate(AttackAngle, enemyVec, vecSize);
		//Chara::continuationUpdate(AttackAngle, enemyVec, vecSize);
		
	}
	else if (KInfo.RockOnKeyPressed && LastRockOnKeyPressed != KInfo.RockOnKeyPressed){
		//���b�N�I������^�[�Q�b�g��ς���
		changeTarget(enemyVec, vecSize);
		rockOn();
		//���b�N�I�����Ă���G�̕����Ɍ�������
		//float rockOnAngle = getRockOnAngle(Position, TargetEnemyPos);
		//MV1SetRotationXYZ(ModelHandle, VGet(0.0f, rockOnAngle + DX_PI_F, 0.0f));
		MV1SetRotationXYZ(ModelHandle, VGet(0.0f, AttackAngle / 180.0f * DX_PI_F + DX_PI_F, 0.0f));
		
		//AttackAngle = rockOnAngle;
	}
	else{
		if (RunFlag != KInfo.MoveKeyPressed)
		{
			RunFlag = KInfo.MoveKeyPressed;
			changeAnim(RunFlag, RunAnim);
		}
		
		if (KInfo.MoveKeyPressed){
			//�ړ��t���O�ɕύX�����邩�H
			if (RunFlag != KInfo.MoveKeyPressed)
			{
				RunFlag = KInfo.MoveKeyPressed;
				changeAnim(RunFlag, RunAnim);
			}
			///Chara::RockOnOnceUpdate(AttackAngle, enemyVec, vecSize);
			rockOn();
			Chara::onceUpdate(angle+AttackAngle, enemyVec, vecSize);
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0f, AttackAngle + DX_PI_F, 0.0f));
		}
		//�j���[�g����
		else{
			Chara::RockOnOnceUpdate(AttackAngle, enemyVec, vecSize);
			rockOn();
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0f, AttackAngle + DX_PI_F, 0.0f));
			MV1SetRotationXYZ(ModelHandle, VGet(0.0f, AttackAngle / 180.0f * DX_PI_F + DX_PI_F, 0.0f));
		}

	}
	float hoge;
	hoge = AttackAngle * 180 / DX_PI_F;

	//���C�t�o�[�X�V
	LBar.updateHP(HP);
	//�A���U���p�ߋ��L�[���X�V
	LastAttackKeyPressed = KInfo.AttackKeyPressed;
	LastRockOnKeyPressed = KInfo.RockOnKeyPressed;

}

void Player::draw(){
	Chara::draw();
	LBar.draw();
}

void Player::rockOn(){
	AttackAngle = getRockOnAngle(Position, TargetEnemyPos) * 180 / DX_PI_F;
	//AttackAngle = (getRockOnAngle(Position, TargetEnemyPos) + DX_PI_F) * 180 / DX_PI_F;
}

void Player::changeTarget(Chara* enemyVec, int EnemyMaxNum){
	TargetEnemyPos = enemyVec[TargetNum].Position;

	if (TargetNum >= EnemyMaxNum-1){
		TargetNum = 0;
	}
	else {
		TargetNum++;
	}

}