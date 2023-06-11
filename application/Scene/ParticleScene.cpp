#include "ParticleScene.h"
#include "SceneManager.h"
#include "GameScene.h"

ParticleScene::~ParticleScene()
{
}

void ParticleScene::Ini()
{
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();
	debugCamera.Update();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();
	
	lightGroup = std::make_shared<LightGroup>();
	lightGroup->Init();
	Model::SetLight(lightGroup);
	AssinpModel::SetLightGroup(lightGroup);
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	

	const wchar_t* modelFile = L"Resources/boneTest/testCube.gltf";
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

	//importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));

	////AssimpLoader loader;
	//AssimpLoader::GetInstance()->Load(*importSetting_);

	//texHandle_.resize(meshes.size());
	//for (int i = 0; i < importSetting_->meshes.size(); i++)
	//{
	//	importSetting_->meshes[i].Vertices.CreateBuffer();

	//	std::string texturename = WStringToString(meshes[i].diffuseMap);
	//	texHandle_[i] = textureM->LoadGraph(texturename);
	//}

	//testModel_.Create(modelFile);
	//assimpObj_.SetModel(&testModel_);
	
}

void ParticleScene::Update()
{
	Camera::current_.eye_ = debugCamera.GetViewProjection()->eye_;
	Camera::current_.up_ = debugCamera.GetViewProjection()->up_;
	Camera::current_.target_ = debugCamera.GetViewProjection()->target_;
	Camera::current_.Update();
	//カメラ更新
	if (Key::PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update();

	object_.Update(*useVP);


	testWT_.Update();

	lightGroup->SetDirLightColor(0, { 1,1,1 });
	
	lightGroup->Update();

	assimpObj_.Update();

}

void ParticleScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Model ::PreDraw();
	lightGroup->Draw(3);
	
	//assimpObj_.Draw();

	///////////////////
	/////パーティクル////
	///////////////////
	ParticleManager::GetInstance()->PreDraw();

	object_.Draw(whiteTexture_);
}
