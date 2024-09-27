#include "ColliderObject.h"

void ColliderObject::Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr)
{
	
	// コライダー|初期化関数実行
	collider_.Initialize(arg_name, arg_callback, arg_colMPtr);
}
