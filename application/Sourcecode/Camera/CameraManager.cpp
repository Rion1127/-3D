#include "CameraManager.h"

CameraManager* CameraManager::GetInstance()
{
	static CameraManager instance;
	return &instance;
}

void CameraManager::Initialize()
{
	camera_.target_ = { 0,0,0 };
	camera_.eye_ = { 0,90,-90 };
}

void CameraManager::Update()
{
	
}
