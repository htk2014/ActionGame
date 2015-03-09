#include <vector>

#include "DxLib.h"

#include "player.h"
#include "enemy.h"
#include "camera.h"

void drawEnemies(Enemy* *enemyVec, int size);
void testEnemies(Player player, KeyInfo playerKeyInfo, Enemy* *enemyVec, int size);
void updateEnemies(Player player, KeyInfo playerKeyInfo, Enemy* *enemyVec, int size);
std::vector<Enemy*> updateEnemies(Player player, KeyInfo playerKeyInfo, std::vector<Enemy*> enemVec);
std::vector<Enemy*> createEnemies();
void createEnemies(std::vector<Enemy*> enemVec);
void terminateEnemies(std::vector<Enemy*> enemVec);

std::vector<Enemy*> killEnemies(std::vector<Enemy*> eVec);
// カメラの回転速度
#define CAMERA_ANGLE_SPEED		3.0f

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int    Input;
	float  Angle;
	//攻撃が敵にあたったか
	int AttackHit = FALSE;
	KeyInfo PlayerKeyInfo;

	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;
	//プレイヤー初期化用の構造体
	//プレイヤーオブジェクト生成
	Player player = Player();
	player.Position = VGet(500.0f, 0.0f, 0.0f);

	//敵オブジェクト生成
	//std::vector<Enemy*> 
	//enemyVec(createEnemies());
	//createEnemies(enemyVec);

	Enemy goblin1 = Goblin(VGet(-520.0f, 0.0f, 10.0f));

	Enemy goblin2 = Goblin(VGet(420.0f, 0.0f, -10.0f));

	std::vector<Enemy*> enemyVec;
	enemyVec = { &goblin1, &goblin2,
		&Goblin(VGet(-420.0f, 0.0f, -10.0f)),
		&Goblin(VGet(420.0f, 0.0f, -120.0f)), 
		&Goblin(VGet(-320.0f, 0.0f, -1222.0f)), 
		&Goblin(VGet(120.0f, 0.0f, -10.0f)), 
		&Goblin(VGet(520.0f, 0.0f, 530.0f)), 
		&Goblin(VGet(620.0f, 0.0f, 40.0f)), 
		&Goblin(VGet(-420.0f, 0.0f, -110.0f)), 
		&Goblin(VGet(820.0f, 0.0f, -85.0f)), 
		&Goblin(VGet(1420.0f, 0.0f, 0.0f)), 
		&Goblin(VGet(20.0f, 0.0f, -10.0f)), 
		&Goblin(VGet(-20.0f, 0.0f, -100.0f)), 
	};
	//std::vector<Enemy> enemyVec;
	//std::vector<Chara> otherVec;

	int stage = MV1LoadModel("Data/stage/Stage00.mv1");
	MV1SetPosition(stage, VGet(0.0f, 0.0f, 0.0f));
	//MV1SetPosition(TestModelHandle, { 0, 0, 0 });

	// 向きを初期化
	Angle = 0.0f;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//カメラ生成
	Camera camera = Camera(player.Position);

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

		PlayerKeyInfo.RockOnKeyPressed = FALSE;
		//ロックオンキーが押されたか？
		if (CheckHitKey(KEY_INPUT_A) == 1)
		{
			PlayerKeyInfo.RockOnKeyPressed = TRUE;
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
								//Angle = 0.0f;
								PlayerKeyInfo.MoveKeyPressed = TRUE;
							}
							else
								if ((Input & PAD_INPUT_RIGHT) != 0)
								{
									Angle = -90.0f;
									//Angle = 180.0f;
									PlayerKeyInfo.MoveKeyPressed = TRUE;
								}
								else
									if ((Input & PAD_INPUT_DOWN) != 0)
									{
										Angle = 0.0f;
										//Angle = -90.0f;
										PlayerKeyInfo.MoveKeyPressed = TRUE;
									}
									else
										if ((Input & PAD_INPUT_UP) != 0)
										{
											Angle = 180.0f;
											//Angle = 90.0f;
											PlayerKeyInfo.MoveKeyPressed = TRUE;
										}
		}

		//モデル更新
		//player.update(PlayerKeyInfo, Angle, goblin1);
		/*
		for (int i = 0; i < enemyVec.size(); i++){
		Enemy enem = enemyVec[i];
		//自分以外を含む配列を作成する
		std::vector<Chara> otherVec;
		for (int j = 0; j < enemyVec.size(); j++){
		//自分自身を除く
		if (&enemyVec[j] != &enem){
		otherVec.push_back(enemyVec[j]);
		}
		}
		//配列にプレイヤーを追加
		otherVec.push_back(player);

		//敵更新
		enem.update(player, PlayerKeyInfo.AttackKeyPressed, otherVec.data(), otherVec.size());
		}
		*/
		//otherVec = { goblin2, player };
		//otherVec = {player };
		//goblin1.update(player, PlayerKeyInfo.AttackKeyPressed, otherVec.data(), otherVec.size());

		//otherVec = { goblin1, player };
		//goblin2.update(player, PlayerKeyInfo.AttackKeyPressed, otherVec.data(), otherVec.size());
		//updateEnemies(player, PlayerKeyInfo, enemyVec.data(), enemyVec.size());
		enemyVec = updateEnemies(player, PlayerKeyInfo,enemyVec);
		enemyVec = killEnemies(enemyVec);

		//drawEnemies()
		//enemyVec = { goblin1};
		std::vector<Chara> tempEnemyVec;
		//enemyVecはENEMYポインタ形のベクター ->プレイヤーの更新処理用にENEMY形のベクターへ変換する
		for (int i = 0; i < enemyVec.size(); i++){
			tempEnemyVec.push_back(*enemyVec[i]);
		}
		player.update(PlayerKeyInfo, Angle, tempEnemyVec);

		//otherVec = { goblin1, goblin2 };
		//enemyVec = { goblin1, goblin2 };
		//player.update(PlayerKeyInfo, Angle, otherVec.data(), otherVec.size());
		//human.update(player, PlayerKeyInfo.AttackKeyPressed);
		//カメラ更新
		camera.update(player.Position, player.AttackAngle);
		//camera.update(player.Position);

		// ３Ｄモデルの描画
		//ステージ描画
		MV1DrawModel(stage);

		player.draw();

		drawEnemies(enemyVec.data(), enemyVec.size());
		//goblin1.draw();
		//goblin2.draw();
		/*
		for (int i = 0; i < enemyVec.size(); i++){
		enemyVec[i].draw();
		}
		*/
		//human.draw();

		//MV1DrawModel(TestModelHandle);


		// 裏画面の内容を表画面に反映
		ScreenFlip();
		//プレイヤーのHPが０になったら終了
		if (player.HP <= 0||enemyVec.size()<=0){
			break;
		}
	}
	player.terminateModel();
	terminateEnemies(enemyVec);
	//goblin1.terminateModel();
	//goblin2.terminateModel();
	MV1DeleteModel(stage);
	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

void drawEnemies(Enemy* *enemyVec, int size){
	for (int i = 0; i < size; i++){
		Enemy enem = *enemyVec[i];
		enem.draw();
	}
}

void updateEnemies(Player player, KeyInfo playerKeyInfo, Enemy* *enemyVec, int size){
	for (int i = 0; i < size; i++){
		Enemy enem = *enemyVec[i];
		
		//自分以外を含む配列を作成する
		std::vector<Chara> otherVec;
		for (int j = 0; j < size; j++){
			Enemy tempEnem = *enemyVec[j];
			//自分自身を除く
			if (i != j){
				otherVec.push_back(tempEnem);
			}
		}
		//配列にプレイヤーを追加
		otherVec.push_back(player);

		//敵更新
		enem.update(player, playerKeyInfo.AttackKeyPressed, otherVec.data(), otherVec.size());

	}
}

std::vector<Enemy*> updateEnemies(Player player, KeyInfo playerKeyInfo, std::vector<Enemy*> enemVec){
	for (int i = 0; i < enemVec.size(); i++){
		Enemy enem = *enemVec[i];

		//自分以外を含む配列を作成する
		std::vector<Chara> otherVec;
		for (int j = 0; j < enemVec.size(); j++){
			Enemy tempEnem = *enemVec[j];
			//自分自身を除く
			if (j != i){
				otherVec.push_back(tempEnem);
			}
		}
		//配列にプレイヤーを追加
		otherVec.push_back(player);

		//敵更新
		enem.update(player, playerKeyInfo.AttackKeyPressed, otherVec);
		*enemVec[i] = enem;
	}
	return enemVec;
}

std::vector<Enemy*> killEnemies(std::vector<Enemy*> eVec){
	std::vector<Enemy*> tempEnemyVec(eVec);
	for (int i = 0; i < tempEnemyVec.size(); i++){
		Enemy enem = *tempEnemyVec[i];
		if (enem.HP <= 0){
			eVec.erase(eVec.begin() + i);
			PlaySoundFile("./Data/sound/VO_dead_00.wav", DX_PLAYTYPE_BACK);
		}
	}
	return eVec;
}
std::vector<Enemy*> createEnemies(){
	//敵位置
	/*
	std::vector<VECTOR> PosVec =
	{
	{ 400.0f, 0.0f, 10.0f },
	{ 300.0f, 0.0f, -133.0f },
	{ 1000.0f, 0.0f, 120.0f },
	{ -400.0f, 0.0f, 320.0f },
	{ 1100.0f, 0.0f, 310.0f },
	{ -100.0f, 0.0f, 130.0f },
	{ 50.0f, 0.0f, 10.0f },
	{ 300.0f, 0.0f, -400.0f }
	};
	*/

	std::vector<VECTOR> PosVec =
	{
		VGet(400.0f, 0.0f, 10.0f),
		VGet(300.0f, 0.0f, 10.0f),
	};
	std::vector<Enemy*> enemVec;
	for (int i = 0; i < PosVec.size(); i++){
		Enemy* enem = &Goblin(VGet(400.0f, 0.0f, 10.0f));
		enemVec.push_back(enem);
	}
	return enemVec;
	//return{ &Goblin(VGet(400.0f, 0.0f, 10.0f)), &Goblin(VGet(300.0f, 0.0f, 10.0f)) };
}


void createEnemies(std::vector<Enemy*> enemVec){
	//敵位置
	/*
	std::vector<VECTOR> PosVec =
	{
		{ 400.0f, 0.0f, 10.0f },
		{ 300.0f, 0.0f, -133.0f },
		{ 1000.0f, 0.0f, 120.0f },
		{ -400.0f, 0.0f, 320.0f },
		{ 1100.0f, 0.0f, 310.0f },
		{ -100.0f, 0.0f, 130.0f },
		{ 50.0f, 0.0f, 10.0f }, 
		{ 300.0f, 0.0f, -400.0f }
	};
	*/

	std::vector<VECTOR> PosVec =
	{
		VGet(400.0f, 0.0f, 10.0f),
		VGet(300.0f, 0.0f, 10.0f),
	};

	for (int i = 0; i < PosVec.size(); i++){
		enemVec.push_back(&Goblin(VGet(400.0f, 0.0f, 10.0f)));
	}
	
}
void terminateEnemies(std::vector<Enemy*> enemVec){
	for (int i = 0; i < enemVec.size(); i++){
		Enemy enem = *enemVec[i];
		enem.terminateModel();
	}
}
