#pragma once
#include "Matrix.h"
#include <cassert>

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
class PlayerBullet
{
public:
	~PlayerBullet();
	//������
	void Initialize(Model* model, const XMFLOAT3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};

