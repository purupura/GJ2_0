#include <Novice.h>
#define _USE_MATH_DEFINES
#include "Vector2.h"
#include <cmath>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include"imgui.h"
#include<time.h>

struct Player {
	Vector2 pos;
	float width;
	float height;
	bool isDig;
};

const int blockMax = 32;
const char kWindowTitle[] = "2604_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Player player;
	player.pos = { 0.0f,0.0f };
	player.isDig = false;

	enum GameScene {
		kTitle,
		kPlay,
		kResult
	};

	GameScene gameScene = kTitle;

	int blockNum[blockMax];

	unsigned int currentTime = unsigned int(time(nullptr));
	srand(currentTime);

	for (int i = 0;i < blockMax;i++) {
		blockNum[i] = rand() % 3;
	}

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
			if (keys[DIK_SPACE]&&!preKeys[DIK_SPACE]) {
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

			if (Novice::IsTriggerMouse(0)) {
				player.isDig = true;
			}
			//プレイ更新処理ここまで

			//プレイ描画処理

			Novice::DrawBox(0, 0, 1280, 720, 0.0f, RED, kFillModeSolid);

			//プレイ描画処理ここまで

			break;

		case GameScene::kResult:
			//result更新処理
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameScene = kTitle;
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
