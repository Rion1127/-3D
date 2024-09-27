#include "KnockBack.h"
#include "Util.h"
#include <imgui.h>

void KnockBack::Init()
{
	move_.attenValue = 0.02f;
	grabity_.SetGravityLimit(1.f);
	grabity_.SetAddValue(-0.07f);
}

void KnockBack::Update()
{
	MoveTo(Vector3(0, 0, 0), move_.attenValue, move_.attenVelocity);
	move_.attenVelocity.y = grabity_.GetGravityValue();
	grabity_.Update();
}

void KnockBack::DrawImGui()
{
	ImGui::Begin("knockBack");

	ImGui::Text("attenVelocity : x %.2f, y %.2f, z, % .2f",
		move_.attenVelocity.x,
		move_.attenVelocity.y,
		move_.attenVelocity.z);

	ImGui::Text("gravity : %.2f",
		grabity_.GetGravityValue());

	ImGui::End();
}

void KnockBack::GravityReset()
{
	grabity_.SetGrabity(0);
	move_.attenVelocity = Vector3(0, 0, 0);
}
