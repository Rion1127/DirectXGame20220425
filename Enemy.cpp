#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include <memory>
#include <list>
#include "Enemy.h"

Enemy::Enemy()
{
	
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//�e�N�X�`���ǂݍ���
	textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, 3, 50);
	matrix.UpdateMatrix(worldTransform_);


	//Shot();

	phese_ApproachIni();
}



void Enemy::Update()
{
	//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	matrix.UpdateMatrix(worldTransform_);
}

void Enemy::Shot()
{
	////�e�̑��x
	//const float bulletSpeed = 1.0f;
	//Vector3 velocity(0, 0, bulletSpeed);
	//Vector3 resultVec(0, 0, 0);
	//Vector3 frontVec(0, 0, 1);

	////�v���C���[�̐��ʃx�N�g��
	//resultVec.x = {
	//  cos(worldTransform_.rotation_.y) * frontVec.x
	//  + sin(worldTransform_.rotation_.y) * frontVec.z
	//};
	//resultVec.z = {
	//	-sin(worldTransform_.rotation_.y) * frontVec.x +
	//	cos(worldTransform_.rotation_.y) * frontVec.z
	//};
	
	assert(player_);
	
	
	
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique< EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, resultVec);
	//�e��o�^����
	bullets_.push_back(std::move(newBullet));

	

	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
}



Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//�f�o�b�O�\��
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
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ += speed;

	//���˃^�C�}�[�J�E���g�_�E��
	shotCoolTime--;
	//�w�莞�ԂɒB����
	if (shotCoolTime <= 0) {
		Shot();
		//���˃^�C�}�[�̏�����
		shotCoolTime = kFireInterval;
	}

	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::phese_ApproachIni()
{
	shotCoolTime = kFireInterval;
}

void Enemy::phase_Leave()
{
	speed = { 0.3f,0,0 };
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ -= speed;
}

void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::phase_Approach,
	&Enemy::phase_Leave
};