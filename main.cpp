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

struct Player {
	Vector2 pos;
	float width;
	float height;
	float speed;
	//掘っているか
	bool isDig;
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
};

void DrawBox(Vector2 pos, float widgh, float height,unsigned int color) {
	Novice::DrawBox(int(pos.x), int(pos.y), int(widgh), int(height), 0.0f, color, kFillModeSolid);
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

	Player player;
	player.pos = { 0.0f,0.0f };
	player.isDig = false;



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



	Block block;
	block.widgh = 100.0f;
	block.height = 500.0f;

	for (int i = 0;i < blockMax;i++) {
		//乱数でブロックの番号を決める
		blockNum[i] = rand() % 10;

		block.pos[i] = { 500.0f + (i * block.widgh),110.0f };

		if (blockNum[i] == 0||blockNum[i] == 1|| blockNum[i] == 2|| blockNum[i] == 3 || blockNum[i] == 4 || blockNum[i] == 5) {
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

	
	int time;

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

			ImGui::Begin("Window");
			ImGui::Text("blockNum:%d,%d,%d,%d,%d", blockNum[0], blockNum[1], blockNum[2], blockNum[3], blockNum[4]);
			ImGui::End();

			//タイトル描画処理ここまで

			break;

		case GameScene::kPlay:

			//プレイ更新処理
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameScene = kResult;
			}

			if (keys[DIK_RIGHT] && !preKeys[DIK_RIGHT]) {
				scroll++;
			} else if (keys[DIK_LEFT] && !preKeys[DIK_LEFT]) {
				scroll--;
			}

			//プレイ更新処理ここまで

			//プレイ描画処理
			//

			DrawBox({ 0.0f-scroll,0.0f }, 1280.0f, 110.0f, BLACK);
			DrawBox({ 0.0f-scroll,610.0f }, 1280.0f, 110.0f, BLACK);
			for (int i = 0;i < blockMax;i++) {
				block.pos[i].x -= scroll;
				DrawBox(block.pos[i], block.widgh, block.height, block.color[i]);
			}

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
					block.color[i]=RED;
				}
			}

			//result更新処理ここまで

			//result描画処理
			Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLUE, kFillModeSolid);
			//result描画処理ここまで

		}

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
