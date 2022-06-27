#include "DebugCamera.h"

float DebugCamera::GetCameraUpX()
{
	return viewProjection_.up.x;
}

float DebugCamera::GetCameraUpY()
{
	return viewProjection_.up.y;
}

float DebugCamera::GetCameraUpZ()
{
	return viewProjection_.up.z;
}
