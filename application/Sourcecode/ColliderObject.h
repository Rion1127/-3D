#pragma once
#include "ColliderManager.h"
#include "SphereCollider.h"
#include <string>
class ColliderObject
{
public:
	void Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr);

	virtual void CallBack() = 0;

protected:
	SphereCollider collider_;
};

