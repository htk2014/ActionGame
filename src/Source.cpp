#include <iostream>

#include "DxLib.h"

#include "player.h"
#include "enemy.h"
#include "camera.h"

// カメラの回転速度
#define CAMERA_ANGLE_SPEED		3.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int    Input;
	float  Angle;
	//攻撃が敵にあたったか
	int AttackHit = FALSE;
	KeyInfo PlayerKeyInfo;
	VECTOR Position = VGet(500.0f, 0.0f, 0.0f);

	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;
	//プレイヤー初期化用の構造体
	//プレイヤーオブジェクト生成
	Player player = Player();
	player.Position = Position;
	//敵オブジェクト生成
	Goblin goblin1 = Goblin({ 520, 0, 1 });

	int TestModelHandle = MV1LoadModel("Data/testCube2.mv1");
	//int anim = MV1LoadModel("Data/stable/testModel1_anim.mv1");
	  //AnimAttachIndex = MV1AttachAnim(ModelHandle, 0, anim, FALSE);

	int stage = MV1LoadModel("Data/stage/Stage00.mv1");
	MV1SetPosition(stage, { 0, 0, 0 });
	MV1SetPosition(TestModelHandle, { 0, 0, 0 });

	// 向きを初期化
	Angle = 0.0f;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//カメラ生成
	Camera camera = Camera();

	// 背景の色を灰色にする
	SetBackgroundColor(0, 0, 0);

	// メインループ(何かキーが押されたらループを抜ける)
	while (ProcessMessage() == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		// ZCSXキーでカメラの操作
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
		// 攻撃しているかどうかのフラグを倒す
		PlayerKeyInfo.AttackKeyPressed = FALSE;
		//攻撃キーが押されたか？
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)
		{
			PlayerKeyInfo.AttackKeyPressed = TRUE;
		}

		// ゲームパッド＋キーボードの入力を取得
		Input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 移動しているかどうかのフラグを倒す
		PlayerKeyInfo.MoveKeyPressed = FALSE;
		//攻撃フラグがたっていないか？
		if (!PlayerKeyInfo.AttackKeyPressed){
			// 方向入力の判定
			// ( 最初に斜め入力の判定を、その後上下左右単体の入力の判定をしています )
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

		//モデル更新
		player.update(PlayerKeyInfo, Angle, goblin1);
		
		goblin1.update(player, PlayerKeyInfo.AttackKeyPressed);
		//カメラ更新
		camera.update(player.Position);

		// ３Ｄモデルの描画
		player.draw();
		goblin1.draw();

		MV1DrawModel(TestModelHandle);

		//ステージ描画
		MV1DrawModel(stage);

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}
	player.terminateModel();
	goblin1.terminateModel();
	MV1DeleteModel(stage);
	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}
