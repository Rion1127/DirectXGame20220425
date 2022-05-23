#include "WorldTransform.h"

#include "Matrix.h"
#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "XMfloat.h"

//�P�ʍs��
Matrix4 WorldTransform::UnitMatrix() {
	Matrix4 unitMatrix;

	unitMatrix.m[0][0] = 1.0f;
	unitMatrix.m[1][1] = 1.0f;
	unitMatrix.m[2][2] = 1.0f;
	unitMatrix.m[3][3] = 1.0f;

	return unitMatrix;
}


//�A������
void WorldTransform::ScaleChange(float x, float y, float z)
{
	matResult.Scale.m[0][0] = scale_.x;
	matResult.Scale.m[1][1] = scale_.y;
	matResult.Scale.m[2][2] = scale_.z;
}
void WorldTransform::RotaChange()
{
	//X����]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//�e���̉�]�s�������
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}
void WorldTransform::ChangeTranslation()
{
	matResult.Trans.m[3][0] = translation_.x;
	matResult.Trans.m[3][1] = translation_.y;
	matResult.Trans.m[3][2] = translation_.z;
}

//�g�ݍ��킹
void WorldTransform::UpdataMatrix()
{
	//�s��̍���
	ScaleChange();
	matWorld_ = UnitMatrix();

	matWorld_ *= matResult.Scale;
	matWorld_ *= matResult.Rot;
	matWorld_ *= matResult.Trans;

	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	//�s��̓]��
	TransferMatrix();
}
