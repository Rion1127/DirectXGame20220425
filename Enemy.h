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
	//‰Šú‰»
	void Initialize(Model* model);
	//XV
	void Update();
	//•`‰æ
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	
};
