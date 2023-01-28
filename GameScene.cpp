#include <random>
#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
GameScene::~GameScene()
{
	delete light;
}

void GameScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	Object3d::Ini();
	BoardObject::Ini();
	Sprite::StaticIni();
	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();

	gameCamera.Ini();
	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();

	light = Light::Create();
	light->Init();

	Object3d::SetLight(light);

	skyDome_ = Object3d::CreateOBJ_uniptr("sphere",true);
	worldTransform_.Ini();
	worldTransform_.position = { -1,0,0 };

	sphere_ = Object3d::CreateOBJ_uniptr("sphere",false);
	sphereWT_.Ini();
	sphereWT_.position = { 1,0,0 };
}

void GameScene::Update()
{

	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();
	cameraUpdate();

	worldTransform_.rotation.y += 0.01f;
	sphereWT_.rotation.y += 0.01f;

	worldTransform_.Update(*useVP);
	sphereWT_.Update(*useVP);

	light->Update();

	ImGui::Begin("quaternion");
	/* ここに追加したいGUIを書く */

	//// Menu Bar
	//Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	//Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };

	//Quaternion identity = identity.IdentityQuaternion();
	//Quaternion conj = q1.Conjugate();
	//Quaternion inv = q1.Inverse();
	//Quaternion normal = q1.Normalize();
	//Quaternion mul1 = q1.Multiply(q2);
	//Quaternion mul2 = q2.Multiply(q1);
	//float norm = q1.Norm();

	//ImGui::LabelText("label", "Value");

	//static float Identity[4] = { identity.x, identity.y, identity.z, identity.w };
	//ImGui::InputFloat4("Identity", Identity);

	//static float Conj[4] = { conj.x, conj.y, conj.z, conj.w };
	//ImGui::InputFloat4("Conjugate", Conj);

	//static float Inv[4] = { inv.x, inv.y, inv.z, inv.w };
	//ImGui::InputFloat4("Inverse", Inv);

	//static float Normal[4] = { normal.x, normal.y, normal.z, normal.w };
	//ImGui::InputFloat4("Normalize", Normal);

	//static float Mul1[4] = { mul1.x, mul1.y, mul1.z, mul1.w };
	//ImGui::InputFloat4("Multiply(q1,q2)", Mul1);

	//static float Mul2[4] = { mul2.x, mul2.y, mul2.z, mul2.w };
	//ImGui::InputFloat4("Multiply(q2,q1)", Mul2);
	//
	//static float Norm = norm;
	//ImGui::InputFloat("Norm", &Norm);


	/*Quaternion rotation = MakeAxisAngle({0.0f,0.0f,1.0f},3.141592 / 2.0f);
	Vector3 pointY = { 0.0f,1.0f,0.0f };
	DirectX::XMMATRIX rotateMatrix = rotation.UpdateMatrix();
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);


	static float RotateByQuaternion[3] = { rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z };
	ImGui::InputFloat4("RotateByQuaternion", RotateByQuaternion);*/


	//Quaternion rotation0 = MakeAxisAngle({ 0.71,0.71f,0.0f }, 0.3f);
	//Quaternion rotation1 = MakeAxisAngle({ 0.71,0.0f, 0.71f }, 3.141592f);
	////Quaternion rotation1 = { -rotation0.x,-rotation0.y,-rotation0.z,-rotation0.w };

	//Quaternion intertpolate0 = rotation0.Slerp(rotation1, 0.0f);
	//Quaternion intertpolate1 = rotation0.Slerp(rotation1, 0.3f);
	//Quaternion intertpolate2 = rotation0.Slerp(rotation1, 0.5f);
	//Quaternion intertpolate3 = rotation0.Slerp(rotation1, 0.7f);
	//Quaternion intertpolate4 = rotation0.Slerp(rotation1, 1.0f);

	//Quaternion intertpolate5 = rotation0.Slerp(rotation1, t);

	//float Intertpolate0[4] = { intertpolate0.x, intertpolate0.y, intertpolate0.z, intertpolate0.w };
	//ImGui::InputFloat4("Slerp(rotation1, 0.0f)", Intertpolate0);

	//float Intertpolate1[4] = { intertpolate1.x, intertpolate1.y, intertpolate1.z, intertpolate1.w };
	//ImGui::InputFloat4("Slerp(rotation1, 0.3f)", Intertpolate1);

	//float Intertpolate2[4] = { intertpolate2.x, intertpolate2.y, intertpolate2.z, intertpolate2.w };
	//ImGui::InputFloat4("Slerp(rotation1, 0.5f)", Intertpolate2);

	//float Intertpolate3[4] = { intertpolate3.x, intertpolate3.y, intertpolate3.z, intertpolate3.w };
	//ImGui::InputFloat4("Slerp(rotation1, 0.7f)", Intertpolate3);

	//float Intertpolate4[4] = { intertpolate4.x, intertpolate4.y, intertpolate4.z, intertpolate4.w };
	//ImGui::InputFloat4("Slerp(rotation1, 1.0f)", Intertpolate4);

	//float Intertpolate5[4] = { intertpolate5.x, intertpolate5.y, intertpolate5.z, intertpolate5.w };
	//ImGui::InputFloat4("Slerp(rotation1, t)", Intertpolate5);

	//ImGui::SliderFloat("t", &t, 0.0f, 1.0f);

	Vector3 direction1 = { 1.0f,1.0f,1.0f };
	Vector3 direction2 = { 1.0f,0.0f,1.0f };
	Quaternion dirToDir = DirectionToDirection(direction1, direction2);


	float DirToDir[4] = { dirToDir.x, dirToDir.y, dirToDir.z, dirToDir.w };
	ImGui::InputFloat4("Dir1 To Dir2", DirToDir);


	ImGui::End();

}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);

	skyDome_->DrawOBJ(&worldTransform_);
	sphere_->DrawOBJ(&sphereWT_);


	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw();


	////////////
	//スプライト//
	////////////
	Sprite::PreDraw();

}



void GameScene::CheckAllCollision()
{

}

void GameScene::cameraUpdate()
{

}

