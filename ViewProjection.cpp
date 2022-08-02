#include "ViewProjection.h"

ViewProjection::ViewProjection()
{
	
	eye = { 0,0,0 };
	target = { 0,0,0 };
	up = { 0,0,0 };
}

void ViewProjection::SetEyePos(float x, float y, float z)
{
	eye.x = x;
	eye.y = y;
	eye.z = z;
}

void ViewProjection::SetEyePos(Vector3 pos)
{
	eye.x = pos.x;
	eye.y = pos.y;
	eye.z = pos.z;
}

void ViewProjection::SetTarget(float x, float y, float z)
{
	target.x = x;
	target.y = y;
	target.z = z;
}

void ViewProjection::SetTarget(Vector3 pos)
{
	target.x = pos.x;
	target.y = pos.y;
	target.z = pos.z;
}

void ViewProjection::SetUpVec(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

void ViewProjection::SetUpVec(Vector3 upVec)
{
	up.x = upVec.x;
	up.y = upVec.y;
	up.z = upVec.z;
}



void ViewProjection::Ini()
{
	matView =
		XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));

	//ìßéãìäâeçsóÒÇÃåvéZ
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		aspectRatio,
		0.1f, 1000.0f
	);
}

void ViewProjection::Update()
{
	matView =
		XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));

	//ìßéãìäâeçsóÒÇÃåvéZ
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		aspectRatio,
		0.1f, 1000.0f
	);
}

XMMATRIX ViewProjection::GetMatView()
{
	return matView;
}

XMMATRIX ViewProjection::GetMatProjection()
{
	return matProjection;
}
