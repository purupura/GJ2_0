#include <Novice.h>
#define _USE_MATH_DEFINES
#include "Vector2.h"
#include <cmath>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include"imgui.h"
#include<time.h>

//ブロックの数
const int blockMax = 32;

struct Corners {
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftDown;
	Vector2 rightDown;
};

struct Player {
	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	float radiusW;
	float radiusH;
	float Lspeed;
	float Rspeed;
	float speed;
	unsigned int color;
	bool isDown;
	int downTimer;
};



//ブロックの種類を列挙
enum BlockType {
	normal,
	bad,
	bomb
};

struct Block {
	Vector2 pos[blockMax];
	float height;
	float widgh;
	BlockType blockType[blockMax];
	int digCounter[blockMax];
	bool isBroken[blockMax];
	unsigned int color[blockMax];
	Corners corners[blockMax];
};



void DrawBox(Vector2 pos, float widgh, float height, unsigned int color) {
	Novice::DrawBox(int(pos.x), int(pos.y), int(widgh), int(height), 0.0f, color, kFillModeSolid);
}

Corners PosUpdate(Vector2 a, float width, float height) {
	Corners c = {};

	c.leftTop.x = (a.x - width / 2.0f);
	c.leftTop.y = (a.y - height / 2.0f);

	c.rightTop.x = ((a.x + width / 2.0f));
	c.rightTop.y = (a.y - height / 2.0f);

	c.leftDown.x = (a.x - width / 2.0f);
	c.leftDown.y = ((a.y + height / 2.0f));

	c.rightDown.x = ((a.x + width / 2.0f));
	c.rightDown.y = ((a.y + height / 2.0f));

	return c;
}

int HitBox(Corners a, Corners b) {
	if (b.leftTop.x <= a.rightTop.x && a.leftTop.x < b.rightTop.x) {
		if (b.leftTop.y <= a.rightDown.y && a.leftTop.y < b.rightDown.y) {
			return 1;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

void DrawQuad(Corners a, int imageGH, int imageWidth, int imageHeight, unsigned int color) {
	Novice::DrawQuad(int(a.leftTop.x), int(a.leftTop.y), int(a.rightTop.x), int(a.rightTop.y), int(a.leftDown.x), int(a.leftDown.y), int(a.rightDown.x), int(a.rightDown.y), 0, 0, imageWidth, imageHeight, imageGH, color);
}

const char kWindowTitle[] = "2604_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float scroll = 0;

	//自機の宣言
	Player player = {
		  {200.0f, 360.0f},
		  {0.0f,20.0f},
		  {-0.8f,-0.8f},
		   100.0f,
		   200.0f,
		   5.0f,
		   5.0f,
		   5.0f,
		   0xffdab9ff
	};

	player.isDown = false;
	player.downTimer = 0
		;

	//プレイヤーの位置と半径から四角を計算
	Corners playerCorners = PosUpdate(player.pos, player.radiusW * 2.0f, player.radiusH * 2.0f);

	//ゲームシーンの切り替え
	enum GameScene {
		kTitle,
		kPlay,
		kResult
	};

	GameScene gameScene = kTitle;

	//ブロックに番号を振ることで乱数でブロックを生成
	int blockNum[blockMax];

	unsigned int currentTime = unsigned int(time(nullptr));
	srand(currentTime);

	/*int whiteGH = Novice::LoadTexture("white1x1.png");*/

	Block block;
	block.widgh = 100.0f;
	block.height = 500.0f;


	for (int i = 0;i < blockMax;i++) {
		//乱数でブロックの番号を決める
		blockNum[i] = rand() % 10;

		block.pos[i] = { 500.0f + (i * block.widgh),360.0f };

		block.corners[i] = PosUpdate(block.pos[i], block.widgh, block.height);

		//0~5が普通、6~8が罠、9が爆弾
		if (blockNum[i] >= 0 && blockNum[i] <= 5) {
			block.blockType[i] = normal;
			block.color[i] = WHITE;
		} else if (blockNum[i] >= 6 && blockNum[i] <= 8) {
			block.blockType[i] = bad;
			block.color[i] = BLUE;
		} else if (blockNum[i] == 9) {
			block.blockType[i] = bomb;
			block.color[i] = RED;
		}

		block.isBroken[i] = false;
		block.digCounter[i] = 0;



	}


	/*int time;*/

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (gameScene) {
		case GameScene::kTitle:
			//タイトル更新処理
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameScene = kPlay;
			}
			//タイトル更新処理ここまで

			//タイトル描画処理



			//タイトル描画処理ここまで

			break;

		case GameScene::kPlay:

			//プレイ更新処理
			/*if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameScene = kResult;
			}*/


			/*if (keys[DIK_RIGHT] && !preKeys[DIK_RIGHT]) {
				scroll = 5.0f;
			} else if (keys[DIK_LEFT] && !preKeys[DIK_LEFT]) {
				scroll = -5.0f;
			}*/

			//プレイヤーの位置と半径から四角を計算
			player.pos.x += player.speed;
			playerCorners = PosUpdate(player.pos, player.radiusW * 2.0f, player.radiusH * 2.0f);
			for (int i = 0;i < blockMax;i++) {
				//プレイヤーがブロックに当たっていたらストップする
				if (!block.isBroken[i]) {
					if (HitBox(playerCorners, block.corners[i])) {
						player.speed = 0.0f;
						//当たっている状態でマウスのボタンが押されたら掘る
						if (!player.isDown) {
							player.color = 0xffdab9ff;
							player.downTimer = 0;
							if (Novice::IsTriggerMouse(0)) {
								if (block.blockType[i] == normal) {
									block.digCounter[i]++;
									//爆弾なら即死
								} else if (block.blockType[i] == bomb) {
									gameScene = kResult;
									//badならダウンになる(未完成)
								} else {
									player.isDown = true;
								}
							}
						} else {
							//とりあえずダウンしていたら青色にする
							player.color = BLUE;
							player.downTimer++;
							//6秒立ったら元に戻る
							if (player.downTimer >= 360) {
								player.isDown = false;
							}
						}

						//badブロックは青色で掘る
						if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
							if (!player.isDown) {
								if (block.blockType[i] == bad) {
									block.digCounter[i]++;
									//ボムを掘ったら即死
								} else if (block.blockType[i] == bomb) {
									gameScene = kResult;
								}
							}
						}

						if (keys[DIK_B] && !preKeys[DIK_B]) {
							if (!player.isDown) {
								if (block.blockType[i] == bomb) {
									block.digCounter[i]++;
								} else if (block.blockType[i] == bad) {
									player.isDown = true;
								}
							}
						}

						if (block.digCounter[i] == 3) {
							block.isBroken[i] = true;
							if (scroll <= 3000) {
								scroll += 100;
							}
						}


					} else {
						player.speed = 5.0f;
					}
					break;
				} else {
					player.speed = 5.0f;
				}
			}

			//プレイ更新処理ここまで

			//プレイ描画処理
			//

			DrawBox({ 0.0f - scroll,0.0f }, 3750.0f, 110.0f, BLACK);
			DrawBox({ 0.0f - scroll,610.0f }, 3750.0f, 110.0f, BLACK);

			for (int i = 0;i < blockMax;i++) {
				if (!block.isBroken[i]) {
					/*Novice::DrawBox(int(block.pos[i].x - scroll), int(block.pos[i].y), int(block.widgh), int(block.height),0.0f, block.color[i], kFillModeSolid);*/
					/*DrawQuad(block.corners[i], whiteGH, 1, 1, block.color[i]);*/
					Novice::DrawBox(
						int(block.corners[i].leftTop.x - scroll), int(block.corners[i].leftTop.y),
						int(block.widgh), int(block.height),
						0.0f, block.color[i], kFillModeSolid
					);
				}
			}
			/*Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y, int(player.radiusW), int(player.radiusH), 0, player.color, kFillModeSolid);*/
			Novice::DrawBox(
				int(playerCorners.leftTop.x - scroll), int(playerCorners.leftTop.y),
				int(player.radiusW * 2), int(player.radiusH * 2),
				0.0f, player.color, kFillModeSolid
			);
			/*DrawQuad(playerCorners, whiteGH, 1, 1, player.color);*/

			//プレイ描画処理ここまで

			break;

		case GameScene::kResult:
			//result更新処理
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameScene = kTitle;
			}

			scroll = 0.0f;

			currentTime = unsigned int(time(nullptr));
			srand(currentTime);
			for (int i = 0;i < blockMax;i++) {

				block.isBroken[i] = false;
				block.digCounter[i] = false;
				//乱数でブロックの番号を決める
				blockNum[i] = rand() % 10;

				block.pos[i] = { 500.0f + (i * block.widgh),110.0f };

				if (blockNum[i] == 0 || blockNum[i] == 1 || blockNum[i] == 2 || blockNum[i] == 3 || blockNum[i] == 4 || blockNum[i] == 5) {
					block.blockType[i] = normal;
					block.color[i] = WHITE;
				} else if (blockNum[i] == 6 || blockNum[i] == 7 || blockNum[i] == 8) {
					block.blockType[i] = bad;
					block.color[i] = BLUE;
				} else if (blockNum[i] == 9) {
					block.blockType[i] = bomb;
					block.color[i] = RED;
				}
			}

			player.pos = { 200.0f, 360.0f };

			//result更新処理ここまで

			//result描画処理
			Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLUE, kFillModeSolid);
			//result描画処理ここまで

		}

		ImGui::Begin("Window");
		ImGui::Text("blockNum:%d,%d,%d,%d,%d",
			blockNum[0], blockNum[1], blockNum[2], blockNum[3], blockNum[4]);
		ImGui::DragFloat("player.Speed", &player.speed, 0.01f);
		ImGui::End();

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
