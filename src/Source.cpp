#include <iostream>

#include "DxLib.h"

#include "player.h"
#include "enemy.h"
#include "camera.h"

// �J�����̉�]���x
#define CAMERA_ANGLE_SPEED		3.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int    Input;
	float  Angle;
	//�U�����G�ɂ���������
	int AttackHit = FALSE;
	KeyInfo PlayerKeyInfo;
	VECTOR Position = VGet(500.0f, 0.0f, 0.0f);

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);
	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;
	//�v���C���[�������p�̍\����
	//�v���C���[�I�u�W�F�N�g����
	Player player = Player();
	player.Position = Position;
	//�G�I�u�W�F�N�g����
	Goblin goblin1 = Goblin({ 520, 0, 1 });

	int TestModelHandle = MV1LoadModel("Data/testCube2.mv1");
	//int anim = MV1LoadModel("Data/stable/testModel1_anim.mv1");
	  //AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	int stage = MV1LoadModel("Data/stage/Stage00.mv1");
	MV1SetPosition(stage, { 0, 0, 0 });
	MV1SetPosition(TestModelHandle, { 0, 0, 0 });

	// ������������
	Angle = 0.0f;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//�J��������
	Camera camera = Camera();

	// �w�i�̐F���D�F�ɂ���
	SetBackgroundColor(0, 0, 0);

	// ���C�����[�v(�����L�[�������ꂽ�烋�[�v�𔲂���)
	while (ProcessMessage() == 0)
	{
		// ��ʂ̃N���A
		ClearDrawScreen();

		// ZCSX�L�[�ŃJ�����̑���
		if (CheckHitKey(KEY_INPUT_C) == 1)
		{
			camera.HAngle += CAMERA_ANGLE_SPEED;
			if (camera.HAngle >= 180.0f)
			{
				camera.HAngle -= 360.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_Z) == 1)
		{
			camera.HAngle -= CAMERA_ANGLE_SPEED;
			if (camera.HAngle <= -180.0f)
			{
				camera.HAngle += 360.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_S) == 1)
		{
			camera.VAngle += CAMERA_ANGLE_SPEED;
			if (camera.VAngle >= 80.0f)
			{
				camera.VAngle = 80.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			camera.VAngle -= CAMERA_ANGLE_SPEED;
			if (camera.VAngle <= 0.0f)
			{
				camera.VAngle = 0.0f;
			}
		}
		// �U�����Ă��邩�ǂ����̃t���O��|��
		PlayerKeyInfo.AttackKeyPressed = FALSE;
		//�U���L�[�������ꂽ���H
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)
		{
			PlayerKeyInfo.AttackKeyPressed = TRUE;
		}

		// �Q�[���p�b�h�{�L�[�{�[�h�̓��͂��擾
		Input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// �ړ����Ă��邩�ǂ����̃t���O��|��
		PlayerKeyInfo.MoveKeyPressed = FALSE;
		//�U���t���O�������Ă��Ȃ����H
		if (!PlayerKeyInfo.AttackKeyPressed){
			// �������͂̔���
			// ( �ŏ��Ɏ΂ߓ��͂̔�����A���̌�㉺���E�P�̂̓��͂̔�������Ă��܂� )
			if ((Input & PAD_INPUT_LEFT) != 0 && (Input & PAD_INPUT_UP) != 0)
			{
				Angle = 135.0f;
				PlayerKeyInfo.MoveKeyPressed = TRUE;
			}
			else
				if ((Input & PAD_INPUT_UP) != 0 && (Input & PAD_INPUT_RIGHT) != 0)
				{
					Angle = -135.0f;
					PlayerKeyInfo.MoveKeyPressed = TRUE;
				}
				else
					if ((Input & PAD_INPUT_RIGHT) != 0 && (Input & PAD_INPUT_DOWN) != 0)
					{
						Angle = -45.0f;
						PlayerKeyInfo.MoveKeyPressed = TRUE;
					}
					else
						if ((Input & PAD_INPUT_DOWN) != 0 && (Input & PAD_INPUT_LEFT) != 0)
						{
							Angle = 45.0f;
							PlayerKeyInfo.MoveKeyPressed = TRUE;
						}
						else
							if ((Input & PAD_INPUT_LEFT) != 0)
							{
								Angle = 90.0f;
								PlayerKeyInfo.MoveKeyPressed = TRUE;
							}
							else
								if ((Input & PAD_INPUT_RIGHT) != 0)
								{
									Angle = -90.0f;
									PlayerKeyInfo.MoveKeyPressed = TRUE;
								}
								else
									if ((Input & PAD_INPUT_DOWN) != 0)
									{
										Angle = 0.0f;
										PlayerKeyInfo.MoveKeyPressed = TRUE;
									}
									else
										if ((Input & PAD_INPUT_UP) != 0)
										{
											Angle = 180.0f;
											PlayerKeyInfo.MoveKeyPressed = TRUE;
										}
		}

		//���f���X�V
		player.update(PlayerKeyInfo, Angle, goblin1);
		
		goblin1.update(player, PlayerKeyInfo.AttackKeyPressed);
		//�J�����X�V
		camera.update(player.Position);

		// �R�c���f���̕`��
		player.draw();
		goblin1.draw();

		MV1DrawModel(TestModelHandle);

		//�X�e�[�W�`��
		MV1DrawModel(stage);

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}
	player.terminateModel();
	goblin1.terminateModel();
	MV1DeleteModel(stage);
	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}
