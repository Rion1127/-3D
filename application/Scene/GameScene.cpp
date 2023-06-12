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
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	Model::Ini();
	Sprite::StaticIni();
	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	
	lightGroup = std::make_shared<LightGroup>();
	lightGroup->Init();
	Model::SetLight(lightGroup);
	AssinpModel::SetLightGroup(lightGroup);

	skyDome_ = std::move(std::make_unique<Object3d>());
	skyDome_->SetModel(Model::CreateOBJ("uvSphere", true));

	sphere_ = std::move(std::make_unique<Object3d>());
	sphere_->SetModel(Model::CreateOBJ("uvSphere", true));

	testObj = std::move(std::make_unique<Object3d>());
	testObj->SetModel(Model::CreateOBJ("uvSphere", true));

	//const wchar_t* modelFile = L"Resources/Alica/Alicia_solid_Unity.FBX";
	const wchar_t* modelFile = L"Resources/boneTest/moveCube/moveCube.gltf";
	//  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	//  L"Resources/FBX/untitled.glb"
	ImportSettings importSetting = {
		modelFile,
		meshes,
		false,
		true
	};
	testModel_.Create(modelFile);
	assimpObj_.SetModel(&testModel_);

	//平行光源
	if (lightType_ == LIGHTTYPE::DIRECTION_)
	{
		lightGroup->SetDirLightActive(0, true);
		lightGroup->SetDirLightActive(1, false);
		lightGroup->SetDirLightActive(2, false);
		lightColor0[0] = 1;
		lightColor0[1] = 1;
		lightColor0[2] = 1;
	}
	////点光源
	//else if (lightType_ == LIGHTTYPE::POINT_) {
	//	lightGroup->SetPointLightActive(0, true);
	//	lightGroup->SetPointLightActive(1, false);
	//	lightGroup->SetPointLightActive(2, false);
	//	pointLightPos[0] = 0.5f;
	//	pointLightPos[0] = 1.0f;
	//	pointLightPos[0] = 0.0f;
	//}
	////スポットライト
	//else if (lightType_ == LIGHTTYPE::SPOT_) {
	//	lightGroup->SetSpotLightActive(0, true);
	//}

	testSprite_.Ini();
	testSprite_.SetAnchor({ 0,0 });
	testSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Test"));
}

void GameScene::Update()
{
	Camera::current_.eye_ = debugCamera.GetViewProjection()->eye_;
	Camera::current_.up_ = debugCamera.GetViewProjection()->up_;
	Camera::current_.target_ = debugCamera.GetViewProjection()->target_;
	Camera::current_.Update();

	//カメラ更新
	debugCamera.Update();

	gameCamera.Update();
	cameraUpdate();

	assimpObj_.SetPos({ 0,0,0 });
	assimpObj_.Update();

	static float rotY = 0;
	rotY += 0.01f;
	
	skyDome_->SetPos({ -2,0,0 });
	skyDome_->SetRot({ 0,rotY,0 });
	sphere_->SetPos({ 2,0,0 });
	sphere_->SetRot({ 0,rotY,0 });
	testObj->SetPos({ 0,0,-2 });
	testObj->SetRot({ 0,rotY,0 });

	skyDome_->Update();
	sphere_->Update();
	testObj->Update();

	testSprite_.Update();

	if (input_->TriggerKey(DIK_RETURN))
	{
		lightType_++;
		if (lightType_ >= LIGHTTYPE::NUMEND_)lightType_ = 0;

		//平行光源
		if (lightType_ == LIGHTTYPE::DIRECTION_)
		{
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
			//スポットライト無効化
			lightGroup->SetSpotLightActive(0, false);
		}
		//点光源
		else if (lightType_ == LIGHTTYPE::POINT_) {
			lightGroup->SetPointLightActive(0, true);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);
			pointLightPos[0] = 0.5f;
			pointLightPos[0] = 1.0f;
			pointLightPos[0] = 0.0f;
			//平行光源無効化
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
		}
		//スポットライト
		else if (lightType_ == LIGHTTYPE::SPOT_) {
			lightGroup->SetSpotLightActive(0, true);
			//点光源
			lightGroup->SetPointLightActive(0, false);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);
		}
	}

	lightGroup->Update();

	//平行光源
	if (lightType_ == LIGHTTYPE::DIRECTION_)
	{
		DirectionalLightUpdate();
	}
	else if (lightType_ == LIGHTTYPE::POINT_)
	{
		PointLightUpdate();
	}
	else if (lightType_ == LIGHTTYPE::SPOT_)
	{
		SpotLightUpdate();
	}

	//lightGroup->SetAmbientColor({ ambientColor0[0],ambientColor0[1] ,ambientColor0[2] });

	lightGroup->SetCircleShadowDir(0, { circleShadowDir[0],circleShadowDir[1], circleShadowDir[2] });
	lightGroup->SetCircleShadowAtten(0, { circleShadowAtten[0],circleShadowAtten[1], circleShadowAtten[2] });
	lightGroup->SetCircleShadowFactorAngle(0, { circleShadowFactorAngle[0],circleShadowFactorAngle[1] });
	int i = 0;
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Model::PreDraw();

	skyDome_->Draw();
	
	AssimpObject3D::PreDraw();
	assimpObj_.Draw();

	// パイプラインステートとルートシグネチャの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetPipelineObjects("Toon")->GetPipelineStateAlpha());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects("Toon")->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	
	



	////////////
	//スプライト//
	////////////
	Sprite::PreDraw();
	//testSprite_.Draw();
	//testSprite_.DrawImGui();
	//testSprite_.DrawImGui();
	
}

void GameScene::CheckAllCollision()
{

}

void GameScene::cameraUpdate()
{

}

void GameScene::DirectionalLightUpdate()
{
	//lightGroup->SetAmbientColor({ ambientColor0[0],ambientColor0[1] ,ambientColor0[2] });
	//0番目の平行光源


	lightGroup->SetDirLightDir(0, { lightDir0[0],lightDir0[1], lightDir0[2] });
	lightGroup->SetDirLightColor(0, { lightColor0[0],lightColor0[1] ,lightColor0[2] });
	//1番目の平行光源
	lightGroup->SetDirLightDir(1, { lightDir1[0],lightDir1[1], lightDir1[2] });
	lightGroup->SetDirLightColor(1, { lightColor1[0],lightColor1[1] ,lightColor1[2] });
	//2番目の平行光源
	lightGroup->SetDirLightDir(2, { lightDir2[0],lightDir2[1], lightDir2[2] });
	lightGroup->SetDirLightColor(2, { lightColor2[0],lightColor2[1] ,lightColor2[2] });

	ImGui::Begin("DirectionalLight");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	//ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	static uint32_t num0clicked = 1;
	if (ImGui::Button("Num 0 Light"))num0clicked++;
	if (num0clicked & 1)
	{
		lightGroup->SetDirLightActive(0, true);
		ImGui::SameLine();
		ImGui::Text("Num 0 Light ON!");
	}
	else lightGroup->SetDirLightActive(0, false);
	ImGui::SliderFloat3("lightDir0", lightDir0, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);

	static uint32_t num1clicked = 0;
	if (ImGui::Button("Num 1 Light"))num1clicked++;
	if (num1clicked & 1) {
		lightGroup->SetDirLightActive(1, true);
		ImGui::SameLine();
		ImGui::Text("Num 1 Light ON!");
	}
	else lightGroup->SetDirLightActive(1, false);

	ImGui::SliderFloat3("lightDir1", lightDir1, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	static uint32_t num2clicked = 0;
	if (ImGui::Button("Num 2 Light"))num2clicked++;
	if (num2clicked & 1) {
		lightGroup->SetDirLightActive(2, true);
		ImGui::SameLine();
		ImGui::Text("Num 2 Light ON!");
	}
	else lightGroup->SetDirLightActive(2, false);
	ImGui::SliderFloat3("lightDir2", lightDir2, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);

	ImGui::End();
}

void GameScene::PointLightUpdate()
{
	lightGroup->SetPointLightPos(0, { pointLightPos[0],pointLightPos[1], pointLightPos[2] });
	lightGroup->SetPointLightColor(0, { pointLightColor[0],pointLightColor[1], pointLightColor[2] });
	lightGroup->SetPointLightAtten(0, { pointLightAtten[0],pointLightAtten[1], pointLightAtten[2] });

	ImGui::Begin("PointLight");
	/* ここに追加したいGUIを書く */

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	//ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("pointLightPos", pointLightPos, -5.f, 5.0f);
	ImGui::SliderFloat3("point LightAtten", pointLightAtten, -5.f, 5.0f);

	ImGui::InputFloat3("CircleShadowDir", circleShadowDir);
	ImGui::InputFloat3("CircleShadowAtten", circleShadowAtten);
	ImGui::InputFloat2("CircleShadowFactorAngle", circleShadowFactorAngle);
	ImGui::InputFloat3("pos", fighterPos);

	ImGui::End();
}

void GameScene::SpotLightUpdate()
{
	lightGroup->SetSpotLightDir(0, { spotlightDir[0],spotlightDir[1], spotlightDir[2] });
	lightGroup->SetSpotLightPos(0, { spotlightPos[0],spotlightPos[1], spotlightPos[2] });
	lightGroup->SetSpotLightColor(0, { spotLightColor[0],spotLightColor[1], spotLightColor[2] });
	lightGroup->SetSpotLightAtten(0, { spotLightAtten[0],spotLightAtten[1], spotLightAtten[2] });
	lightGroup->SetSpotLightFactorAngle(0, { spotLightFactorAngle[0],spotLightFactorAngle[1] });

	ImGui::Begin("SpotLight");
	/* ここに追加したいGUIを書く */

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	//ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	ImGui::SliderFloat3("SpotLightDir", spotlightDir, -5.f, 5.0f);
	ImGui::ColorEdit3("spotLightColor", spotLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("SpotLightPos", spotlightPos, -5.f, 10.0f);
	ImGui::SliderFloat3("SpotLightAtten", spotLightAtten, -5.f, 5.0f);
	ImGui::SliderFloat3("SpotLightFactorAngle", spotLightFactorAngle, -100.f, 100.0f);

	ImGui::InputFloat3("CircleShadowDir", circleShadowDir);
	ImGui::InputFloat3("CircleShadowAtten", circleShadowAtten);
	ImGui::InputFloat2("CircleShadowFactorAngle", circleShadowFactorAngle);
	ImGui::InputFloat3("pos", fighterPos);

	ImGui::End();
}

