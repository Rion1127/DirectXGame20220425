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
	
};
