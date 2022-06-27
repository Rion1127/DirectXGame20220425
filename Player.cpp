
#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, 0, 0);
	matrix.UpdateMatrix(worldTransform_);
}

void Player::Update()
{
	Move();
	Rotation();
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//行列更新
	matrix.UpdateMatrix(worldTransform_);
}

void Player::Draw(ViewProjection viewProjection_)
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//デバッグ表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"player:(%f,%f,%f)", worldTransform_/*[PartID::Root]*/.translation_.x,
		worldTransform_/*[PartID::Root]*/.translation_.y,
		worldTransform_/*[PartID::Root]*/.translation_.z);
	//デバッグ表示
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"player:(%f)", worldTransform_/*[PartID::Root]*/.rotation_.y);
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//キャラクターの移動速度
	const float playerSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move = { -playerSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { playerSpeed,0,0 };
	}

	if (input_->PushKey(DIK_W)) {
		move = { 0,playerSpeed,0 };
	}
	else if (input_->PushKey(DIK_S)) {
		move = { 0,-playerSpeed,0 };
	}

	//注視点移動（ベクトルの加算）
	worldTransform_/*[PartID::Root]*/.translation_.x += move.x;
	worldTransform_/*[PartID::Root]*/.translation_.y += move.y;
	worldTransform_/*[PartID::Root]*/.translation_.z += move.z;
	//移動限界座標
	const float moveLimitX = 35;
	const float moveLimitY = 18;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
}

void Player::Rotation()
{
	const float rotaSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= rotaSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += rotaSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		worldTransform_.scale_.y += rotaSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.scale_.y -= rotaSpeed;
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float bulletSpeed = 1.0f;
		Vector3 velocity(0, 0, bulletSpeed);
		Vector3 resultVec(0, 0, 0);
		Vector3 frontVec(0, 0, 1);

		//プレイヤーの正面ベクトル
		resultVec.x = {
		  cos(worldTransform_.rotation_.y) * frontVec.x
		  + sin(worldTransform_.rotation_.y) * frontVec.z
		};
		resultVec.z = {
			-sin(worldTransform_.rotation_.y) * frontVec.x +
			cos(worldTransform_.rotation_.y) * frontVec.z
		};

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique< PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, resultVec);
		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}
