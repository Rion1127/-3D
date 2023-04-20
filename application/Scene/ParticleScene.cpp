#include "ParticleScene.h"
#include "SceneManager.h"
#include "GameScene.h"

ParticleScene::~ParticleScene()
{
	delete lightGroup;
}

void ParticleScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
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
	
	lightGroup = LightGroup::Create();
	Object3d::SetLight(lightGroup);
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	WT_.Ini();

	texture_ = textureM->LoadGraph("keyBlade2.png");
	whiteTexture_ = textureM->LoadGraph("uv.png");

	const wchar_t* modelFile = L"Resources/FBX/Alica/Alicia_solid_Unity.FBX";
	//  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	//  L"Resources/FBX/untitled.glb"
	ImportSettings importSetting = {
		modelFile,
		meshes,
		false,
		true
	};

	//importSetting_.filename = modelFile;
	//importSetting_.meshes = meshes;
	//importSetting_.inverseU = false;
	//importSetting_.inverseV = true;

	importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));



	AssimpLoader loader;
	loader.Load(*importSetting_);

	texHandle_.resize(meshes.size());
	for (int i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer(directX->GetDevice());

		std::string texturename = WStringToString(meshes[i].diffuseMap);
		texHandle_[i] = textureM->LoadGraph(texturename);
	}

	testWT_.Ini();

	int a = 0;

	
	
	
}

void ParticleScene::Update()
{
	Camera::current.eye = debugCamera.GetViewProjection()->eye;
	Camera::current.up = debugCamera.GetViewProjection()->up;
	Camera::current.target = debugCamera.GetViewProjection()->target;
	Camera::current.Update();
	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update();

	object_.Update(*useVP);


	testWT_.Update();

	lightGroup->SetDirLightColor(0, { 1,1,1 });
	
	lightGroup->Update();

}

void ParticleScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();
	lightGroup->Draw(3);
	for (int i = 0; i < importSetting_->meshes.size(); i++)
	{
		TextureManager::GetInstance()->
			SetGraphicsDescriptorTable(DirectXCommon::GetInstance()->GetCommandList(), texHandle_[i]);

		importSetting_->meshes[i].Vertices.Draw(DirectXCommon::GetInstance()->GetCommandList(), &testWT_, 0);
	}

	///////////////////
	/////パーティクル////
	///////////////////
	ParticleManager::GetInstance()->PreDraw();

	//object_.Draw(whiteTexture_);
}
