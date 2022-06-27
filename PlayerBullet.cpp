#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initialize(Model* model, const XMFLOAT3& position)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform.Initialize();

	matrix.ScaleChange(worldTransform, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform, 0, 0, 0);
	worldTransform.translation_ = position;
	matrix.UpdataMatrix(worldTransform);
}

void PlayerBullet::Update()
{
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}
