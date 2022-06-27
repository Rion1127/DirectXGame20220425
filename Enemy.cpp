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
	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();
	
	matrix.UpdateMatrix(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//デバッグ表示
	debugText_->SetPos(50, 190);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"Phase:%d",
		phase_);
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

void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::phase_Approach,
	&Enemy::phase_Leave
};