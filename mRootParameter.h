#pragma once
#include <d3d12.h>

class mRootParameter
{
public:

	void Ini();

	D3D12_ROOT_PARAMETER* GetEntity() { return entity; }

	 D3D12_ROOT_PARAMETER entity[3];
};

