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
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//入力
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//行動フェーズ
	enum class Phase {
		Approach,	//接近する
		Leave,		//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;
	Vector3 speed = { 0,0,-0.1f };

	void phase_Approach();
	void phase_Leave();
	
};
