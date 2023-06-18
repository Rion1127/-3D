#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "JsonLoader.h"

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
	gameCamera.UpdateLookAt();
	
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLightGroup(lightManager_->GetLightGroup().get());



	JsonLoader::GetInstance()->LoadFile("test2.json","Test");

	JsonLoader::GetInstance()->SetObjects(&objects_,"Test");
	JsonLoader::GetInstance()->SetCamera(&gameCamera, "Test");
	for (auto& obj : objects_)
	{
		obj->Init();
	}
}

void GameScene::Update()
{
	Camera::scurrent_ = debugCamera.GetViewProjection();

	Camera::scurrent_= &gameCamera;
	Camera::scurrent_->Update(CameraMode::LookTo);

	//カメラ更新
	debugCamera.Update();

	if (Key::TriggerKey(DIK_R)) {
		JsonLoader::GetInstance()->LoadFile("test2.json", "Test");

		JsonLoader::GetInstance()->SetObjects(&objects_, "Test");
		JsonLoader::GetInstance()->SetCamera(&gameCamera, "Test");
	}
	cameraUpdate();

	lightManager_->DebugUpdate();

	for (auto& obj : objects_)
	{
		obj->Update();
	}
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Model::PreDraw();

	for (auto& obj : objects_)
	{
		obj->Draw();
	}
	
	AssimpObject3D::PreDraw();

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
