#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//テクスチャ読み込み
	textureHandle_ = textureHandle;
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, 3, 50);
	matrix.UpdateMatrix(worldTransform_);

}

void Enemy::Update()
{
	Vector3 speed = { 0,0,-0.1f };
	worldTransform_.translation_ += speed;


	matrix.UpdateMatrix(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}