#include "FieldManager.h"

FieldManager* FieldManager::GetInstance()
{
	static FieldManager instance;
	return &instance;
}

void FieldManager::Initialize()
{
	field_.Initialize();
}

void FieldManager::Update()
{
	field_.Update();
}

void FieldManager::Draw()
{
	field_.Draw();
}

void FieldManager::TitleUpdate()
{
	field_.TitleUpdate();
}
