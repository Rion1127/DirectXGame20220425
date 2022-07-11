#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "XMFLOAT3.h"
#include <random>
//#include "math.h"
#include "Vector3.h"

#define XM_PI 3.141592

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("enemy.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	EnemyModel = Model::Create();

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,10,-20 };
	////カメラの注視点座標を設定
	//viewProjection_.target = { 10,0,0 };
	////カメラ上方向ベクトルを設定（右上45度指定）
	//viewProjection_.up = { cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0.0f };

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = 100.0f * (XM_PI / 180);

	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定->手前
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定->奥
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参考するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_/*debugCamera_->GetViewProjection()*/);
#pragma region ライン描画座標
	//ライン描画座標X
	for (int i = 0; i < 30; i++) {
		vector3X_[i] = { 0,0,0 + (float)(2.5f * i) };
		vector3X_2[i] = { 75.0f,0.0f,0.0f + (float)(2.5f * i) };
	}
	colorX = { 255,0,0,1 };
	//ライン描画座標Z
	for (int i = 0; i < 30; i++) {
		vector3Z_[i] = { 0 + (float)(2.5f * i) ,0,0 };
		vector3Z_2[i] = { 0.0f + (float)(2.5f * i) ,0.0f, 75.0f };
	}
	colorZ = { 0,0,255,1 };
#pragma endregion

	////乱数シード生成期
	//std::random_device seed_gen;
	////メルセンヌ・ツイスターの乱数エンジン
	//std::mt19937_64 engine(seed_gen());
	//ワールドトランスフォームの初期化
	//for (WorldTransform& worldTransform : worldTransforms_) {
	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();
	//	//スケールチェンジ初期化
	//	matrix.ScaleChange(worldTransform, 1.0f, 1.0f, 1.0f, 1.0f);
	//	//乱数範囲の指定
	//	std::uniform_real_distribution<float> rotaDistX(0, XM_PI);
	//	std::uniform_real_distribution<float> rotaDistY(0, XM_PI);
	//	std::uniform_real_distribution<float> rotaDistZ(0, XM_PI);
	//	random.x = rotaDistX(engine);
	//	random.y = rotaDistY(engine);
	//	random.z = rotaDistZ(engine);
	//	//回転初期化
	//	matrix.RotaChange(worldTransform, random.x, random.y, random.z);
	//	//乱数範囲の指定
	//	std::uniform_real_distribution<float> transDistX(-10, 10);
	//	std::uniform_real_distribution<float> transDistY(-10, 10);
	//	std::uniform_real_distribution<float> transDistZ(-10, 10);
	
	//	random.x = transDistX(engine);
	//	random.y = transDistY(engine);
	//	random.z = transDistZ(engine);
	//	//平行移動
	//	matrix.ChangeTranslation(worldTransform, random.x, random.y, random.z);

	//	matrix.UpdataMatrix(worldTransform);
	//}

	////キャラクターの大本
	//worldTransforms_[PartID::kRoot].Initialize();
	////脊髄
	//worldTransforms_[PartID::kSpine].Initialize();
	//worldTransforms_[PartID::kSpine].parent_ = &worldTransforms_[PartID::kRoot];
	//worldTransforms_[PartID::kSpine].translation_ = { 0.0f,0.0f,0.0f };

	//matrix.UpdataMatrix(worldTransforms_[0]);

	////上半身
	//worldTransforms_[PartID::kChest].Initialize();
	//worldTransforms_[PartID::kChest].parent_ = &worldTransforms_[PartID::kSpine];
	//worldTransforms_[PartID::kChest].translation_ = { 0.0f,0.0f,0.0f };

	//worldTransforms_[PartID::kHead].Initialize();
	//worldTransforms_[PartID::kHead].parent_ = &worldTransforms_[PartID::kChest];
	//worldTransforms_[PartID::kHead].translation_ = { 0.0f,0.0f,3.0f };

	//worldTransforms_[PartID::kArmL].Initialize();
	//worldTransforms_[PartID::kArmL].parent_ = &worldTransforms_[PartID::kChest];
	//worldTransforms_[PartID::kArmL].translation_ = { 3.0f,0.0f,0.0f };

	//worldTransforms_[PartID::kArmR].Initialize();
	//worldTransforms_[PartID::kArmR].parent_ = &worldTransforms_[PartID::kChest];
	//worldTransforms_[PartID::kArmR].translation_ = { -3.0f,0.0f,0.0f };


	//下半身
	/*worldTransforms_[PartID::kHip].Initialize();
	worldTransforms_[PartID::kHip].parent_ = &worldTransforms_[PartID::kSpine];
	worldTransforms_[PartID::kHip].translation_ = { 0.0f,-3.0f,0.0f };

	worldTransforms_[PartID::kLegL].Initialize();
	worldTransforms_[PartID::kLegL].parent_ = &worldTransforms_[PartID::kHip];
	worldTransforms_[PartID::kLegL].translation_ = { 3.0f,-3.0f,0.0f };

	worldTransforms_[PartID::kLegR].Initialize();
	worldTransforms_[PartID::kLegR].parent_ = &worldTransforms_[PartID::kHip];
	worldTransforms_[PartID::kLegR].translation_ = { -3.0f,-3.0f,0.0f };*/

	//自キャラ生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	worldTransforms_.Initialize();
	matrix.ScaleChange(worldTransforms_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransforms_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransforms_,0, 0, 0);
	matrix.UpdateMatrix(worldTransforms_);

	//false バイオ :: true カメラ視点
	isCamera = false;
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->PushKey(DIK_0)) {
		if (isDebugCamera == false)isDebugCamera = true;
		else if (isDebugCamera == true)isDebugCamera = false;
		
	}

	if (isDebugCamera) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetMatView();
		viewProjection_.matProjection = debugCamera_->GetMatViewProjection();
		viewProjection_.UpdateMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
	}
#endif
	if (input_->TriggerKey(DIK_Q)) {
		if (isCamera == false)isCamera = true;
		else if (isCamera == true)isCamera = false;
	}
	//自キャラ更新
	player_->Update();
	
	



	//レイの当たり判定
	
	//ワールド座標を代入
	Vector3 objPos;
	Vector3 rayPos;
	objPos = worldTransforms_.translation_;
	rayPos = player_->worldTransform_.translation_;
	//レイの始点と終点を代入
	Vector3 rayStart;
	Vector3 rayEnd;
	rayStart = {
		player_->worldTransform_.translation_.x,
		player_->worldTransform_.translation_.y,
		player_->worldTransform_.translation_.z + 5.0f,
	};

	rayEnd = {
		player_->worldTransform_.translation_.x,
		player_->worldTransform_.translation_.y,
		player_->worldTransform_.translation_.z - 5.0f,
	};
	//始点と終点からレイのベクトル(a→)を求める
	Vector3 rayVec;
	rayVec = rayEnd - rayStart;
	//正規化(a→N)
	rayVec.normalize();
	//レイとオブジェクトのベクトル(b→)を求める
	Vector3 ABVec;
	ABVec = {
		worldTransforms_.translation_.x - player_->worldTransform_.translation_.x,
		worldTransforms_.translation_.y - player_->worldTransform_.translation_.y,
		worldTransforms_.translation_.z - player_->worldTransform_.translation_.z
	};
	//b→・a→N をray2ObjLengthに格納
	Vector3 ray2ObjLength;
	ray2ObjLength = ABVec.cross(rayVec);
	//Qを求める a→N * b→・a→N + P
	Vector3 Q;
	Q = rayVec * ABVec.cross(rayVec) + rayPos;
	//オブジェクトからレイの垂線(obj-Q)を求める
	Vector3 line;
	line = objPos - Q;
	//垂線の長さを求める
	line.length();
	
	//垂線の長さが半径+半径より短ければ当たってる
	bool isHit = false;



	//デバッグ表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"rayStart:%f,%f,%f",
		rayStart.x,
		rayStart.y,
		rayStart.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"rayEnd:%f,%f,%f",
		rayEnd.x,
		rayEnd.y,
		rayEnd.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"rayVec:%f,%f,%f",
		rayVec.x,
		rayVec.y,
		rayVec.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"ABVec:%f,%f,%f",
		ABVec.x,
		ABVec.y,
		ABVec.z);
	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"ray2ObjLength:%f,%f,%f",
		ray2ObjLength.x,
		ray2ObjLength.y,
		ray2ObjLength.z);
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Q:%f,%f,%f",
		Q.x,
		Q.y,
		Q.z);
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"line:%f,%f,%f",
		line.x,
		line.y,
		line.z);
	/*debugText_->SetPos(50, 190);
	debugText_->Printf(
		"lineLength:%f,%f,%f",
		lineLength.x,
		lineLength.y,
		lineLength.z);*/

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region ３Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	/*for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/
	//for (int i = 2; i <= PartID::kArmR; i++) {
	//	model_->Draw(worldTransforms_[i], viewProjection_/*debugCamera_->GetViewProjection()*/, textureHandle_);
	//}

	player_->Draw(viewProjection_);
	model_->Draw(worldTransforms_, viewProjection_, textureHandle_);

	//for (int i = 0; i < 30; i++) {
	//	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(vector3X_[i], vector3X_2[i], colorX);
	//	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(vector3Z_[i], vector3Z_2[i], colorZ);
	//}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

