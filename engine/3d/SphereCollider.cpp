#include "SphereCollider.h"

void SphereCollider::Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr)
{
    ICollider::Initialize(arg_name, arg_callback, arg_colMPtr);
    shape_ = SHAPE_SPHERE;
}
