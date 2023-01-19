#include <random>
#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
GameScene::~GameScene()
{
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

}

void GameScene::Update()
{

	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();
	cameraUpdate();


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


	Quaternion rotation = MakeAxisAngle({0.0f,0.0f,1.0f},3.141592 / 2.0f);
	Vector3 pointY = { 0.0f,1.0f,0.0f };
	DirectX::XMMATRIX rotateMatrix = rotation.UpdateMatrix();
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	

	static float RotateByQuaternion[3] = { rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z };
	ImGui::InputFloat4("RotateByQuaternion", RotateByQuaternion);
	
	//ImGui::SliderFloat("Rot", &rot, 0.0f, ConvertAngleToRadian(360), "x = %.3f");


	ImGui::End();

}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);


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

