#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "XMfloat.h"

#define XM_PI 3.141592

void UnitMatrix(Matrix4 m);

void ScaleChange(WorldTransform worldTransform, float m1, float m2, float m3, float m4);

void RotaChange(WorldTransform worldTransform, float m1, float m2, float m3);

void ChangeTranslation(WorldTransform worldTransform, float tx, float ty, float tz);

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参考するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
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
	//スケールチェンジ初期化
	ScaleChange(worldTransform_, 1.0f, 1.0f, 1.0f, 1.0f);
	//回転初期化
	RotaChange(worldTransform_, XM_PI / 4.0f, XM_PI / 4.0f, XM_PI / 4.0f);
	//平行移動
	ChangeTranslation(worldTransform_, 5.0f, 0.0f, 5.0f);
}

void GameScene::Update() {
	debugCamera_->Update();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Chest:rotation_.z(%f)", sin(worldTransform_.rotation_.z));
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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	for (int i = 0; i < 30; i++) {
		//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
		PrimitiveDrawer::GetInstance()->DrawLine3d(vector3X_[i], vector3X_2[i], colorX);
		//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
		PrimitiveDrawer::GetInstance()->DrawLine3d(vector3Z_[i], vector3Z_2[i], colorZ);
	}

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


//単位ベクトル
void UnitMatrix(Matrix4 m) {
	Matrix4 unitMatrix;

	unitMatrix.m[0][0] = 1.0f;
	unitMatrix.m[1][1] = 1.0f;
	unitMatrix.m[2][2] = 1.0f;
	unitMatrix.m[3][3] = 1.0f;

	m = unitMatrix;
}
//スケーリングチェンジ
void ScaleChange(WorldTransform worldTransform, float m1, float m2, float m3, float m4) {
	//X,Y,Z方向のスケーリングを設定
	worldTransform.scale_ = { m1,m2,m3 };
	//スケーリング行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = worldTransform.scale_.x;
	matScale.m[1][1] = worldTransform.scale_.y;
	matScale.m[2][2] = worldTransform.scale_.z;
	matScale.m[3][3] = m4;

	UnitMatrix(worldTransform.matWorld_);
	worldTransform.matWorld_ *= matScale;

	//行列の転送
	worldTransform.TransferMatrix();
}
//回転
void RotaChange(WorldTransform worldTransform, float m1, float m2, float m3) {
	//X、Y、Z軸周りの回転角を設定
	worldTransform.rotation_ = {m1,m2,m3 };

	//合成用回転行列を宣言
	Matrix4 matRot;
	//X軸回転行列を宣言
	Matrix4 matRotX, matRotY,matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//各軸の回転行列を合成
	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	UnitMatrix(worldTransform.matWorld_);
	worldTransform.matWorld_ *= matRot;
	//行列の転送
	worldTransform.TransferMatrix();

	
}

void ChangeTranslation(WorldTransform worldTransform, float tx, float ty, float tz)
{
	//Ｘ，Ｙ，Ｚ軸周りの平行移動を設定
	worldTransform.translation_ = { tx,ty,tz };
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;

	UnitMatrix(worldTransform.matWorld_);
	worldTransform.matWorld_ *= matTrans;

	//行列の転送
	worldTransform.TransferMatrix();
}
