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
	gameCamera.Update();
	
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssinpModel::SetLightGroup(lightManager_->GetLightGroup().get());

	skyDome_ = std::move(std::make_unique<Object3d>());
	skyDome_->SetModel(Model::CreateOBJ_uniptr("uvSphere", true));

	sphere_ = std::move(std::make_unique<Object3d>());
	sphere_->SetModel(Model::CreateOBJ_uniptr("uvSphere", true));

	testObj = std::move(std::make_unique<Object3d>());
	testObj->SetModel(Model::CreateOBJ_uniptr("uvSphere", true));

	//const wchar_t* modelFile = L"Resources/Alica/Alicia_solid_Unity.FBX";
	const wchar_t* modelFile = L"Resources/boneTest/moveCube/moveCube.gltf";
	//  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	//  L"Resources/FBX/untitled.glb"
	std::vector<Mesh> meshes;
	ImportSettings importSetting = {
		modelFile,
		meshes,
		false,
		true
	};
	testModel_.Create(modelFile);
	assimpObj_.SetModel(&testModel_);

	

	testSprite_.Ini();
	testSprite_.SetAnchor({ 0,0 });
	testSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Test"));

	testSound_ = SoundManager::LoadWave("selectSE.wav", "TestSE");

	JsonLoader::GetInstance()->LoadFile("test.json");

	JsonLoader::GetInstance()->SetObjects(&objects_, 0);
	for (auto& obj : objects_)
	{
		obj->Init();
	}
}

void GameScene::Update()
{
	Camera::scurrent_.eye_ = debugCamera.GetViewProjection()->eye_;
	Camera::scurrent_.up_ = debugCamera.GetViewProjection()->up_;
	Camera::scurrent_.target_ = debugCamera.GetViewProjection()->target_;
	Camera::scurrent_.Update();

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

	lightManager_->DebugUpdate();
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
