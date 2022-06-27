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
	//èâä˙âª
	void Initialize(Model* model, const XMFLOAT3& position);
	//çXêV
	void Update();
	//ï`âÊ
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};

