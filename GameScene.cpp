#include <random>
#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
GameScene::~GameScene()
{
	delete lightGroup;
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
	//BoardObject::Ini();
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

	lightGroup = LightGroup::Create();
	//light->SetLightDir()

	Object3d::SetLight(lightGroup);

	skyDome_ = Object3d::CreateOBJ_uniptr("sphere",true);
	worldTransform_.Ini();
	worldTransform_.position = { -1,0,0 };

	sphere_ = Object3d::CreateOBJ_uniptr("sphere",false);
	sphereWT_.Ini();
	sphereWT_.position = { 1,0,0 };

	floor_ = Object3d::CreateOBJ_uniptr("cube", true);
	floorWT_.Ini();
	floorWT_.position = { 0,-1,0 };
	floorWT_.scale = { 5,0.2f,5 };

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	/*lightGroup->SetPointLightActive(0, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[0] = 1.0f;
	pointLightPos[0] = 0.0f;*/

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	lightGroup->SetSpotLightActive(0, true);
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
	floorWT_.Update(*useVP);

	static Vector3 lightDir = { 0,1,5 };

	if (input_->PushKey(DIK_W)) lightDir.y -= 1.f;
	if (input_->PushKey(DIK_S)) lightDir.y += 1.f;
	if (input_->PushKey(DIK_A)) lightDir.x += 1.f;
	if (input_->PushKey(DIK_D)) lightDir.x -= 1.f;

	lightGroup->Update();

	
	

	/*lightGroup->SetAmbientColor({ ambientColor0[0],ambientColor0[1] ,ambientColor0[2] });
	lightGroup->SetDirLightDir(0, { lightDir0[0],lightDir0[1], lightDir0[2] });
	lightGroup->SetDirLightColor(0, { lightColor0[0],lightColor0[1] ,lightColor0[2] });

	lightGroup->SetDirLightDir(1, { lightDir1[0],lightDir1[1], lightDir1[2] });
	lightGroup->SetDirLightColor(1, { lightColor1[0],lightColor1[1] ,lightColor1[2] });

	lightGroup->SetDirLightDir(2, { lightDir2[0],lightDir2[1], lightDir2[2] });
	lightGroup->SetDirLightColor(2, { lightColor2[0],lightColor2[1] ,lightColor2[2] });*/

	/*lightGroup->SetPointLightPos(0, { pointLightPos[0],pointLightPos[1], pointLightPos[2] });
	lightGroup->SetPointLightColor(0, { pointLightColor[0],pointLightColor[1], pointLightColor[2] });
	lightGroup->SetPointLightAtten(0, { pointLightAtten[0],pointLightAtten[1], pointLightAtten[2] });*/

	lightGroup->SetSpotLightDir(0, { spotlightDir[0],spotlightDir[1], spotlightDir[2] });
	lightGroup->SetSpotLightPos(0, { spotlightPos[0],spotlightPos[1], spotlightPos[2] });
	lightGroup->SetSpotLightColor(0, { spotLightColor[0],spotLightColor[1], spotLightColor[2] });
	lightGroup->SetSpotLightAtten(0, { spotLightAtten[0],spotLightAtten[1], spotLightAtten[2] });
	lightGroup->SetSpotLightFactorAngle(0, { spotLightFactorAngle[0],spotLightFactorAngle[1] });

	ImGui::Begin("Light");
	/* ここに追加したいGUIを書く */

	ImGui::SetWindowPos(ImVec2(0,0));
	ImGui::SetWindowSize(ImVec2(500, 200));

	/*ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	ImGui::SliderFloat3("lightDir0", lightDir0,-1.f,1.0f);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);

	ImGui::SliderFloat3("lightDir1", lightDir1, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);

	ImGui::SliderFloat3("lightDir2", lightDir2, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);*/

	/*ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("pointLightPos", pointLightPos, -1.f, 1.0f);
	ImGui::SliderFloat3("point LightAtten", pointLightAtten, -5.f, 5.0f);*/

	ImGui::SliderFloat3("SpotLightDir", spotlightDir, -5.f, 5.0f);
	ImGui::ColorEdit3("spotLightColor", spotLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("SpotLightPos", spotlightPos, -5.f, 10.0f);
	ImGui::SliderFloat3("SpotLightAtten", spotLightAtten, -5.f, 5.0f);
	ImGui::SliderFloat3("SpotLightFactorAngle", spotLightFactorAngle, -100.f, 100.0f);

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
	floor_->DrawOBJ(&floorWT_);

	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	//BoardObject::PreDraw();


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

