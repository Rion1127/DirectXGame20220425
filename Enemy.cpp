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
	switch(phase_) {
	case Phase::Approach:
	default:
		phase_Approach();
		break;
	case Phase::Leave:
		phase_Leave();
		break;
	}

	matrix.UpdateMatrix(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::phase_Approach()
{
	speed = { 0,0,-0.3f };
	//移動（ベクトルを加算）
	worldTransform_.translation_ += speed;
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::phase_Leave()
{
	speed = { 0.3f,0,0 };
	//移動（ベクトルを加算）
	worldTransform_.translation_ -= speed;
}
