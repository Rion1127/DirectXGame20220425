#pragma once
#include "WorldTransform.h"
#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
class Enemy
{
public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//����
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	Vector3 speed = { 0,0,-0.1f };

	void phase_Approach();
	void phase_Leave();
	
};
